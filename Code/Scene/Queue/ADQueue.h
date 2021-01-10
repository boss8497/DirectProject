#pragma once
#include"pch.h"
#include"GameObjectManager.h"
#include"UIManager.h"
#include"ViewWindow.h"
#include"TreeViewWindow.h"
#include"ItemWindow.h"


class ADQueue
{
public:
	ADQueue();
	~ADQueue();
	ADQueue(const ADQueue& copy) = delete;



public:
	bool Inite();
	void Destroy();

	/*생성된 3D와 2D를 나눈 이유*/
	//오브젝트가 생성되고 필수로 초기화 해줘야되는 Component들이 틀려서
	//구분해주기 위해 나누었음
	//삭제는 구분없이 다 가능하기 때문 구분하지 않았음
	void NewObject(GameObjectManager* in_Manager, ID3D11Device* device, HWND hwnd, ViewWindow::ViewWindow * vWindow,
		TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow);
	void NewUIObject(UIManager* in_Manager, ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, ViewWindow::ViewWindow * vWindow,
		TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow);

	void DestroyObject(GameObjectManager* in_Manager, UIManager* in_uiManager, ViewWindow::ViewWindow * vWindow,
		TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow);
private:
	std::queue<BaseObject*> m_DestroyQ;
	std::queue<BaseObject*> m_AddQ;
};