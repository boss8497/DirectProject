#include "pch.h"
#include "FbxClass.h"

FbxClass::FbxClass()
{
}

FbxClass::~FbxClass()
{
	this->Destroy();
}

bool FbxClass::Inite(BaseObject* parent, const char* filePath)
{
	m_manager = FbxManager::Create();
	if (!m_manager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}

	//Create an IOsetting object, this object holdds all import/eexport setting
	FbxIOSettings* ios = FbxIOSettings::Create(m_manager, IOSROOT);
	m_manager->SetIOSettings(ios);


	//Load plugins from the executable directory (optional)

	FbxString lPath = FbxGetApplicationDirectory();
	m_manager->LoadPluginsDirectory(lPath.Buffer());


	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	m_scene = FbxScene::Create(m_manager, "My Scene");
	if (!m_scene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}

	return this->LoadScene(parent, filePath);
}

void FbxClass::Update(UpdateQueue* uQueue)
{
}

void FbxClass::Destroy()
{
	// delete m_meshSkinning
	for (auto it = m_meshSkinning.begin(); it != m_meshSkinning.end();)
	{
		FbxMeshSkinning* ms = (*it);
		it = m_meshSkinning.erase(it);
		ms->Destroy();
		delete ms;
	}

	if (m_MeshData)
	{
		m_MeshData->Destroy();
		delete m_MeshData;
		m_MeshData = nullptr;
	}

	/*매니저 삭제 이슈*/
	/*if (m_manager)
	{
		m_manager->Destroy();
	}*/
}

bool FbxClass::Skinning_Inite(ID3D11Device * device, HWND hwnd)
{
	for (unsigned int i = 0; i < m_meshSkinning.size(); ++i)
	{
		m_meshSkinning[i]->Inite(device, hwnd, m_MeshData->m_skeleton);
	}
	return true;
}

bool FbxClass::LoadScene(BaseObject* parent, const char * filePath)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	int i, lAnimStackCount;
	bool lStatus;

	/*파ㅇ일*/
	GetFilePath(filePath);
	printf("%s\n", m_file_Path);

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(m_manager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(filePath, -1, m_manager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());
		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
		m_manager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}



	// Import the scene.
	lStatus = lImporter->Import(m_scene);

	// Destroy the importer.
	lImporter->Destroy();

	/*새로 추가*/
	this->m_MeshData = new FbxMeshData(this->GetHierarchy(parent));
	//아래 함수에서 m_meshSkining을 채움
	this->MeshDataSet();
	this->SetSkeletonTransform();

	return lStatus;
}

NodeData* FbxClass::GetHierarchy(BaseObject* parent)
{
	std::stack<FbxNode*> NodeStack;
	FbxNode* lRootNode = m_scene->GetRootNode();
	int index = 0;
	BaseObject* rootObj = new BaseObject();
	BaseObject* tmp = nullptr;
	NodeData* tmpdata = nullptr;

	parent->AddChild(rootObj);
	NodeData* rootData = new NodeData(lRootNode, rootObj, lRootNode->GetName());


	rootObj->SetName(lRootNode->GetName());

	for (int i = 0; i < lRootNode->GetChildCount(); ++i)
	{
		FbxNode* child = lRootNode->GetChild(i);

		BaseObject* child_obj = new BaseObject(child->GetName());
		rootObj->AddChild(child_obj);

		NodeData* child_data = new NodeData(child, child_obj, child->GetName());
		rootData->AddChild(child_data);
		while (true)
		{
			for (int j = 0; j < child->GetChildCount(); ++j)
			{
				//stack push
				NodeStack.push(child->GetChild(j));


				//data set
				tmp = new BaseObject(child->GetChild(j)->GetName());
				child_obj->AddChild(tmp);

				tmpdata = new NodeData(child->GetChild(j), tmp, child->GetChild(j)->GetName());
				child_data->AddChild(tmpdata);
			}


			if (NodeStack.empty())
				break;

			child = NodeStack.top();
			child_obj = rootObj->FindChild(child->GetName());
			child_data = rootData->FindChild(child->GetName());
			NodeStack.pop();


		}
	}


	m_root = rootData;
	return rootData;
}

void FbxClass::MeshDataSet()
{
	std::stack<NodeData*> ObjectStack;
	NodeData* rootNode = m_MeshData->m_rootNode;

	/*트리 순회*/
	for (int i = 0; i < rootNode->GetChildCount(); ++i)
	{
		NodeData* child = rootNode->m_child[i];

		while (true)
		{
			/*데이터 처리*/
			this->ProcessNode(child);

			for (int j = 0; j < child->GetChildCount(); ++j)
			{
				ObjectStack.push(child->GetChild(j));
			}


			if (ObjectStack.empty())
				break;

			child = ObjectStack.top();
			ObjectStack.pop();
		}

	}

}

void FbxClass::ProcessNode(NodeData * node)
{
	FbxNodeAttribute::EType lAttributeType;
	FbxMeshSkinning* skinning = nullptr; 
	FbxAMatrix m;
	Vector::Vector3 tmp;
	m_MeshData->m_NowNode = node;

	if (node->m_node->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL Node Attribute\n\n");
	}
	else
	{
		lAttributeType = (node->m_node->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		default:
			m = node->m_node->EvaluateGlobalTransform();

			tmp = m.GetT();
			node->m_obj->GetCompponent<Transform>()->Position = tmp;
			tmp = m.GetR();
			node->m_obj->GetCompponent<Transform>()->Rotate = tmp;
			tmp = m.GetS();
			node->m_obj->GetCompponent<Transform>()->Scale = tmp;
			break;

		case FbxNodeAttribute::eSkeleton:
			break;

		case FbxNodeAttribute::eMesh:
			//Process Mesh
			this->ProcessMesh(node);
			//Process Ani 스켈레톤이 있으면 데이터 뽑기
			if (!m_MeshData->m_skeleton.empty())
				ProcessJointsAndAnimations(node);
			AssociateMaterialToMesh(node);
			ProcessMaterial(node);


			/*Skinnign초기화*/
			skinning = new FbxMeshSkinning(m_MeshData);
			m_meshSkinning.push_back(skinning);
			m_MeshData->ClearData();
			break;
		}
	}


	/*여기에서 이제 받은 데이터로 내 머터리얼을 만들어 GameObject에 Componnent 추가 하기*/

}

void FbxClass::ProcessMesh(NodeData * node)
{
	FbxMesh* mesh = node->m_node->GetMesh();
	m_MeshData->m_TriangleCount = mesh->GetPolygonCount();
	/*해당 메쉬노드의 모든 버텍스 데이터 가져오기*/
	/*여기(해당 vertex index)에 Blending index랑 weight 정보를 넣어줘야됨*/
	FbxVector4* Vertices = mesh->GetControlPoints();
	FbxGeometryElementNormal* Normal = mesh->GetElementNormal(0);
	FbxLayerElementArrayTemplate<FbxVector2> *UV = nullptr;
	unsigned int VertexCount = 0;
	mesh->GetTextureUV(&UV, FbxLayerElement::eTextureDiffuse);

	for (int i = 0; i < m_MeshData->m_TriangleCount; ++i)
	{
		Triangle currTriangle;
		m_MeshData->m_Triangle.push_back(currTriangle);


		//3 = 삼각형 vertex 개수
		for (int j = 0; j < 3; ++j)
		{
			int VertexIndex = mesh->GetPolygonVertex(i, j);
			FbxVertexType vertext;
			//ZeroMemory(&vertext, sizeof(FbxVertexType));

			//set Position
			vertext.m_Vertice.position.x = (float)Vertices[VertexIndex].mData[0];
			vertext.m_Vertice.position.y = (float)Vertices[VertexIndex].mData[1];
			vertext.m_Vertice.position.z = (float)Vertices[VertexIndex].mData[2];

			//set UV
			//ReadUV(mesh, VertexIndex, mesh->GetTextureUVIndex(i, j), 0, vertext.m_Vertice.texture);
			vertext.m_Vertice.texture.x = (float)UV->GetAt(mesh->GetTextureUVIndex(i, j)).mData[0];
			vertext.m_Vertice.texture.y = 1.0f - (float)UV->GetAt(mesh->GetTextureUVIndex(i, j)).mData[1];

			//set Normal
			vertext.m_Vertice.normal.x = (float)Normal->GetDirectArray().GetAt(VertexIndex).mData[0];
			vertext.m_Vertice.normal.y = (float)Normal->GetDirectArray().GetAt(VertexIndex).mData[2];
			vertext.m_Vertice.normal.z = -(float)Normal->GetDirectArray().GetAt(VertexIndex).mData[1];

			m_MeshData->m_Vertex.push_back(vertext);
			m_MeshData->m_Triangle.back().mIndices.push_back(VertexCount);
			++VertexCount;
		}

	}
}

void FbxClass::ProcessJointsAndAnimations(NodeData * node)
{
	FbxMesh* currMesh = node->m_node->GetMesh();
	int numOfDeformers = currMesh->GetDeformerCount();
	FbxVector4 s, r, t;
	s = node->m_node->GetGeometricScaling(FbxNode::eSourcePivot);
	r = node->m_node->GetGeometricRotation(FbxNode::eSourcePivot);
	t = node->m_node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxAMatrix geometryTransform(t, r, s);


	/*메쉬 노드가 들어옴  메쉬랑 스켈레톤노드랑 틀림*/
	/*메쉬 노드랑 스켈레톤 노드랑 연결을 해주는거임*/
	for (int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));

		if (!currSkin)
		{
			continue;
		}

		//FbxCluster는 애니메이션이 있는 뼈대중 Skinning데이터에 대한 정보만 있다.
		int numOfClusters = currSkin->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			/*스켈레톤 노드를 가진 데이터를 찾음*/
			int currJointIndex = m_MeshData->GetSkeletonIndex(currJointName);

			if (!m_MeshData->m_skeleton[currJointIndex]->m_setBindpose)
			{
				//printf("Bind %s\n", currJointName.c_str());
				FbxAMatrix transformMatrix;
				FbxAMatrix transformLinkMatrix;
				FbxAMatrix globalBindposeInverseMatrix;
				FbxAMatrix TransformMatrix;
				TransformMatrix.SetIdentity();
				TransformMatrix.mData[1][1] = 0.0f;
				TransformMatrix.mData[1][2] = 1.0f;
				TransformMatrix.mData[2][2] = 0.0f;
				TransformMatrix.mData[2][1] = 1.0f;

				//Get Parent World Matrix
				currCluster->GetTransformMatrix(transformMatrix);
				//Get World Matrix
				currCluster->GetTransformLinkMatrix(transformLinkMatrix);
				/*printf("%s\n", m_MeshData->m_skeleton[currJointIndex]->m_node->m_name.c_str());
				printf("Parent Matrix\n");
				this->PrintFbxVector4(transformMatrix.GetT());
				this->PrintFbxVector4(transformMatrix.GetR());
				this->PrintFbxVector4(transformMatrix.GetS());
				printf("LinkMatrix\n");*/

				FbxVector4 rot = transformLinkMatrix.GetR();
				FbxVector4 transl = transformLinkMatrix.GetT();
				transl = transl * 0.001f;
				rot.mData[1] = -rot.mData[1];
				//transformLinkMatrix.SetR(rot);
				//transformLinkMatrix.SetT(transl);

				/*this->PrintFbxVector4(transformLinkMatrix.GetT());
				this->PrintFbxVector4(transformLinkMatrix.GetR());
				this->PrintFbxVector4(transformLinkMatrix.GetS());*/
				globalBindposeInverseMatrix = transformMatrix.Inverse() * transformLinkMatrix * geometryTransform;

				/*printf("Parent Matrix.inverse\n");
				this->PrintFbxMatrix(transformMatrix.Inverse());


				printf("Parent Matrix.inverse * Link Matrix\n");
				this->PrintFbxVector4(globalBindposeInverseMatrix.GetT());
				this->PrintFbxVector4(globalBindposeInverseMatrix.GetR());
				this->PrintFbxVector4(globalBindposeInverseMatrix.GetS());
				printf("------------------------------------------\n");*/
				//스켈레톤에 위치 행렬을 넣어줌
				//이게 GameObject(Skeleton)의 위치를 결정함
				m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse = globalBindposeInverseMatrix.Transpose();
				m_MeshData->m_skeleton[currJointIndex]->m_setBindpose = true;
				Transform* tr = m_MeshData->m_skeleton[currJointIndex]->m_node->m_obj->GetCompponent<Transform>();
				/*FbxVector4 translatioon = m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetT();;
				FbxVector4 rotation = m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetR();
				translatioon.Set(translatioon.mData[0], translatioon.mData[1], -translatioon.mData[2]);
				rotation.Set(-rotation.mData[0], -rotation.mData[1], rotation.mData[2]);*/
				tr->Fbx_worldMatrix = globalBindposeInverseMatrix;
				tr->Position = m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetT();
				tr->Rotate = m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetR();
				tr->Scale = m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetS();

				FbxAMatrix tessstM;
				tessstM.SetT(m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetT());
				tessstM.SetR(m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetR());
				tessstM.SetS(m_MeshData->m_skeleton[currJointIndex]->m_GlobalBindposeInverse.GetS());
				/*printf("-----------------fbx Matrix-------------------------\n");
				this->PrintFbxMatrix(tessstM);
				printf("-----------------fbx Matrix-------------------------\n");
				this->PrintFbxMatrix(globalBindposeInverseMatrix);
				printf("-----------------my Matrix-------------------------\n");
				tr->GetLocalMatrix().Print();
				printf("-----------------my Matrix-------------------------\n");
				tr->Position.print();
				tr->Rotate.print();
				tr->Scale.print();*/
			}

			int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (int i = 0; i < numOfIndices; ++i)
			{
				int VertexIndex = currCluster->GetControlPointIndices()[i];

				/*if (m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[0] == 0) m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[0] = currJointIndex;
				if (m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[1] == 0) m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[1] = currJointIndex;
				if (m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[2] == 0) m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[2] = currJointIndex;
				if (m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[3] == 0) m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[3] = currJointIndex;
				if (m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[0] == 0) m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[0] = currCluster->GetControlPointWeights()[i];
				if (m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[1] == 0) m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[1] = currCluster->GetControlPointWeights()[i];
				if (m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[2] == 0) m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[2] = currCluster->GetControlPointWeights()[i];
				if (m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[3] == 0) m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[3] = currCluster->GetControlPointWeights()[i];*/
				/*Bone Index count 4*/
				if (m_MeshData->m_Vertex[VertexIndex].m_index > 3)
				{
					printf("m_MeshData->m_Vertex[VertexIndex].m_index Over Error\n");
					continue;
				}
				m_MeshData->m_Vertex[VertexIndex].m_Skeleton_index[m_MeshData->m_Vertex[VertexIndex].m_index] = currJointIndex;
				m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[m_MeshData->m_Vertex[VertexIndex].m_index] = currCluster->GetControlPointWeights()[i];
				//m_MeshData->m_Vertex[VertexIndex].m_BlendingWeight[m_MeshData->m_Vertex[VertexIndex].m_index] = 0.0f;
				m_MeshData->m_Vertex[VertexIndex].m_index =+ 1;
			}


			/*애니메이션*/
			FbxAnimStack* currAnimStack = m_scene->GetSrcObject<FbxAnimStack>(0);
			if (currAnimStack)
			{
				FbxString animStackName = currAnimStack->GetName();
				m_MeshData->m_skeleton[currJointIndex]->m_AnimationName = animStackName.Buffer();

				FbxTakeInfo* takeInfo = m_scene->GetTakeInfo(animStackName);
				FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
				FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
				m_MeshData->m_skeleton[currJointIndex]->m_AnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;

				Keyframe** currAnim = &m_MeshData->m_skeleton[currJointIndex]->m_Animation;

				for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
				{
					FbxTime currTime;
					currTime.SetFrame(i, FbxTime::eFrames24);
					*currAnim = new Keyframe();
					(*currAnim)->mFrameNum = i;
					FbxAMatrix currentTransformOffset = node->m_node->EvaluateGlobalTransform(currTime) * geometryTransform;
					(*currAnim)->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
					currAnim = &((*currAnim)->mNext);
				}//
			}//
		}
	}
}

void FbxClass::ProcessMaterial(NodeData * node)
{
	FbxMesh* mesh = node->m_node->GetMesh();
	int materialcount = mesh->GetNode()->GetSrcObjectCount<FbxSurfaceMaterial>();
	m_MeshData->m_MaterialCount = materialcount;

	for (int i = 0; i < materialcount; i++)
	{
		FbxSurfaceMaterial *material = (FbxSurfaceMaterial*)mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(i);

		if (material)
		{
			FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			const FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(0));
			const FbxFileTexture* filetexture = FbxCast<FbxFileTexture>(texture);


			if (filetexture)
			{
				m_MeshData->m_Texture_FilePath.push_back(GetTextureFilePath(filetexture->GetRelativeFileName()));
			}
		}
	}
}

void FbxClass::AssociateMaterialToMesh(NodeData * node)
{
	FbxLayerElementArrayTemplate<int>* materialIndices;
	FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;
	FbxMesh* currMesh = node->m_node->GetMesh();

	if (currMesh->GetElementMaterial())
	{
		materialIndices = &(currMesh->GetElementMaterial()->GetIndexArray());
		materialMappingMode = currMesh->GetElementMaterial()->GetMappingMode();

		if (materialIndices)
		{
			switch (materialMappingMode)
			{
			case FbxGeometryElement::eByPolygon:
			{
				if (materialIndices->GetCount() == m_MeshData->m_TriangleCount)
				{
					for (int i = 0; i < m_MeshData->m_TriangleCount; ++i)
					{
						unsigned int materialIndex = materialIndices->GetAt(i);
						m_MeshData->m_Triangle[i].mMaterialIndex = materialIndex;
					}
				}
			}
			break;

			case FbxGeometryElement::eAllSame:
			{
				unsigned int materialIndex = materialIndices->GetAt(0);
				for (int i = 0; i < m_MeshData->m_TriangleCount; ++i)
				{
					m_MeshData->m_Triangle[i].mMaterialIndex = materialIndex;
				}
			}
			break;

			default:
				throw std::exception("Invalid mapping mode for material\n");
			}
		}
	}
}

void FbxClass::ReadUV(FbxMesh * inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, Vector::Vector2 & outUV)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}

void FbxClass::SetSkeletonTransform()
{
	//스켈레톤 포지션 설정
	for (auto it = m_MeshData->m_skeleton.begin(); it != m_MeshData->m_skeleton.end(); it++)
	{
		FbxVector4 t = (*it)->m_GlobalBindposeInverse.GetT();
		FbxVector4 s = (*it)->m_GlobalBindposeInverse.GetS();
		FbxVector4 r = (*it)->m_GlobalBindposeInverse.GetR();
		Vector::Vector3 translate;
		Vector::Vector3 scale;
		Vector::Vector3 rotate;


		translate.Set(static_cast<float>(t.mData[0]),
			static_cast<float>(t.mData[1]),
			static_cast<float>(t.mData[2]));
		scale.Set(static_cast<float>(s.mData[0]), 
			static_cast<float>(s.mData[1]),
			static_cast<float>(s.mData[2]));
		rotate.Set(static_cast<float>(r.mData[0]),
			static_cast<float>(r.mData[1]),
			static_cast<float>(r.mData[2]));


		//printf("%s\n", (*it)->m_node->m_name.c_str());
		//this->PrintFbxVector4(t);
		//this->PrintFbxVector4(r);
		//this->PrintFbxVector4(s);
		//printf("Import\n");
		//translate.print();
		//rotate.print();
		//scale.print();

		Transform* tr = (*it)->m_node->m_obj->GetCompponent<Transform>();
		tr->SetPosition(translate);
		tr->SetScale(scale);
		tr->SetRotate(rotate);
	}

	/*Test*//*Test*//*Test*//*Test*//*Test*//*Test*/
	/*NodeData* ParentNodeData = m_MeshData->m_rootNode;
	NodeData* childData = nullptr;
	std::stack<NodeData*> DataStack;

	while (true)
	{
		if (ParentNodeData->m_obj->Parent)
		{
			Transform* Ptr = ParentNodeData->m_obj->Parent->GetCompponent<Transform>();
			Transform* Ctr = ParentNodeData->m_obj->GetCompponent<Transform>();
			Vector::Vector3 pos;



			printf("%s\n", Ctr->GameObject()->Name.c_str());
			printf("----------------\n");
			Ctr->Position.print();
			Ctr->Rotate.print();
			Ctr->Scale.print();
			printf("----------------\n");
			printf("Parent : %s\n", Ptr->GameObject()->Name.c_str());
			printf("----------------\n");
			Ptr->Position.print();
			Ptr->Rotate.print();
			Ptr->Scale.print();
			printf("----------------\n");

			Matrix::Matrix Pmat = Ptr->GetLocalMatrix().Inverse();
			Matrix::Matrix Cmat = Ctr->GetLocalMatrix();
			Matrix::Matrix result = Cmat * Pmat;

			Ctr->WorldMatrixImportSRT(result);



			Ctr->Position.print();
			Ctr->Rotate.print();
			Ctr->Scale.print();
			printf("----------------\n");

		}

		for (int i = 0; i < ParentNodeData->GetChildCount(); i++)
		{
			DataStack.push(ParentNodeData->GetChild(i));
		}

		if (DataStack.empty())
			break;


		ParentNodeData = DataStack.top();
		DataStack.pop();
	}*/

	/*Test*//*Test*//*Test*//*Test*//*Test*//*Test*//*Test*/

	for (unsigned int i = 0; i < m_MeshData->m_skeleton.size(); ++i)
	{
		Matrix::Matrix m;
		Transform* tr = m_MeshData->m_skeleton[i]->m_node->m_obj->GetCompponent<Transform>();
		m_SkeletonMatrix.push_back(m);
	}
}

void FbxClass::SetSkeletonWorldMatrix()
{
	int index = 0;
	//스켈레톤 포지션 설정
	for (unsigned int i = 0; i < m_MeshData->m_skeleton.size(); ++i)
	{
		Transform* tr = m_MeshData->m_skeleton[i]->m_node->m_obj->GetCompponent<Transform>();
		m_SkeletonMatrix[i] = tr->GetWorldMatrix();
	}
}

bool FbxClass::Render(ID3D11DeviceContext * deviceContext, DirectionalLight * light)
{
	SetSkeletonWorldMatrix();
	for (unsigned int i = 0; i < m_meshSkinning.size(); ++i)
	{
		if (!m_meshSkinning[i]->Render(deviceContext, light, m_SkeletonMatrix))
		{
			printf("FbxClass::Render Error\n");
		}
	}

	return true;
}

void FbxClass::GetFilePath(const char * filePath)
{
	char path[255];
	ZeroMemory(path, sizeof(path));
	ZeroMemory(m_file_Path, FILENAME_MAX);
	strcpy_s(path, filePath);

	for (int i = strlen(path) - 1; i >= 0; --i)
	{
		if (path[i] == '\\')
		{
			memcpy(m_file_Path, path, i+1);
			break;
		}

		if (path[i] == '/')
		{
			memcpy(m_file_Path, path, i+1);
			break;
		}
	}
}

const char * FbxClass::GetTextureFilePath(const char * filePath)
{
	char path[255];
	char filename[255];
	ZeroMemory(path, sizeof(path));
	ZeroMemory(filename, sizeof(filename));
	strcpy_s(path, filePath);

	for (int i = strlen(path) - 1; i >= 0; --i)
	{
		if (path[i] == '\\')
		{
			memcpy(filename, &path[i + 1], strlen(path));
			break;
		}

		if (path[i] == '/')
		{
			memcpy(filename, &path[i + 1], strlen(path));
			break;
		}
	}

	ZeroMemory(path, sizeof(path));
	strcpy_s(path, m_file_Path);

	strcat_s(path, filename);
	std::string str = path;
	return str.c_str();
}
