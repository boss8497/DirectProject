#include "pch.h"
#include "Render.h"
#include"SkyBoxClass.h"

Graphics::Render::Render()
{
}

Graphics::Render::~Render()
{
}

void Graphics::Render::SetBackColor(Vector::Vector4 & color)
{
	m_d3d->SetBackColor(color);
}

bool Graphics::Render::Inite(Vector::Vector2 screenSize, HWND hwnd, bool fullscreen, bool vsync, ItemWindow::ItemWindow* iWindow)
{
	m_d3d = new D3D::DirectX3D();
	if (!m_d3d->Inite(screenSize, hwnd, fullscreen, vsync))
	{
		return false;
	}

	scene = new Scene();
	if (!scene->Inite(m_d3d->GetDevice(), m_d3d->GetDeviceContext(), hwnd, iWindow))
	{
		printf("Scene Dynamic Create Error\n");
		if (scene)
		{
			scene->Destroy();
			delete scene;
		}

		return false;
	}
	m_hwnd = hwnd;
	Vector::Vector4 color(1, 0, 0, 1);
	this->SetBackColor(color);

	return true;
}

void Graphics::Render::Destroy()
{
	if (scene)
	{
		scene->Destroy();
		delete scene;
		scene = nullptr;
	}

	if (m_d3d)
	{
		m_d3d->Destroy();
		delete m_d3d;
		m_d3d = nullptr;
	}

}

void Graphics::Render::Update(ViewWindow::ViewWindow * vWindow, 
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	scene->Update(m_d3d->GetDevice(), m_d3d->GetDeviceContext(), m_hwnd,
		vWindow, tWindow, iWindow);
}

bool Graphics::Render::Rendering()
{
	m_d3d->BeginScene();



	m_d3d->TurnOnNoCulling();
	m_d3d->TurnZBufferOff();
	scene->SkyboxRender(m_d3d->GetDeviceContext());
	m_d3d->TurnZBufferOn();
	m_d3d->TurnOffNoCulling();

	scene->GridTerrainRender(m_d3d->GetDeviceContext());


	scene->Render(m_d3d->GetDeviceContext());




	m_d3d->TurnOnWireFrame();
	scene->RenderWire(m_d3d->GetDeviceContext());
	m_d3d->TurnOffWireFrame();


	m_d3d->TurnZBufferOff();


	scene->UI_Render(m_d3d->GetDeviceContext(), m_d3d);

	m_d3d->TurnZBufferOn();




	m_d3d->EndScene();

	return true;
}
