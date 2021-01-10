#pragma once
#include"ItemWindow.h"
#include"CubeObject.h"
#include"UI.h"
#include"Input.h"

#define IDC_TREEVIEW 0

enum POPUP_MENU
{
	None,
	INSPECTOR,
	CREATE_OBJECT,
	DELETE_OBJECT,
	OBJECT_EMPTY,
	OBJECT_CUBE,
	OBJECT_ROTATE_SPHERE,
	OBJECT2D_EMPTY,
	OBJECT2D_IMG,
	OBJECT2D_TEXT
};

/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/*       클래스 흐름															  */
/*		 MessageHandler() =>  WM_NOTIFY(윈도우 메시지) => TreeViewEvent() =>	  */
/*		(RCLICK 일때) PopUp() =>  Popup Event								  */
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

namespace TreeWindow
{
	class TreeViewWinodw : public BaseWindow
	{
	public:
		TreeViewWinodw();
		~TreeViewWinodw();
		TreeViewWinodw(const TreeViewWinodw& copy) = delete;

	public:
		virtual bool Inite();
		virtual void Destroy();
		virtual void Update();
		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
		

		bool AddItem(BaseObject* obj);
		void DeleteItem(BaseObject* obj);
		void SetItemWindow(ItemWindow::ItemWindow* itemWindow);
	private:
		void CreateTreeView();
		void ResizeChildWindow();
		void PopUp();
		void TreeViewEvent(LPARAM lparam);
		HTREEITEM GetSeleItem();
		HTREEITEM GetSelectItem(TVHITTESTINFO* hitInfo);
		/*return IDC*/
		int ChildAddWarringMSG(HTREEITEM item);


		void BeginDrag(NMTREEVIEW* tv);
		void MoveDragEvent(LPARAM lparam);
		bool EndDragEvent();

		/*팝업메뉴*/
	private:
		void PopMenu_INSPECTOR();
		void PopMenu_DELETE_OBJECT();
		void PopMenu_OBJECT_EMPTY();
		void PopMenu_OBJECT_CUBE();
		void PopMenu_2DOBJECT_EMPTY();
		void PopMenu_2DOBJECT_IMG();
		void PopMenu_OBJECT2D_TEXT();
		void PopMenu_ROTATE_SPHERE();

	public:
		HTREEITEM DestroyItem = nullptr;
		HTREEITEM RootItem;
		HWND m_Thwnd;
		HWND m_Phwnd;
	private:
		/*포인터만 받아온다 할당해제x*/
		ItemWindow::ItemWindow* m_ItemWindow;
		//test
		HTREEITEM draggeditem;
		HIMAGELIST draggedImageList; 
		HIMAGELIST imageList;
		bool DragFlag = false;
	};

	static TreeViewWinodw* TreeWindowHandle = nullptr;
	static LRESULT CALLBACK TreeWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
		case WM_CREATE:
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return TreeWindowHandle->MessageHandler(hwnd, umsg, wparam, lparam);
		}

		return 0;
	}
}