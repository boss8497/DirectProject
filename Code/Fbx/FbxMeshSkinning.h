#pragma once
#include"FbxUtilities.h"
#include"Material.h"

class FbxMeshSkinning : public Component<BaseObject>
{
public:
	FbxMeshSkinning() = delete;
	FbxMeshSkinning(FbxMeshData* data);
	~FbxMeshSkinning();
	FbxMeshSkinning(const FbxMeshSkinning& copy) = default;

	void FbxMeshData_Import(FbxMeshData* data);
public:
	bool Inite(ID3D11Device* device, HWND hwnd, std::vector<SkeletonNode*>& skeleton);
	void Update(UpdateQueue* uQueue);
	void Destroy();



	bool Render(ID3D11DeviceContext * deviceContext,
		DirectionalLight * light, std::vector<Matrix::Matrix>& matrix);

public:
	bool CreateBuffer(ID3D11Device * device);

private:
	//m_Skeleton : 포인터만 받아옴 메모리해제 x
	std::vector<SkeletonNode*>* m_Skeleton;
	std::vector<FbxVertexType> m_Vertex;
	std::vector<Material*> m_material;
	std::vector<Indices> m_indices;
	std::vector<std::string> m_Texture_Path;
	int m_vertexCount = 0;
	int m_indexCount = 0;
};