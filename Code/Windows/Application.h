#pragma once
#include"pch.h"
#include"Render.h"
#include"Screen.h"
#include"Input.h"
#include<Dbt.h>
#include"AudioComponent.h"
#include"TimeClass.h"
#include"BaseWindow.h"
#include"ViewWindow.h"
#include"TreeViewWindow.h"
#include"ItemWindow.h"
#include"WindowSizeControl.h"



namespace Windows
{
	class Application
	{
	public:
		Application();
		Application(const Application& copy) = delete;
		~Application();


		bool Inite();
		void Destroy();
		void Run();


		void WindowMoveCenter();
		void ReSizeWindow();


		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
	private:
		bool Frame();
		void IniteWindows();
		void DestroyWindows();


	private:
		LPCWSTR				m_applicationName;
		HINSTANCE			m_hinstance;
		HWND				m_hwnd;
		ViewWindow::ViewWindow*		m_ViewWindow = nullptr;
		TreeWindow::TreeViewWinodw* m_TreeViewWindow = nullptr;
		ItemWindow::ItemWindow*		m_ItemWindow = nullptr;
		WindowSizeControl*			m_WSC = nullptr;

		Vector::Vector2 m_WindowSize;
		Vector::Vector2 m_WindowPosition;
		

		Graphics::Render*			m_render = nullptr;
		HDEVNOTIFY					hNewAudio = nullptr;
		std::vector<BaseWindow*>	m_ChildWindow_V;



	};


	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static Application* WindowsHandle = 0;
}