#pragma once
#include"BaseWindow.h"
#include"WindowInfo.h"

namespace ViewWindow
{

	class ViewWindow : public BaseWindow
	{
	public:
		ViewWindow();
		~ViewWindow();
		ViewWindow(const ViewWindow& copy) = delete;

	public:
		virtual bool Inite();
		virtual void Destroy();
		virtual void Upddate();

		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	public:
		bool IsMouseEnter();

	private:
		void MouseEnterEvent();
		void MouseMoveEvent();
		void MouseLeaveEvent();

	public:
		bool LButton = false;
		bool RButton = false;
		bool MButton = false;

	private:
		bool MouseEnter = false;
		bool MouseEnterEventFlag = false;
	};


	static ViewWindow* ViewWindowHandle = nullptr;
	static LRESULT CALLBACK ViewWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
		case WM_CREATE:
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
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


		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return ViewWindowHandle->MessageHandler(hwnd,umsg,wparam,lparam);
		}

		return 0;
	}

}