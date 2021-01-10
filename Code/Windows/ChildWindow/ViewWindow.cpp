#include "pch.h"
#include "ViewWindow.h"

ViewWindow::ViewWindow::ViewWindow()
{
}

ViewWindow::ViewWindow::~ViewWindow()
{
}

bool ViewWindow::ViewWindow::Inite()
{
	ViewWindowHandle = this;

	return true;
}

void ViewWindow::ViewWindow::Destroy()
{
}

void ViewWindow::ViewWindow::Upddate()
{
}

LRESULT ViewWindow::ViewWindow::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
	{
		printf("KEYDOWN ViewWindow\n");
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

	case WM_KEYUP:
	{
		return 0;
	}

	case WM_MOUSEMOVE:
		this->MouseMoveEvent();
		break;

	case WM_MOUSEHOVER:
		this->MouseEnterEvent();
		break;

	case WM_MOUSELEAVE:
		this->MouseLeaveEvent();
		break;


	case WM_DEVICECHANGE:
		return 0;

	case WM_SIZE:
	{
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}//end default

	}//end switch

	return 0;
}

bool ViewWindow::ViewWindow::IsMouseEnter()
{
	return this->MouseEnter;
}

void ViewWindow::ViewWindow::MouseEnterEvent()
{
	this->MouseEnter = true;
	TRACKMOUSEEVENT tMouse;
	tMouse.cbSize = sizeof(TRACKMOUSEEVENT);
	tMouse.dwFlags = TME_LEAVE;
	tMouse.hwndTrack = this->m_hwnd;
	tMouse.dwHoverTime = 10;
	TrackMouseEvent(&tMouse);
}

void ViewWindow::ViewWindow::MouseMoveEvent()
{
	if (!this->MouseEnter && !this->MouseEnterEventFlag)
	{
		TRACKMOUSEEVENT tMouse;
		tMouse.cbSize = sizeof(TRACKMOUSEEVENT);
		tMouse.dwFlags = TME_HOVER | TME_LEAVE;
		tMouse.hwndTrack = this->m_hwnd;
		tMouse.dwHoverTime = 10;
		if (TrackMouseEvent(&tMouse))
		{
			MouseEnterEventFlag = true;
		}
	}
}

void ViewWindow::ViewWindow::MouseLeaveEvent()
{
	this->MouseEnter = false;
	this->MouseEnterEventFlag = false;
}
