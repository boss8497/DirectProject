#pragma once
#include"BaseObject.h"
#include"Texture.h"
#include"Mesh.h"
#include"Shader.h"
#include"DirectionalLight.h"
#include"Transform.h"
#include"FbxUtilities.h"
#include"UpdateQueue.h"


class Material : public Component<BaseObject>
{
public:
	Material();
	~Material();

public:
	/*파일 지정 하기 위해서는 초기화 이전에 파일경로를 설정 해줘야 된다*/
	bool Inite(ID3D11Device* device, HWND hwnd);
	bool FbxInite(ID3D11Device* device, HWND hwnd, std::vector<SkeletonNode*>& skeleton,
		std::vector<FbxVertexType>& _vertex,
		Indices& _indices);
	virtual void Destroy();
	void Update(UpdateQueue* uQueue);

	bool Render(ID3D11DeviceContext * deviceContext, Transform* tr, DirectionalLight* light);
	bool Render_WireFrame(ID3D11DeviceContext * deviceContext, Transform* tr, DirectionalLight* light);
	bool FbxRender(ID3D11DeviceContext * deviceContext, DirectionalLight* light, std::vector<Matrix::Matrix>& matrix);

	void SetMeshFilePath(const char* name);
	void SetTextureFilePath(const char* name);
	void SetShaderFilePath(const char* vs_name, const char* ps_name);

	bool FbxVertexCutting(ID3D11Device * device, HWND hwnd, std::vector<SkeletonNode*>& skeleton, 
		std::vector<FbxVertexType>& _vertex,
		Indices& _indices);

	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
public:
	bool m_ChangeFlag = false; 
	bool m_WireFrame = false;
	Shader::TextureBufferType m_TextureBuffer;

	MESH_TYPE m_MeshType = MESH_RECT_TYPE;

	char m_Mesh_FilePath[FILENAME_MAX];
	char m_Textrue_FilePath[FILENAME_MAX];
	char m_PS_FilePath[FILENAME_MAX];
	char m_VS_FilePath[FILENAME_MAX];

	Mesh*		m_mesh = nullptr;
	Texture*	m_texture = nullptr;
	Shader*		m_shader = nullptr;


	std::vector<Mesh*> m_MeshList;
	std::vector<SkeletonObjPtr> m_SkeletonPtr;
	/*Fbx*/
};