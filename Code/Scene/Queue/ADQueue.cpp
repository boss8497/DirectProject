#include "pch.h"
#include "ADQueue.h"
#include "GameObject.h"

ADQueue::ADQueue()
{
}

ADQueue::~ADQueue()
{
}

bool ADQueue::Inite()
{
	return true;
}

void ADQueue::Destroy()
{
}

void ADQueue::NewObject(GameObjectManager * in_Manager, ID3D11Device* device, HWND hwnd, ViewWindow::ViewWindow * vWindow,
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	/*���� ����*/
	//BaseObject Class�� static Queue�� ����
	//New�����ڷ� Object�����ÿ� static Queue�� ������Ʈ ����
	//�װ� �� �����Ӹ��� üũ �ؼ� ������ ��ü������ �߰�����
	if (!BaseObject::GetNewQueue()->empty())
	{
		std::swap(this->m_AddQ, *BaseObject::GetNewQueue());
		while (!m_AddQ.empty())
		{
			BaseObject* CurrentObj = m_AddQ.front();
			m_AddQ.pop();
			
			if (!in_Manager->AddGameObject(device, hwnd, CurrentObj))
			{
			}


			if (!tWindow->AddItem(CurrentObj))
			{

			}
			

			
		}

		//clear Queue
		std::queue<BaseObject*> tmpQ;
		std::swap(this->m_AddQ, tmpQ);
	}
}

void ADQueue::NewUIObject(UIManager * in_Manager, ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, ViewWindow::ViewWindow * vWindow,
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	/*���� ����*/
	//BaseObject Class�� static Queue�� ����
	//New�����ڷ� Object�����ÿ� static Queue�� ������Ʈ ����
	//�װ� �� �����Ӹ��� üũ �ؼ� ������ ��ü������ �߰�����
	if (!BaseObject::GetNewUIQueue()->empty())
	{
		std::swap(this->m_AddQ, *BaseObject::GetNewUIQueue());
		while (!m_AddQ.empty())
		{
			BaseObject* CurrentObj = m_AddQ.front();
			m_AddQ.pop();

			if (!in_Manager->AddGameObject(device, deviceContext, hwnd, screensize, CurrentObj))
			{

			}

			if (!tWindow->AddItem(CurrentObj))
			{

			}
		}

		//clear Queue
		std::queue<BaseObject*> tmpQ;
		std::swap(this->m_AddQ, tmpQ);
	}
}

void ADQueue::DestroyObject(GameObjectManager* in_Manager, UIManager* in_uiManager, ViewWindow::ViewWindow * vWindow,
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	if (!BaseObject::GetDestroyQueue()->empty())
	{
		std::swap(this->m_DestroyQ, *BaseObject::GetDestroyQueue());
		while (!m_DestroyQ.empty())
		{
			BaseObject* CurrentObj = m_DestroyQ.front();
			m_DestroyQ.pop();

			in_Manager->ListEraseObject(CurrentObj);
			in_uiManager->ListEraseObject(CurrentObj);
			tWindow->DeleteItem(CurrentObj);
			delete CurrentObj;
		}
	}

	//clear Queue
	std::queue<BaseObject*> tmpQ;
	std::swap(this->m_DestroyQ, tmpQ);
}
