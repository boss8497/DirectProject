#include "pch.h"
#include "Application.h"

Windows::Application::Application()
{
}

Windows::Application::~Application()
{
}

bool Windows::Application::Inite()
{
	//윈도우 api 초기화
	IniteWindows();


	//렌더링 클래스 초기화
	m_render = new Graphics::Render();
	if (!m_render->Inite(Screen::Instance().GetSize(), m_ViewWindow->m_hwnd, false, true, m_ItemWindow))
	{
		return false;
	}

	Input::Instance().Inite(m_hinstance, m_hwnd);

	/*새로운 장치 사운드를 체크하기 위한것*/
	DEV_BROADCAST_DEVICEINTERFACE filter = {};
	filter.dbcc_size = sizeof(filter);
	filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	filter.dbcc_classguid = KSCATEGORY_AUDIO;


	/*USB장치 인식을 위해 추가*/
	hNewAudio = RegisterDeviceNotification(m_hwnd, &filter, DEVICE_NOTIFY_WINDOW_HANDLE);
	

	/*테스트 중*/


	return true;
}

void Windows::Application::Destroy()
{
	if (m_WSC)
	{
		m_WSC->Destroy();
		delete m_WSC;
		m_WSC = nullptr;
	}

	if (m_ViewWindow)
	{
		m_ViewWindow->Destroy();
		delete m_ViewWindow;
		m_ViewWindow = nullptr;
	}

	if (m_TreeViewWindow)
	{
		m_TreeViewWindow->Destroy();
		delete m_TreeViewWindow;
		m_TreeViewWindow = nullptr;
	}

	if (m_ItemWindow)
	{
		m_ItemWindow->Destroy();
		delete m_ItemWindow;
		m_ItemWindow = nullptr;
	}

	if (m_render)
	{
		m_render->Destroy();
		delete m_render;
		m_render = nullptr;
	}
	DestroyWindows();
}

void Windows::Application::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			//msg루프 탈출



			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!Frame())
				break;

		}

		//ESC입력시 msg루프 탈출
		if (Input::Instance().IsEcapPressed() == true)
		{
			break;
		}

	}
}

void Windows::Application::WindowMoveCenter()
{
	Vector::Vector2 size = Screen::Instance().GetSize();
	Vector::Vector2 pos;

	pos.x = (GetSystemMetrics(SM_CXSCREEN) - size.x) / 2.0f;
	pos.y = (GetSystemMetrics(SM_CYSCREEN) - size.y) / 2.0f;
	MoveWindow(m_hwnd, static_cast<int>(pos.x), static_cast<int>(pos.y),
		static_cast<int>(size.x), static_cast<int>(size.y), TRUE);

	Screen::Instance().SetPosition(pos);
}

void Windows::Application::ReSizeWindow()
{

}

LRESULT Windows::Application::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_CREATE:
		break;

	case WM_KEYDOWN:
	{
		switch (wparam)
		{
		case VK_SPACE:
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


	case WM_DEVICECHANGE:
		printf("WM_DEVICECHANGE\n");
		if (wparam == DBT_DEVICEARRIVAL)
		{
			auto pDev = reinterpret_cast<PDEV_BROADCAST_HDR>(lparam);
			if (pDev)
			{
				if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					auto pInter = reinterpret_cast<const PDEV_BROADCAST_DEVICEINTERFACE>(pDev);
					if (pInter->dbcc_classguid == KSCATEGORY_AUDIO)
					{
						AudioSystem::Instance()->OnNewAudioDevice(true);
					}
				}
			}
		}
		return 0;

	case WM_SIZE:
	{
		if(m_WSC)
			m_WSC->SetWindowScailing();
		//WindowMoveCenter();
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (m_WSC)
			m_WSC->MouseUpdate(m_hinstance, MouseState::NONE);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		if (m_WSC)
			m_WSC->MouseUpdate(m_hinstance, MouseState::LB_DOWN);
		return 0;
	}

	case WM_LBUTTONUP:
	{
		if (m_WSC)
			m_WSC->MouseUpdate(m_hinstance, MouseState::LB_UP);
		return 0;
	}

	case WM_MOUSELEAVE:
	{
		if (m_WSC)
			m_WSC->MouseLeaveEvent();
		return 0;
	}

	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}//end default

	}//end switch

	return 0;
}

bool Windows::Application::Frame()
{
	/*Deleta time 계산해서 관리 해줭야됨*/
	TimeClass::Instance().Update();

	GetCursorPos(&Input::Instance().Mouse_Position);
	ScreenToClient(m_hwnd, &Input::Instance().Mouse_Position);

	if (!Input::Instance().Update())
	{
		return false;
	}

	m_render->Update(m_ViewWindow, m_TreeViewWindow, m_ItemWindow);
	return m_render->Rendering();
}

void Windows::Application::IniteWindows()
{
	WindowsHandle = this;

	m_hinstance = GetModuleHandle(NULL);
	m_applicationName = L"DirectProject";



	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = m_applicationName;
	wcex.hIconSm = wcex.hIcon;


	RegisterClassEx(&wcex);


	/*wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpfnWndProc = WndProc2;
	wcex.lpszClassName = L"Child";
	RegisterClassEx(&wcex);*/

	/*윈도우로부터 화면 크기 받아오기*/
	//Screen::Instance().Size.x = static_cast<float>(GetSystemMetrics(SM_CXSCREEN));
	//Screen::Instance().Size.y = static_cast<float>(GetSystemMetrics(SM_CYSCREEN));

	
	/*윈도우 생성시 lpszClassName child라 안지으면 생성이 안됨..*/
	m_ViewWindow = new ViewWindow::ViewWindow();
	ViewWindow::ViewWindowHandle = m_ViewWindow;
	m_ViewWindow->Inite();
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpfnWndProc = ViewWindow::ViewWndProc;
	wcex.lpszClassName = L"Child";
	RegisterClassEx(&wcex);
	m_ChildWindow_V.push_back(m_ViewWindow);

	m_TreeViewWindow = new TreeWindow::TreeViewWinodw();
	TreeWindow::TreeWindowHandle = m_TreeViewWindow;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpfnWndProc = TreeWindow::TreeWndProc;
	wcex.lpszClassName = L"Child2";
	RegisterClassEx(&wcex);
	m_ChildWindow_V.push_back(m_TreeViewWindow);
	m_TreeViewWindow->m_Phwnd = this->m_hwnd;

	m_ItemWindow = new ItemWindow::ItemWindow();
	ItemWindow::ItemWindowHandle = m_ItemWindow;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpfnWndProc = ItemWindow::ItemWndProc;
	wcex.lpszClassName = L"Child3";
	RegisterClassEx(&wcex);
	m_ChildWindow_V.push_back(m_ItemWindow);


	m_TreeViewWindow->SetItemWindow(m_ItemWindow);

	/*전체창모드*/

	/*DEVMODE dmScreenSettings;
	memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	dmScreenSettings.dmPelsWidth = (unsigned long)m_application_Size.x;
	dmScreenSettings.dmPelsHeight = (unsigned long)m_application_Size.y;
	dmScreenSettings.dmBitsPerPel = 32;
	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);*/

	m_WindowSize = Screen::Instance().GetSize();
	int CaptionY;
	CaptionY = GetSystemMetrics(SM_CYCAPTION);
	RECT rt = { 0, 0,static_cast<LONG>(m_WindowSize.x),static_cast<LONG>(m_WindowSize.y) };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, 0);

	m_WindowPosition.x = (GetSystemMetrics(SM_CXSCREEN) - m_WindowSize.x) / 2;
	m_WindowPosition.y = (GetSystemMetrics(SM_CYSCREEN) - m_WindowSize.y) / 2;

	/*WS_OVERLAPPEDWINDOW : 가장 기본적인 윈도우 옵션*/
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW , m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_SYSMENU,
		static_cast<int>(m_WindowPosition.x), static_cast<int>(m_WindowPosition.y),
		rt.right - rt.left , rt.bottom - rt.top,
		NULL, NULL, m_hinstance, NULL);

	Screen::Instance().SetPosition(m_WindowPosition);




	m_ViewWindow->m_hwnd = CreateWindow(L"Child", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, 0, m_hinstance, NULL);


	m_TreeViewWindow->m_hwnd = CreateWindow(L"Child2", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, 0, m_hinstance, NULL);

	m_ItemWindow->m_hwnd = CreateWindow(L"Child3", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		0, 0, 100, 100, m_hwnd, 0, m_hinstance, NULL);


	/*윈도우 사이즈컨트롤 초기화*/
	m_WSC = new WindowSizeControl();
	
	if (!m_WSC->Inite(m_hwnd, &m_ChildWindow_V))
	{

	}


	/*사이즈를 설정후 자식 윈도우 초기화*/
	m_TreeViewWindow->Inite();
	m_ItemWindow->Inite();


	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void Windows::Application::DestroyWindows()
{
	DestroyWindow(m_hwnd);
	m_hwnd = nullptr;


	UnregisterClass(m_applicationName, m_hinstance);

	WindowsHandle = nullptr;


	if (hNewAudio)
		UnregisterDeviceNotification(hNewAudio);

	CoUninitialize();
}

static LRESULT CALLBACK Windows::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		printf("KEYDOWN Windows\n");
		switch (wparam)
		{
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		case VK_UP:
			break;
		case VK_DOWN:
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return WindowsHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}

	return 0;
}