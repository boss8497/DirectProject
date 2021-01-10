#pragma once
#include"Component.h"
#include"FbxUtilities.h"
#include"FbxMeshSkinning.h"


class FbxClass : public Component<BaseObject>
{
public:
	FbxClass();
	~FbxClass();
	FbxClass(const FbxClass& copy) = delete;

public:
	bool Inite(BaseObject* parent, const char* filePath);
	void Update(UpdateQueue* uQueue);
	void Destroy();
	bool Skinning_Inite(ID3D11Device * device, HWND hwnd);


	bool LoadScene(BaseObject* parent, const char* filePath);
	NodeData* GetHierarchy(BaseObject* parent);

	/*순회하면서 데이터 만든다*/
	void MeshDataSet();


	void ProcessJointsAndAnimations(NodeData* node);
	void ProcessNode(NodeData* node);
	void ProcessMesh(NodeData* node);
	void ProcessMaterial(NodeData* node);
	void AssociateMaterialToMesh(NodeData* node);

	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, Vector::Vector2& outUV);


	void SetSkeletonTransform();
	void SetSkeletonWorldMatrix();
public:
	bool Render(ID3D11DeviceContext * deviceContext, DirectionalLight* light);

	
private:
	void GetFilePath(const char* filePath);
	const char* GetTextureFilePath(const char* filePath);


	void PrintFbxVector4(FbxVector4 v)
	{
		printf("[ %lf.6 , %lf.6 ,%lf.6 ,%lf.6 ]\n",v.mData[0], v.mData[1],
			v.mData[2],v.mData[3]);
	}
	void PrintFbxMatrix(FbxAMatrix& m)
	{
		for (int i = 0; i < 4; ++i)
		{
			printf("[");
			for (int j = 0; j < 4; ++j)
			{
				printf("\t%lf.6", m.mData[i][j]);
				if (j < 3)
				{
					printf(",");
				}
			}
			printf("]\n");
		}
	}
private:
	FbxManager * m_manager = nullptr;
	FbxScene* m_scene = nullptr;
	NodeData* m_root;


	char m_file_Path[FILENAME_MAX];
private:
	std::vector<FbxMeshSkinning*> m_meshSkinning;
	std::vector<Matrix::Matrix> m_SkeletonMatrix;

	/*생성되면서 바로 Skeleton Joint만듬*/
	FbxMeshData* m_MeshData = nullptr;
};