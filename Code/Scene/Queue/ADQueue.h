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

	/*������ 3D�� 2D�� ���� ����*/
	//������Ʈ�� �����ǰ� �ʼ��� �ʱ�ȭ ����ߵǴ� Component���� Ʋ����
	//�������ֱ� ���� ��������
	//������ ���о��� �� �����ϱ� ���� �������� �ʾ���
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