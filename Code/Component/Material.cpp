#include "pch.h"
#include "Material.h"

Material::Material()
{
	ZeroMemory(m_Mesh_FilePath, FILE_NAME_MAX);
	strcpy_s(m_Mesh_FilePath, FILE_NAME_MAX, Default_Mesh);
	ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
	strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, Default_Texture);
	ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
	strcpy_s(m_VS_FilePath, FILE_NAME_MAX, Default_VS);
	ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
	strcpy_s(m_PS_FilePath, FILE_NAME_MAX, Default_PS);


	m_mesh = nullptr;
	m_texture = nullptr;
	m_shader = nullptr;


	m_TextureBuffer.color.Set(1, 1, 1, 1);
	this->m_CID = COMPONENT_ID::MATERIAL;
}

Material::~Material()
{
}

bool Material::Inite(ID3D11Device * device, HWND hwnd)
{
	//메쉬 초기화
	if (m_mesh == nullptr)
	{
		m_mesh = new Mesh();

		/*if (!m_mesh->Inite(device, m_Mesh_FilePath))
		{
			printf("Mesh Inite Error");
			return false;
		}*/
		switch (this->m_MeshType)
		{
		case MESH_RECT_TYPE:
			if (!m_mesh->Inite(device, m_Mesh_FilePath))
			{
				printf("Mesh Inite Error");
				return false;
			}
			break;
		case MESH_SPHERE_TYPE:
			if (!m_mesh->CreateSphere(device, 1.0f, 20, 20))
			{
				printf("Mesh Inite Error");
				return false;
			}
			break;
		default:
			break;
		}
	}



	//텍스쳐 초기화
	if (m_texture == nullptr)
	{
		m_texture = new Texture();
		if (!m_texture->Inite(device, m_Textrue_FilePath))
		{
			printf("Textrue Inite Error");
			return false;
		}
	}


	//쉐이더 초기화
	if (m_shader == nullptr)
	{
		m_shader = new Shader();
		if (!m_shader->Inite(device, hwnd, m_VS_FilePath, m_PS_FilePath))
		{
			printf("Shader Inite Error");
			return false;
		}
	}

	return true;
}

bool Material::FbxInite(ID3D11Device * device, HWND hwnd, std::vector<SkeletonNode*>& skeleton, std::vector<FbxVertexType>& _vertex,
	Indices& _indices)
{
	//텍스쳐 초기화
	m_texture = new Texture();
	if (!m_texture->Inite(device, m_Textrue_FilePath))
	{
		printf("Material : FbxInite - Textrue Inite Error\n");
		return false;
	}

	m_shader = new Shader();
	if (!m_shader->IniteFbx(device, hwnd, m_VS_FilePath, nullptr))
	{
		printf("Material : FbxInite - IniteFbx Error\n");
		return false;
	}

	if (!FbxVertexCutting(device, hwnd, skeleton, _vertex, _indices))
	{
		printf("Material : FbxInite - FbxVertexCutting Error\n");
		return false;
	}

	return true;
}

void Material::Destroy()
{
	if (m_texture)
	{
		m_texture->Destroy();
		m_texture = nullptr;
	}

	if (m_shader)
	{
		m_shader->Destroy();
		m_shader = nullptr;
	}

	if (m_mesh)
	{
		m_mesh->Destroy();
		m_mesh = nullptr;
	}
}

void Material::Update(UpdateQueue* uQueue)
{
	if (this->m_ChangeFlag)
	{
		if (this->m_texture != nullptr)
		{
			/*1. 먼저 어떤 파일명이 바뀌었는지 찾는다*/
			/*파일명이 변경이 됬다면 다시 초기화 한다*/
			if (std::strcmp(this->m_texture->GetFilePath(), m_Textrue_FilePath) != 0)
			{
				m_texture->SetFileName(m_Textrue_FilePath);
				uQueue->Push(m_texture);
			}


			this->m_ChangeFlag = false;
		}
	}
}

bool Material::Render(ID3D11DeviceContext * deviceContext, Transform * tr, DirectionalLight * light)
{
	if (!m_mesh->Render(deviceContext))
	{
		printf("Material Mesh Render Error\n");
		return false;
	}
	if (!m_shader->SetShaderParameters(deviceContext, tr,
		m_texture->GetTexture(), light, &m_TextureBuffer))
	{
		printf("Material Shader SetShaderParameters Error\n");
		return false;
	}

	m_shader->RenderShader(deviceContext, m_mesh->GetIndexCount());
	return true;
}

bool Material::Render_WireFrame(ID3D11DeviceContext * deviceContext, Transform * tr, DirectionalLight * light)
{
	if (this == nullptr)
		return false;

	Shader::TextureBufferType color;
	color.color.Set(0, 255, 0, 1);
	if (!m_mesh->Render(deviceContext))
	{
		printf("Material Mesh Render Error\n");
		return false;
	}
	if (!m_shader->SetShaderParameters(deviceContext, tr,
		m_texture->GetTexture(), light, &color))
	{
		printf("Material Shader SetShaderParameters Error\n");
		return false;
	}

	m_shader->RenderShader(deviceContext, m_mesh->GetIndexCount());
	return true;
}

bool Material::FbxRender(ID3D11DeviceContext * deviceContext, DirectionalLight * light, std::vector<Matrix::Matrix>& matrix)
{
	for (unsigned int i = 0; i < m_MeshList.size(); ++i)
	{
		if (!m_MeshList[i]->FbxRender(deviceContext))
		{
			printf("Material Mesh FbxRender Error\n");
			return false;
		}

		if (!m_shader->FbxSetShaderParameters(deviceContext,
			m_texture->GetTexture(), light, m_SkeletonPtr[i], matrix, &m_TextureBuffer))
		{
			printf("Material Shader SetShaderParameters Error\n");
			return false;
		}

		m_shader->RenderShader(deviceContext, m_MeshList[i]->GetIndexCount());
	}
	return true;
}

void Material::SetMeshFilePath(const char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_Mesh_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Mesh_FilePath, FILE_NAME_MAX, Default_Mesh);
	}
	else
	{
		ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_VS_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}

void Material::SetTextureFilePath(const char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, Default_Texture);
	}
	else
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}

void Material::SetShaderFilePath(const char * vs_name, const char * ps_name)
{
	if (vs_name == nullptr)
	{
		ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_VS_FilePath, FILE_NAME_MAX, Default_VS);
	}
	else
	{
		ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_VS_FilePath, FILE_NAME_MAX, vs_name);
	}

	if (ps_name == nullptr)
	{
		ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_PS_FilePath, FILE_NAME_MAX, Default_PS);
	}
	else
	{
		ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_PS_FilePath, FILE_NAME_MAX, ps_name);
	}
	this->m_ChangeFlag = true;
}

bool Material::FbxVertexCutting(ID3D11Device * device, HWND hwnd, std::vector<SkeletonNode*>& skeleton, std::vector<FbxVertexType>& _vertex, Indices & _indices)
{
	int m_vertexCount = _vertex.size();
	FbxVertexTypeVS* vertices = new FbxVertexTypeVS[m_vertexCount];

	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = _vertex[i].m_Vertice.position;
		vertices[i].texture = _vertex[i].m_Vertice.texture;
		vertices[i].normal = _vertex[i].m_Vertice.normal;

		vertices[i].weight.x = static_cast<float>(_vertex[i].m_BlendingWeight[0]);
		vertices[i].weight.y = static_cast<float>(_vertex[i].m_BlendingWeight[1]);
		vertices[i].weight.z = static_cast<float>(_vertex[i].m_BlendingWeight[2]);
		vertices[i].weight.w = static_cast<float>(_vertex[i].m_BlendingWeight[3]);


		vertices[i].index[0] = _vertex[i].m_Skeleton_index[0];
		vertices[i].index[1] = _vertex[i].m_Skeleton_index[1];
		vertices[i].index[2] = _vertex[i].m_Skeleton_index[2];
		vertices[i].index[3] = _vertex[i].m_Skeleton_index[3];
	}


	FbxVertexTypeVS* CuttingVertices = nullptr;
	Indices* CuttingIndices = nullptr;
	int CuttingVertexCount = 0;
	std::vector<int> SkeletonIndex;
	int PolygonCount = (int)_indices.m_indices.size() / 3;
	bool SkeletonEqualFlag = false;

	int currPolygonIndex = 0;
	int startPolygonIndex = 0;

	while (startPolygonIndex < PolygonCount)
	{
		for (int i = startPolygonIndex; i < PolygonCount; ++i)
		{

			for (int j = 0; j < 3; ++j)
			{
				if (SkeletonIndex.size() == 0)
				{
					if (vertices[_indices.m_indices[i * 3 + j]].index[0] != -1)
						SkeletonIndex.push_back(vertices[_indices.m_indices[i * 3 + j]].index[0]);

					for (int k = 1; k < 4; ++k)
					{
						for (unsigned int h = 0; h < SkeletonIndex.size(); ++h)
						{
							if (SkeletonIndex[h] == vertices[_indices.m_indices[i * 3 + j]].index[k])
							{
								SkeletonEqualFlag = true;
								break;
							}
						}

						if (!SkeletonEqualFlag)
						{
							if (vertices[_indices.m_indices[i * 3 + j]].index[k] != -1)
								SkeletonIndex.push_back(vertices[_indices.m_indices[i * 3 + j]].index[k]);
						}
						SkeletonEqualFlag = false;
					}
				}
				else
				{
					for (int k = 0; k < 4; ++k)
					{
						for (unsigned int h = 0; h < SkeletonIndex.size(); ++h)
						{
							if (SkeletonIndex[h] == vertices[_indices.m_indices[i * 3 + j]].index[k])
							{
								SkeletonEqualFlag = true;
								break;
							}
						}

						if (!SkeletonEqualFlag)
						{
							if (vertices[_indices.m_indices[i * 3 + j]].index[k] != -1)
								SkeletonIndex.push_back(vertices[_indices.m_indices[i * 3 + j]].index[k]);
						}
						SkeletonEqualFlag = false;
					}
				}

			}

			currPolygonIndex = i + 1;
			//다음 최대로 추가될 인덱스 초과 확인
			int SkelIndexSize = (int)SkeletonIndex.size() + 3;
			if (SkelIndexSize >= BoneMatrixCount)
			{
				break;
			}

		}

		int TriangleCount = 0;
		int CuttingVertexSize = (currPolygonIndex - startPolygonIndex) * 3;
		CuttingVertices = new FbxVertexTypeVS[CuttingVertexSize];
		CuttingIndices = new Indices();

		for (int i = startPolygonIndex; i < currPolygonIndex; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				CuttingVertices[TriangleCount * 3 + j] = vertices[_indices.m_indices[i * 3 + j]];

				//find index & set new Cutting index

				for (int k = 0; k < 4; ++k)
				{
					for (unsigned int h = 0; h < SkeletonIndex.size(); ++h)
					{
						if (SkeletonIndex[h] == CuttingVertices[TriangleCount * 3 + j].index[k])
						{
							CuttingVertices[TriangleCount * 3 + j].index[k] = h;
							break;
						}
					}
				}
				unsigned long data = TriangleCount * 3 + j;
				CuttingIndices->m_indices.push_back(data);

			}
			++TriangleCount;
		}

		SkeletonObjPtr CuttingSkeletonPtr;
		int SkeletonIndexSize = static_cast<int>(SkeletonIndex.size());
		for (int i = 0; i < SkeletonIndexSize; ++i)
		{
			BaseObject* tmp = skeleton[SkeletonIndex[i]]->m_node->m_obj;
			CuttingSkeletonPtr.m_objList.push_back(tmp);
		}
		m_SkeletonPtr.push_back(CuttingSkeletonPtr);


		Mesh* tmp_mesh = new Mesh();

		/*Inite Mesh*/
		if (!tmp_mesh->FbxInite(device, CuttingVertices, CuttingVertexSize, CuttingIndices))
		{
			return false;
		}
		m_MeshList.push_back(tmp_mesh);
		tmp_mesh = nullptr;


		/*free*/
		delete[] CuttingVertices;
		CuttingVertices = nullptr;

		CuttingIndices->m_indices.clear();
		delete CuttingIndices;
		CuttingIndices = nullptr;

		SkeletonIndex.clear();

		/*while Out Seed*/
		startPolygonIndex = currPolygonIndex;
	}
	return true;
}

InspacterItem * Material::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT & NextPos, float compFrameXsize)
{
	InspacterItem* MaterialFrame = nullptr;
	POINT FrameSize = { (LONG)compFrameXsize, 150 };
	POINT nPos = NextPos;


	//프레임 생성
	MaterialFrame = new InspacterItem();
	MaterialFrame->CreateComponentFrame(phwnd, phins, nPos, FrameSize, "Material");





	POINT FilePathPos = nPos;
	POINT FilePathSize = { 0, };
	InspacterItem* FilePathItem = nullptr;

	FilePathPos.x += INFO_PADDING;
	FilePathPos.y += INFO_PADDING * 2;

	FilePathSize.x = (LONG)compFrameXsize - INFO_PADDING * 2;
	FilePathSize.y = INFO_PADDING * 2;

	FilePathItem = new InspacterItem();
	FilePathItem->CreateInspecterEditBox(phwnd, phins, FilePathPos, FilePathSize
		, IDC_ITEM::EDITTMP, 0, "Path", this->m_Textrue_FilePath, true);

	MaterialFrame->m_SubItemList.push_back(FilePathItem);



	//다음 위치
	FilePathPos.y += FilePathSize.y;

	POINT BtnPos = FilePathPos;
	POINT BtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* Btn = nullptr;
	UpdateItem * BtnUpdate = nullptr;

	Btn = new InspacterItem();
	Btn->CreateInspecterButton(phwnd, phins, BtnPos, BtnSize, IDC_ITEM::BUTTONETMP1, COMPONENT_ID::MATERIAL, "Find Texture File");
	MaterialFrame->m_SubItemList.push_back(Btn);


	//업데이트 이벤트 등록
	BtnUpdate = new UpdateItem();
	BtnUpdate->Inite(Btn->m_hwnd, (void*)FilePathItem->m_hwnd,
		UpdateType::UPDATE_BUTTON_OPEN_FILEPATH_DIALOG, UpdateValueType::HWND_WINDOWGETTEXT_YTYPE,
		IDC_ITEM::BUTTONETMP1);

	upList->push_back(BtnUpdate);

	//다음 위치
	BtnPos.y += BtnSize.y;



	POINT ColorPos = BtnPos;
	POINT ColorSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* ColorItem = nullptr;
	UpdateItem* ColorUpdate = nullptr;


	ColorItem = new InspacterItem();
	ColorItem->CreateInspecterButton(phwnd, phins, ColorPos, ColorSize, IDC_ITEM::BUTTONETMP2,
		0, "Select Texture Color");
	MaterialFrame->m_SubItemList.push_back(ColorItem);

	ColorUpdate = new UpdateItem();
	ColorUpdate->Inite(ColorItem->m_hwnd, (void*)&this->m_TextureBuffer.color, UpdateType::UPDATE_BUTTON_OPEN_COLOR_DIALOG,
		UpdateValueType::VECTER3_YTYPE, IDC_ITEM::BUTTONETMP2);
	upList->push_back(ColorUpdate);


	return MaterialFrame;
}
