#include "pch.h"
#include "BaseWindow.h"

BaseWindow::BaseWindow()
{
}

BaseWindow::~BaseWindow()
{
}

bool BaseWindow::Inite()
{
	return true;
}

void BaseWindow::Destroy()
{
}

void BaseWindow::Update()
{
}

void BaseWindow::SetWindowMove(Vector::Vector2 pos, Vector::Vector2 size, Vector::Vector4 padding)
{
	if (m_hwnd)
	{
		this->m_Position = pos;
		this->m_WindowSize = size;
		this->m_Padding = padding;
		MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
			static_cast<int>(m_Position.y + m_Padding.y),
			static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
			static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
	}
}

void BaseWindow::SetWindowMove(_In_ POINT& pos, _In_ POINT& size, Vector::Vector4 padding)
{
	if (m_hwnd)
	{
		this->m_Position.Set(&pos);
		this->m_WindowSize.Set(&size);
		this->m_Padding = padding;
		MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
			static_cast<int>(m_Position.y + m_Padding.y),
			static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
			static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
	}
}

void BaseWindow::SetWindowMove()
{
	MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
		static_cast<int>(m_Position.y + m_Padding.y),
		static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
		static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
}

void BaseWindow::SetWindowPosition(Vector::Vector2 pos)
{
	if (m_hwnd)
	{
		this->m_Position = pos;
		MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
			static_cast<int>(m_Position.y + m_Padding.y),
			static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
			static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
	}
}

void BaseWindow::SetWindowSize(Vector::Vector2 size)
{
	if (m_hwnd)
	{
		this->m_WindowSize = size;
		MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
			static_cast<int>(m_Position.y + m_Padding.y),
			static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
			static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
	}
}

void BaseWindow::SetWindowPadding(Vector::Vector4 padding)
{
	if (m_hwnd)
	{
		this->m_Padding = padding;
		MoveWindow(m_hwnd, static_cast<int>(m_Position.x + m_Padding.x),
			static_cast<int>(m_Position.y + m_Padding.y),
			static_cast<int>(m_WindowSize.x - m_Padding.x - m_Padding.z),
			static_cast<int>(m_WindowSize.y - m_Padding.y - m_Padding.w), TRUE);
	}
}

HWND BaseWindow::CreateGroupBox(HWND hwnd, HINSTANCE hins, const char * name, int idc, RECT & size)
{
	HWND childhwnd = CreateWindow(TEXT("button"), MethodHelper::GetWC(name),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		size.left,size.top,
		size.right, size.bottom,
		hwnd, (HMENU)idc, hins, NULL);

	return childhwnd;
}

HWND BaseWindow::CreatePushButton(HWND hwnd, HINSTANCE hins, const char * name, int idc, RECT & size, _In_ bool group)
{
	HWND childhwnd = CreateWindow(TEXT("button"), MethodHelper::GetWC(name), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		size.left, size.top,
		size.right, size.bottom, 
		hwnd, (HMENU)idc, hins, NULL);

	return childhwnd;
}

HWND BaseWindow::CreateEditBox(HWND hwnd, HINSTANCE hins, const char * name, int idc, RECT & size, bool readonly)
{
	HWND childhwnd;
	if (readonly)
	{
		childhwnd = CreateWindow(TEXT("edit"), MethodHelper::GetWC(name), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
			size.left, size.top,
			size.right, size.bottom,
			hwnd, (HMENU)idc, hins, NULL);
	}
	else
	{
		childhwnd = CreateWindow(TEXT("edit"), MethodHelper::GetWC(name), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			size.left, size.top,
			size.right, size.bottom,
			hwnd, (HMENU)idc, hins, NULL);
	}


	/*
	case WM_COMMAND
	switch(HIWORD(wparam)
	{
	case EN_CHANGE:
	GetWindowText(HWND, str,size)
	}
	*/

	return childhwnd;
}

HWND BaseWindow::CreateCheckBox(HWND hwnd, HINSTANCE hins, const char * name, int idc, RECT & size, bool group, bool check)
{
	HWND childhwnd = CreateWindow(TEXT("button"), MethodHelper::GetWC(name), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		size.left, size.top,
		size.right, size.bottom,
		hwnd, (HMENU)idc, hins, NULL);
	
	if(check)
		SendMessage(childhwnd, BM_SETCHECK, BST_CHECKED, 0);
	else
		SendMessage(childhwnd, BM_SETCHECK, BST_UNCHECKED, 0);

	return childhwnd;
}
