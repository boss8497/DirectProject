#include "pch.h"
#include "FbxMeshSkinning.h"
FbxMeshSkinning::FbxMeshSkinning(FbxMeshData* data)
{
	m_Skeleton = &(data->m_skeleton);
	this->FbxMeshData_Import(data);
}
FbxMeshSkinning::~FbxMeshSkinning()
{
}

void FbxMeshSkinning::FbxMeshData_Import(FbxMeshData * data)
{
	//메터리얼 생성
	for (int i = 0; i < data->m_MaterialCount; ++i)
	{
		Material* mat = new Material();
		Indices indice;

		mat->SetShaderFilePath(Default_FBX_VS, nullptr);
		if (data->m_Texture_FilePath.size() > 0)
			mat->SetTextureFilePath(data->m_Texture_FilePath[i].c_str());
		else
			mat->SetTextureFilePath(nullptr);

		m_material.push_back(mat);
		m_indices.push_back(indice);
	}

	//버텍스 데이터 가져오기
	m_Vertex.assign(data->m_Vertex.begin(), data->m_Vertex.end());

	//인덱스 배열 설정(머터리얼이 다수 일때)
	for (int i = 0; i < data->m_TriangleCount; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_indices[data->m_Triangle[i].mMaterialIndex].m_indices.push_back(data->m_Triangle[i].mIndices[j]);
		}
	}
}

bool FbxMeshSkinning::Inite(ID3D11Device * device, HWND hwnd, std::vector<SkeletonNode*>& skeleton)
{
	for (unsigned int i = 0; i < m_material.size(); ++i)
	{
		m_material[i]->FbxInite(device, hwnd, skeleton, m_Vertex, m_indices[i]);
	}
	return true;
}

void FbxMeshSkinning::Update(UpdateQueue* uQueue)
{
}

void FbxMeshSkinning::Destroy()
{
	for (auto it = m_material.begin(); it != m_material.end();)
	{
		Material* mat = (*it);
		it = m_material.erase(it);
		mat->Destroy();
		delete mat;
	}
}



bool FbxMeshSkinning::Render(ID3D11DeviceContext * deviceContext, DirectionalLight * light, std::vector<Matrix::Matrix>& matrix)
{
	for (unsigned int i = 0; i < m_material.size(); ++i)
	{
		if (!m_material[i]->FbxRender(deviceContext, light, matrix))
		{
			printf("FbxMeshSkinning : Render Error\n");
			return false;
		}
	}
	return true;
}

bool FbxMeshSkinning::CreateBuffer(ID3D11Device * device)
{

	return true;
}
