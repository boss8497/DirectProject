#pragma once
#include"GameObjectManager.h"
#include"UIManager.h"
#include"BaseWindow.h"
#include"Inspecter.h"
#include"Camera.h"

/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/*  �帧��														*/
/*  TreeView Popup Menu���� Info Ŭ����							*/
/*  Inspecter Class�� CreateInspecter()�Լ��� ȣ���� �ν����͸� ����	*/
/*  IDC�� ������ ITEM�� ������ �����ϰ� Command�޽��� �߻���			*/
/*  Inspecter Class�� InspecterEvent()�Լ��� ȣ����				*/
/*  IDC�� ���� ������� �̺�Ʈ�� �߻� ��Ų��						*/
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

enum IDC_LIST
{
	SCROLLBAR = 4501,
	/*�ʱ� ������ Enable, Tranform ������Ʈ*/
	BASE_INFO,
	BASE_INFO_GROUPBOX,
	BASE_INFO_ENABLE,
	MESH,
	MATERIAL_IDC,
	FBX
};

namespace ItemWindow
{
	class ItemWindow : public BaseWindow
	{
	public:
		ItemWindow();
		~ItemWindow();
		ItemWindow(const ItemWindow& copy) = delete;

	public:
		virtual bool Inite();
		virtual void Destroy();
		virtual void Update();
	public:
		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		void CreateObjectInfo(_In_ HTREEITEM in_obj);
		BaseObject* FindObject(HTREEITEM in_obj);
		BaseObject* FindObject2D(HTREEITEM in_obj);
		void InspacterClear();
	private:
		void CreateScroll();
		void DestroyScrollBar();
		void ScrollBarEvent(WPARAM wparam, LPARAM lparam);

	private:
		HWND m_ScrollBar = nullptr;
		RECT mSB_SIZE;
		POINT mSB_RANGE;

		Inspecter* m_inspecter = nullptr;

	public:
		//�����͸� ������ ����
		GameObjectManager* m_PtrGM = nullptr;
		UIManager* m_PtrUM = nullptr;
		BaseObject* m_CurrentObj = nullptr;
		Camera* m_MainCamera = nullptr;
	};

	static ItemWindow* ItemWindowHandle = nullptr;

	static LRESULT CALLBACK ItemWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
		{
			printf("KEYDOWN ItemWindow\n");
			switch (wparam)
			{
			case VK_SPACE:
				printf("space\n");
				break;

			case VK_RETURN:
				printf("VK_RETURN\n");
				break;

			default:
				break;
			}

			return 0;
		}



		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return ItemWindowHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}

		return 0;
	}
}