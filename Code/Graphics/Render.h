#pragma once
#include"DirectX3D.h"
#include"Vector.h"
#include"GameObject.h"
#include"Scene.h"
#include"Screen.h"
#include"ViewWindow.h"
#include"ItemWindow.h"
#include"TreeViewWindow.h"


namespace Graphics
{
	class Render
	{
	public:
		Render();
		Render(const Render& copy) = delete;
		~Render();


		void SetBackColor(Vector::Vector4& color);

	public:
		bool Inite(Vector::Vector2 screenSize, HWND hwnd, bool fullcreen,bool vsync, ItemWindow::ItemWindow* iWindow);
		void Destroy();
		void Update(ViewWindow::ViewWindow* vWindow, TreeWindow::TreeViewWinodw* tWindow, ItemWindow::ItemWindow* iWindow);


		bool Rendering();
	private:
		D3D::DirectX3D* m_d3d;
		HWND m_hwnd;
		Scene* scene;
	};
}