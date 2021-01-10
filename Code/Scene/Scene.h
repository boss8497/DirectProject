#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"GameObjectManager.h"
#include"Screen.h"
#include"Camera.h"
#include"GameObject.h"
#include"UIManager.h"
#include"UI.h"
#include"Text.h"
#include"Input.h"
#include"AudioComponent.h"
#include"FbxClass.h"
#include"UpdateQueue.h"
#include"ADQueue.h"
#include"TimeClass.h"
#include"ViewWindow.h"
#include"ItemWindow.h"
#include"TreeViewWindow.h"

class Scene
{
public:
	Scene();
	~Scene();
	Scene(const Scene& s) = delete;



public:
	bool Inite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, ItemWindow::ItemWindow* iWindow);
	void Destroy(); 
	void Update(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd,
		ViewWindow::ViewWindow* vWindow, TreeWindow::TreeViewWinodw* tWindow, 
		ItemWindow::ItemWindow* iWindow);


	bool Inite_StaticMember();
	void Destroy_StaticMember();

	virtual bool GamobjectLoad();
	virtual bool UILoad();
	
	void ObjectUpdate(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, ViewWindow::ViewWindow * vWindow,
		TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow);

	void Gameobject_Update(ID3D11Device * device);
	void UI_Update(ID3D11Device * device);

	bool Render(ID3D11DeviceContext * deviceContext);
	bool RenderWire(ID3D11DeviceContext * deviceContext);
	bool UI_Render(ID3D11DeviceContext * deviceContext, D3D::DirectX3D* d3d);

	bool MainCameraUpdate();

	bool CheckDestroyItemWindw(TreeWindow::TreeViewWinodw * tWindow);
	//고쳐야될것
	void SkyboxRender(ID3D11DeviceContext * deviceContext);
	void GridTerrainRender(ID3D11DeviceContext * deviceContext);
private:
	Camera * Main_Camera = nullptr;
	DirectionalLight* DirectionLight = nullptr;
	GameObject* SkyBoxObject = nullptr;
	GameObject* GridTerrainObject = nullptr;

	UIManager* m_ui_manager = nullptr;
	GameObjectManager * m_gm_manager = nullptr;

	Text* countText;

	RenderQueue* m_RenderQueue = nullptr;
	UpdateQueue* m_UpdateQueue = nullptr;
	ADQueue* m_ADQueue = nullptr;
public:
	FbxClass* fbx = nullptr;

	BaseObject* testObj;
	Texture2D* textTexture2d;
	Material* MatTest;
	BaseObject* testDestroy;
};