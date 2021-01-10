#include "pch.h"
#include "ItemWindow.h"

ItemWindow::ItemWindow::ItemWindow()
{
}

ItemWindow::ItemWindow::~ItemWindow()
{
}

bool ItemWindow::ItemWindow::Inite()
{
	if (m_inspecter == nullptr)
	{
		m_inspecter = new Inspecter();
	}
	
	return true;
}

void ItemWindow::ItemWindow::Destroy()
{
}

void ItemWindow::ItemWindow::Update()
{
	if (m_inspecter)
		m_inspecter->Update();
}

void ItemWindow::ItemWindow::CreateScroll()
{
	if (m_ScrollBar != nullptr)
		this->DestroyScrollBar();

	m_ScrollBar = CreateWindow(L"scrollbar", 
		NULL , WS_CHILD | WS_VISIBLE | SBS_VERT,
		this->mSB_SIZE.top, this->mSB_SIZE.left, 
		this->mSB_SIZE.right, this->mSB_SIZE.bottom, 
		this->m_hwnd, (HMENU)IDC_LIST::SCROLLBAR, this->m_hinstance, NULL);
	SetScrollRange(m_ScrollBar, SB_CTL, this->mSB_RANGE.x, this->mSB_RANGE.y, TRUE);
	SetScrollPos(m_ScrollBar, SB_CTL, 0, TRUE);
}

void ItemWindow::ItemWindow::DestroyScrollBar()
{
	
	if (m_ScrollBar)
	{
		DestroyWindow(m_ScrollBar);
		m_ScrollBar = nullptr;
	}
}

void ItemWindow::ItemWindow::ScrollBarEvent(WPARAM wparam, LPARAM lparam)
{
	if ((HWND)lparam == m_ScrollBar)
	{
		int VarPos =
			GetScrollPos(m_ScrollBar, SB_CTL);
		printf(" VarPos VarPos : %d\n", VarPos);
		switch (LOWORD(wparam))
		{
		case SB_LINEUP:
			VarPos = max(0, VarPos - 1);
			printf(" SB_LINEUP VarPos : %d\n", VarPos);
			break;

		case SB_LINEDOWN:
			VarPos = min(255, VarPos + 1);
			printf(" SB_LINEDOWN VarPos : %d\n", VarPos);
			break;

		case SB_PAGEUP:
			VarPos = max(0, VarPos - 10);
			break;

		case SB_PAGEDOWN:
			VarPos = min(255, VarPos + 10);
			break;

		case SB_THUMBTRACK:
			VarPos = HIWORD(wparam);
			break;
		}
		SetScrollPos(m_ScrollBar, SB_CTL, VarPos, TRUE);
		InvalidateRect(this->m_hwnd, NULL, FALSE);
	}
	
}

LRESULT ItemWindow::ItemWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_COMMAND:
		m_inspecter->InspecterEvent(hwnd, wparam, lparam);
		break;

	case WM_VSCROLL:
		this->ScrollBarEvent(wparam, lparam);
		break;

	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}//end default

	}//end switch

	return 0;
}

void ItemWindow::ItemWindow::CreateObjectInfo(HTREEITEM in_obj)
{
	this->m_CurrentObj = this->m_PtrGM->FindObject(in_obj);

	if (this->m_CurrentObj == nullptr)
	{
		this->m_CurrentObj = this->m_PtrUM->FindObject(in_obj);
	}


	if (this->m_CurrentObj == nullptr)
	{
		printf("Error : Not Find Select Obejct : ItemWindow::ItemWindow::CreateObjectInfo()\n");
	}
	else
		m_inspecter->CreateInspecter(this->m_hwnd, this->m_hinstance, m_CurrentObj);
}

BaseObject * ItemWindow::ItemWindow::FindObject(HTREEITEM in_obj)
{
	return this->m_PtrGM->FindObject(in_obj);
}

BaseObject * ItemWindow::ItemWindow::FindObject2D(HTREEITEM in_obj)
{
	return this->m_PtrUM->FindObject(in_obj);
}

void ItemWindow::ItemWindow::InspacterClear()
{
	this->m_inspecter->InspecterClear();
}
