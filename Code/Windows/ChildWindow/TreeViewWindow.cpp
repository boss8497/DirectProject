#include "pch.h"
#include "TreeViewWindow.h"
#include "RotateSphere.h"

TreeWindow::TreeViewWinodw::TreeViewWinodw()
{
}

TreeWindow::TreeViewWinodw::~TreeViewWinodw()
{
}

bool TreeWindow::TreeViewWinodw::Inite()
{
	//https://docs.microsoft.com/en-us/windows/win32/controls/create-a-tree-view-control
	TreeWindowHandle = this;
	this->CreateTreeView();
	return true;
}

void TreeWindow::TreeViewWinodw::Destroy()
{
}

void TreeWindow::TreeViewWinodw::Update()
{
}

LRESULT TreeWindow::TreeViewWinodw::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{

	case WM_CREATE:
		break;


	case WM_NOTIFY:

		switch (LOWORD(wparam))
		{
		case IDC_TREEVIEW:
			this->TreeViewEvent(lparam);
			break;
		}
		break;

	case WM_SIZE:
	{
		this->ResizeChildWindow();
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (DragFlag)
			this->MoveDragEvent(lparam);
	}
	break;

	case WM_LBUTTONUP:
	{
		if (DragFlag)
		{
			this->EndDragEvent();
			SendMessage(this->m_Thwnd, TVM_SELECTITEM, (WPARAM)TVGN_DROPHILITE,
				(LPARAM)0);
			this->draggeditem = 0;
			this->draggedImageList = 0;
			this->DragFlag = false;
		}
		break;
	}

	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}//end default

	}//end switch

	return 0;
}

void TreeWindow::TreeViewWinodw::BeginDrag(NMTREEVIEW * tv)
{
	if (tv->hdr.hwndFrom == this->m_Thwnd)
	{
		draggeditem = tv->itemNew.hItem;
		draggedImageList = (HIMAGELIST)SendMessage(this->m_Thwnd,
			TVM_CREATEDRAGIMAGE, (WPARAM)0, (LPARAM)draggeditem);

		ImageList_BeginDrag(draggedImageList, 0, 0, 0);
		ImageList_DragEnter(this->m_Thwnd, tv->ptDrag.x, tv->ptDrag.y);


		SetCapture(GetParent(this->m_Thwnd));
		ShowCursor(false);
		DragFlag = true;
	}
}

void TreeWindow::TreeViewWinodw::MoveDragEvent(LPARAM lparam)
{
	POINT point;
	point.x = LOWORD(lparam);
	point.y = HIWORD(lparam);
	ClientToScreen(this->m_hwnd, &point);
	ScreenToClient(this->m_Thwnd, &point);

	ImageList_DragMove(point.x, point.y);
	ImageList_DragShowNolock(false);


	TVHITTESTINFO hitTestInfo;
	hitTestInfo.pt.x = point.x;
	hitTestInfo.pt.y = point.y;
	HTREEITEM targetitem = (HTREEITEM)SendMessage(this->m_Thwnd, TVM_HITTEST,
		(WPARAM)0, (LPARAM)&hitTestInfo);

	if (targetitem)
	{
		SendMessage(this->m_Thwnd, TVM_SELECTITEM, (WPARAM)TVGN_DROPHILITE,
			(LPARAM)targetitem);
	}

	ImageList_DragShowNolock(true);
}

bool TreeWindow::TreeViewWinodw::EndDragEvent()
{
	HTREEITEM targeItem = (HTREEITEM)SendMessage(this->m_Thwnd, TVM_GETNEXTITEM,
		(WPARAM)TVGN_DROPHILITE, (LPARAM)0);

	ImageList_EndDrag();
	ImageList_Destroy(draggedImageList);
	ReleaseCapture();
	ShowCursor(true);

	BaseObject* dragobj = m_ItemWindow->FindObject(draggeditem);
	BaseObject* target = m_ItemWindow->FindObject(targeItem);



	if (dragobj != nullptr)
	{
		//이미 같은 부모라면
		if (dragobj->GetParent() == target)
			return false;
		//자기자신이라면
		if (dragobj == target)
			return false;
		//자기의 자식이라면
		if (dragobj->IsAllChild(target))
		{
			return false;
		}

		if (target != nullptr)
		{
			dragobj->ReleaseParent();
			target->AddChild(dragobj);

			this->DeleteItem(dragobj);
			this->AddItem(dragobj);
		}
		else
		{
			//루트 오브젝트로 삽입
			dragobj->ReleaseParent();
			this->m_ItemWindow->m_PtrGM->AddRootObject(dragobj);

			this->DeleteItem(dragobj);
			this->AddItem(dragobj);
		}
		this->m_ItemWindow->InspacterClear();
		this->m_ItemWindow->CreateObjectInfo(dragobj->Hierarchy.m_Item);
		return true;
	}



	dragobj = m_ItemWindow->FindObject2D(draggeditem);
	target = m_ItemWindow->FindObject2D(targeItem);

	//이미 같은 부모라면
	if (dragobj->GetParent() == target)
		return false;
	//자기자신이라면
	if (dragobj == target)
		return false;
	//자기의 자식이라면
	if (dragobj->IsAllChild(target))
	{
		return false;
	}


	if (dragobj != nullptr)
	{
		if (target != nullptr)
		{
			dragobj->ReleaseParent();
			target->AddChild(dragobj);

			this->DeleteItem(dragobj);
			this->AddItem(dragobj);
		}
		else
		{
			//루트 오브젝트로 삽입
			dragobj->ReleaseParent();
			this->m_ItemWindow->m_PtrUM->AddRootObject(dragobj);

			this->DeleteItem(dragobj);
			this->AddItem(dragobj);
		}
		this->m_ItemWindow->InspacterClear();
		this->m_ItemWindow->CreateObjectInfo(dragobj->Hierarchy.m_Item);
	}


	return true;
}

bool TreeWindow::TreeViewWinodw::AddItem(BaseObject * obj)
{
	std::queue<BaseObject*> depth_queue;
	std::queue<BaseObject*> child_queue;
	int depth = 1;

	if (obj->Hierarchy.m_Item != nullptr)
	{
		return false;
	}

	//depth 체크
	BaseObject* depthParent = obj->GetParent();
	while (depthParent != nullptr)
	{
		++depth;
		depthParent = depthParent->GetParent();
	}


	depth_queue.push(obj);

	while (!depth_queue.empty())
	{
		BaseObject* current = nullptr;
		BaseObject* currentParent = nullptr;
		int depth_queue_size = depth_queue.size();

		for (int i = 0; i < depth_queue_size; ++i)
		{
			current = depth_queue.front();
			depth_queue.pop();

			currentParent = current->GetParent();

			

			TVITEM tvi;
			TVINSERTSTRUCT tvins;

			//tvi 초기화
			tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.pszText = MethodHelper::GetWC(current->Name.c_str());
			tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);
			tvi.lParam = (LPARAM)depth;



			tvins.item = tvi;
			tvins.hInsertAfter = TVI_LAST;
			tvins.item.iImage = 2;
			tvins.item.iSelectedImage = 0;
			if (currentParent == nullptr)
				tvins.hParent = RootItem;
			else
				tvins.hParent = currentParent->Hierarchy.m_Item;

			current->Hierarchy.m_Item = (HTREEITEM)SendMessage(this->m_Thwnd, TVM_INSERTITEM, 0,
				(LPARAM)(LPTVINSERTSTRUCT)&tvins);
			current->Hierarchy.m_Thwnd = this->m_Thwnd;

			for (int j = 0; j < current->GetChildCount(); ++j)
			{
				child_queue.push(current->GetChild(j));
			}
		}

		std::swap(depth_queue, child_queue);
		++depth;
	}

	InvalidateRect(this->m_hwnd, NULL, TRUE);
	return true;
}

void TreeWindow::TreeViewWinodw::DeleteItem(BaseObject * obj)
{
	TreeView_DeleteItem(this->m_Thwnd, obj->Hierarchy.m_Item);
	obj->Hierarchy.m_Item = nullptr;
}

void TreeWindow::TreeViewWinodw::SetItemWindow(ItemWindow::ItemWindow * itemWindow)
{
	this->m_ItemWindow = itemWindow;
}

void TreeWindow::TreeViewWinodw::CreateTreeView()
{
	imageList = ImageList_Create(16, 16, ILC_COLORDDB | ILC_MASK, 3, 4);
	HICON icon;
	icon = (HICON)::LoadImage(GetModuleHandle(0),
		MAKEINTRESOURCE(IDI_ICON3),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	ImageList_AddIcon(imageList, icon);

	icon = (HICON)::LoadImage(GetModuleHandle(0),
		MAKEINTRESOURCE(IDI_ICON4),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	ImageList_AddIcon(imageList, icon);

	icon = (HICON)::LoadImage(GetModuleHandle(0),
		MAKEINTRESOURCE(IDI_ICON2),
		IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	ImageList_AddIcon(imageList, icon);



	this->m_Thwnd = CreateWindow(
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		0, 0,
		static_cast<int>(this->m_WindowSize.x),
		static_cast<int>(this->m_WindowSize.y),
		this->m_hwnd,
		(HMENU)IDC_TREEVIEW,
		this->m_hinstance,
		NULL);



	SendMessage(this->m_Thwnd, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)imageList);

	/*초기화가 다 끝나고 나야지 체크박스 체크가 가능해짐*/
	//LONG stlye = GetWindowLong(this->m_Thwnd, GWL_STYLE) | WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_CHECKBOXES;
	//SetWindowLong(this->m_Thwnd, GWL_STYLE, stlye);



	wchar_t* ParentName = L"Hirarcy";

	TVINSERTSTRUCT insertStruct;
	insertStruct.hParent = TVI_ROOT;
	insertStruct.hInsertAfter = TVI_LAST;        // handle to item or TVI_FIRST, TVI_LAST, TVI_ROOT
	insertStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	//insertStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	insertStruct.item.pszText = ParentName;
	insertStruct.item.cchTextMax = sizeof(ParentName) / sizeof(ParentName[0]);
	insertStruct.item.iImage = 1;                       // image index of ImageList
	insertStruct.item.iSelectedImage = 1;

	RootItem = (HTREEITEM)SendMessage(this->m_Thwnd, TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
}

void TreeWindow::TreeViewWinodw::ResizeChildWindow()
{
	MoveWindow(this->m_Thwnd, 0, 0,
		static_cast<int>(this->m_WindowSize.x),
		static_cast<int>(this->m_WindowSize.y),
		TRUE);
}

void TreeWindow::TreeViewWinodw::PopUp()
{
	POINT point;
	GetCursorPos(&point);


	HMENU menu = CreatePopupMenu();
	HMENU menu2 = CreatePopupMenu();
	AppendMenu(menu, NULL, (int)POPUP_MENU::INSPECTOR, L"Info");
	AppendMenu(menu, NULL, (int)POPUP_MENU::DELETE_OBJECT, L"Delete");
	AppendMenu(menu, MF_POPUP, (UINT_PTR)menu2, L"Create");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT_EMPTY, L"3D GameBoject(Empty)");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT_CUBE, L"3D Cube");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT_ROTATE_SPHERE, L"3D Rotate_Sphere");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT2D_EMPTY, L"2D GameBoject(Empty)");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT2D_IMG, L"2D Image");
	AppendMenu(menu2, NULL, (int)POPUP_MENU::OBJECT2D_TEXT, L"2D Text");


	POPUP_MENU command = (POPUP_MENU)TrackPopupMenu(menu, TPM_RETURNCMD, point.x, point.y, 0, this->m_Thwnd, 0);

	switch (command)
	{
	case POPUP_MENU::INSPECTOR:
		this->PopMenu_INSPECTOR();
		break;

	case POPUP_MENU::DELETE_OBJECT:
		this->PopMenu_DELETE_OBJECT();
		break;

	case POPUP_MENU::CREATE_OBJECT:
		break;

	case POPUP_MENU::OBJECT_ROTATE_SPHERE:
		this->PopMenu_ROTATE_SPHERE();
		break;


	case POPUP_MENU::OBJECT_EMPTY:
		this->PopMenu_OBJECT_EMPTY();
		break;

	case POPUP_MENU::OBJECT_CUBE:
		this->PopMenu_OBJECT_CUBE();
		break;

	case POPUP_MENU::OBJECT2D_EMPTY:
		this->PopMenu_2DOBJECT_EMPTY();
		break;

	case POPUP_MENU::OBJECT2D_IMG:
		this->PopMenu_2DOBJECT_IMG();
		break;

	case POPUP_MENU::OBJECT2D_TEXT:
		this->PopMenu_OBJECT2D_TEXT();
		break;
	}


	DestroyMenu(menu);



	/*TVHITTESTINFO hitTestinfo;
	hitTestinfo.pt.x = point.x;
	hitTestinfo.pt.y = point.y;
	TreeView_HitTest(this->m_Thwnd, &hitTestinfo);*/


}

void TreeWindow::TreeViewWinodw::TreeViewEvent(LPARAM lparam)
{

	switch (((LPNMHDR)lparam)->code)
	{
	case NM_CLICK:
	{

	}
	break;

	case TVN_BEGINDRAG:
		this->BeginDrag((NMTREEVIEW*)lparam);
		break;

	case NM_RCLICK:
		this->PopUp();
		break;

	case NM_DBLCLK:
	{
		HTREEITEM Selected = this->GetSeleItem();
		BaseObject* pObject = this->m_ItemWindow->FindObject(Selected);


		if (pObject != nullptr)
		{
			this->m_ItemWindow->m_MainCamera->LookAt(pObject);
			this->m_ItemWindow->m_PtrGM->m_TargetObject = pObject;
			m_ItemWindow->CreateObjectInfo(Selected);
		}
		else
		{
			pObject = this->m_ItemWindow->FindObject2D(Selected);
			if (pObject != nullptr)
			{
				m_ItemWindow->CreateObjectInfo(Selected);
			}
		}
	}
	break;


	case TVN_BEGINLABELEDIT:
		break;

	default:
		break;
	}

}

HTREEITEM TreeWindow::TreeViewWinodw::GetSeleItem()
{
	HTREEITEM Selected = NULL;

	Selected = (HTREEITEM)SendMessage(this->m_Thwnd,
		TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);

	SendMessage(this->m_Thwnd, TVM_SELECTITEM, 0,
		(LPARAM)Selected);
	return Selected;
}

HTREEITEM TreeWindow::TreeViewWinodw::GetSelectItem(TVHITTESTINFO * hitInfo)
{
	return (HTREEITEM)SendMessage(this->m_Thwnd,
		TVM_HITTEST, 0, (LPARAM)hitInfo);;
}

int TreeWindow::TreeViewWinodw::ChildAddWarringMSG(HTREEITEM item)
{
	TVITEM tvi;
	WCHAR objectName[255];

	memset(&tvi, 0, sizeof(tvi));
	memset(objectName, 0, 256);
	tvi.mask = TVIF_TEXT;
	tvi.pszText = objectName;
	tvi.cchTextMax = 256;
	tvi.hItem = item;

	SendMessage(this->m_Thwnd, TVM_GETITEM, 0,
		(LPARAM)&tvi);

	WCHAR warring[255] = L"Add child of ";
	WCHAR warring2[255] = L"?\n(Yes add child, No Root)";
	wcscat_s(warring, wcslen(objectName) + wcslen(warring) + 1, objectName);
	wcscat_s(warring, wcslen(warring2) + wcslen(warring) + 1, warring2);

	return MessageBox(this->m_hwnd, warring, L"Add child of Object?", MB_YESNO | MB_ICONQUESTION);
}

void TreeWindow::TreeViewWinodw::PopMenu_INSPECTOR()
{
	HTREEITEM Selected = this->GetSeleItem();
	m_ItemWindow->CreateObjectInfo(Selected);
	this->m_ItemWindow->m_PtrGM->m_TargetObject = this->m_ItemWindow->m_PtrGM->FindObject(Selected);
}

void TreeWindow::TreeViewWinodw::PopMenu_DELETE_OBJECT()
{
	HTREEITEM Selected = NULL;

	Selected = (HTREEITEM)SendMessage(this->m_Thwnd,
		TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);

	this->m_ItemWindow->InspacterClear();
	this->m_ItemWindow->m_PtrGM->m_TargetObject = nullptr;
	this->DestroyItem = Selected;
}

void TreeWindow::TreeViewWinodw::PopMenu_OBJECT_EMPTY()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if (Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}


	switch (idc)
	{
	case IDYES:
	{
		GameObject * gm = new GameObject();
		BaseObject* pObject = this->m_ItemWindow->FindObject(Selected);
		if (pObject != nullptr)
		{
			pObject->AddChild(gm);
		}
	}
	break;
	case IDNO:
	{
		GameObject * gm = new GameObject();
	}
	break;

	default:
	{
		GameObject * gm = new GameObject();
	}
		break;
	}
}

void TreeWindow::TreeViewWinodw::PopMenu_OBJECT_CUBE()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if(Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}

	switch (idc)
	{
	case IDYES:
	{
		CubeObject* co = new CubeObject();
		BaseObject* pObject = this->m_ItemWindow->FindObject(Selected);
		if (pObject != nullptr)
		{
			pObject->AddChild(co);
			m_ItemWindow->CreateObjectInfo(Selected);
		}
	}
	break;
	case IDNO:
	{
		CubeObject* co = new CubeObject();
	}
	break;

	default:
	{
		CubeObject * co = new CubeObject();
	}
	break;
	}
	/*GameObject* gm = new GameObject();
	Material* mat = nullptr;

	gm->SetName("Cube");
	mat = gm->AddCompponent<Material>();
	mat->SetMeshFilePath(nullptr);
	mat->SetTextrueFilePath(nullptr);
	mat->SetShaderFilePath(nullptr, nullptr);*/
}

void TreeWindow::TreeViewWinodw::PopMenu_2DOBJECT_EMPTY()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if (Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}


	switch (idc)
	{
	case IDYES:
	{
		UI* obj = new UI();
		BaseObject* pObject = this->m_ItemWindow->FindObject(Selected);
		if (pObject != nullptr)
		{
			pObject->AddChild(obj);
		}
	}
	break;
	case IDNO:
	{
		UI* obj = new UI();
	}
	break;

	default:
	{
		UI* obj = new UI();
	}
	break;

	}
}

void TreeWindow::TreeViewWinodw::PopMenu_2DOBJECT_IMG()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if (Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}

	switch (idc)
	{
	case IDYES:
	{
		UI* obj = nullptr;
		BaseObject* pObject = this->m_ItemWindow->FindObject2D(Selected);

		if (pObject != nullptr)
		{
			UI* obj = nullptr;
			obj->SetName("Image");
			Texture2D* tx = obj->AddCompponent<Texture2D>();
			obj->GetCompponent<Transform>()->SetSize(100, 100);
			tx->SetTextureFilePath(nullptr);
			tx->SetVSFilePath(nullptr);
			tx->SetPSFilePath(nullptr);
			pObject->AddChild(obj);
		}
		else
		{
			UI* obj = new UI();
			obj->SetName("Image");
			Texture2D* tx = obj->AddCompponent<Texture2D>();
			obj->GetCompponent<Transform>()->SetSize(100, 100);
			tx->SetTextureFilePath(nullptr);
			tx->SetVSFilePath(nullptr);
			tx->SetPSFilePath(nullptr);
		}
	}
	break;
	case IDNO:
	{
		UI* obj = new UI();
		obj->SetName("Image");
		Texture2D* tx = obj->AddCompponent<Texture2D>();
		obj->GetCompponent<Transform>()->SetSize(100, 100);
		tx->SetTextureFilePath(nullptr);
		tx->SetVSFilePath(nullptr);
		tx->SetPSFilePath(nullptr);
	}
	break;

	default:
	{
		UI* obj = new UI();
		obj->SetName("Image");
		Texture2D* tx = obj->AddCompponent<Texture2D>();
		obj->GetCompponent<Transform>()->SetSize(100, 100);
		tx->SetTextureFilePath(nullptr);
		tx->SetVSFilePath(nullptr);
		tx->SetPSFilePath(nullptr);
	}
	break;

	}
}

void TreeWindow::TreeViewWinodw::PopMenu_OBJECT2D_TEXT()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if (Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}

	switch (idc)
	{
	case IDYES:
	{
		UI* obj = nullptr;
		BaseObject* pObject = this->m_ItemWindow->FindObject2D(Selected);

		if (pObject != nullptr)
		{
			UI* obj = new UI();
			obj->SetName("Text");
			Text* tx = obj->AddCompponent<Text>();
			obj->GetCompponent<Transform>()->SetSize(200, 200);
			pObject->AddChild(obj);
		}
		else
		{
			UI* obj = new UI();
			obj->SetName("Text");
			Text* tx = obj->AddCompponent<Text>();
			obj->GetCompponent<Transform>()->SetSize(200, 200);
		}
	}
	break;
	case IDNO:
	{
		UI* obj = new UI();
		obj->SetName("Text");
		Text* tx = obj->AddCompponent<Text>();
		obj->GetCompponent<Transform>()->SetSize(200, 200);
	}
	break;

	default:
	{
		UI* obj = new UI();
		obj->SetName("Text");
		Text* tx = obj->AddCompponent<Text>();
		obj->GetCompponent<Transform>()->SetSize(200, 200);
	}
	break;

	}
}

void TreeWindow::TreeViewWinodw::PopMenu_ROTATE_SPHERE()
{
	HTREEITEM Selected = this->GetSeleItem();
	int idc = 0;
	if (Selected != nullptr)
	{
		idc = this->ChildAddWarringMSG(Selected);
	}

	switch (idc)
	{
	case IDYES:
	{
		RotateShpere* co = new RotateShpere();
		BaseObject* pObject = this->m_ItemWindow->FindObject(Selected);
		if (pObject != nullptr)
		{
			pObject->AddChild(co);
		}
	}
	break;
	case IDNO:
	{
		RotateShpere* co = new RotateShpere();
	}
	break;

	default:
	{
		RotateShpere* co = new RotateShpere();
	}
	break;

	}
}
