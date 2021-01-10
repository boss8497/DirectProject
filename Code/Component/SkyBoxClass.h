#pragma once
#include"pch.h"
#include"Component.h"
#include"Mesh.h"
#include"NoLightShader.h"
#include"Texture.h"
#include"SkyBoxMesh.h"
#include"NoLightShader.h"


class SkyBoxClass : public Component<BaseObject>
{
public:
	SkyBoxClass();
	~SkyBoxClass();
	SkyBoxClass(const SkyBoxClass& copy) = delete;

public:
	bool Inite(ID3D11Device* device, HWND hwnd);
	void Update(UpdateQueue* uQueue);
	void Destroy();

	bool Render(ID3D11DeviceContext * deviceContext, Transform* tr, DirectionalLight* light);


	void SetTextureFilePath(const char* name);
	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
private:
	bool m_ChangeFlag = false;
	char m_Textrue_FilePath[FILENAME_MAX];

	SkyBoxMesh * m_SMesh = nullptr;
	NoLightShader* m_NShader = nullptr;

	Texture* m_Texture = nullptr;
};