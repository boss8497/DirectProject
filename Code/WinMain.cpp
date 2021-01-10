#include"pch.h"
#include"Application.h"
#include"Transform.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	Windows::Application* window = new Windows::Application;

	if (window->Inite())
	{
		window->Run();
	}


	window->Destroy();
	delete window;
	window = nullptr;

	return 0;
}