#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"Vector.h"
#include"Screen.h"

namespace D3D
{
	class DirectX3D : public AligendAllocationPolicy<16>
	{
	public:
		DirectX3D();
		DirectX3D(const DirectX3D& _copy) = delete;



	public:
		bool Inite(Vector::Vector2 screenSize, HWND hwnd, bool fullscreen, bool vsync);
		void Destroy();


		void BeginScene();
		void EndScene();

		void SetBackColor(Vector::Vector4& color);

	public:
		ID3D11Device * GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		void GetWorldMatrix(XMMATRIX* m);
		void GetVideoCardInfo(char* name, int* memory);
		void TurnZBufferOn();
		void TurnZBufferOff();
		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		void TurnOnWireFrame();
		void TurnOffWireFrame();

		void TurnOnNoCulling();
		void TurnOffNoCulling();

		bool SetBackbufferAndView(Vector::Vector2& screenSize, Vector::Vector2& screenRate, bool resize);
	private:
		bool CreateDevice(HWND hwnd, Vector::Vector2& screenSize, Vector::Vector2* screenRate);
		bool CreateSwapchangeAndDevice(HWND hwnd, Vector::Vector2& screenSize, Vector::Vector2& screenRate, bool fullscreen);
		bool CreateRasterize();


		Vector::Vector4				m_backcolor;
		bool						m_vsync_enabled = false;
		int							m_videoCardMemory = 0;
		char						m_videoCardDescription[128] = { 0, };
		IDXGISwapChain*				m_swapChain = nullptr;
		ID3D11Device*				m_device = nullptr;
		ID3D11DeviceContext*		m_deviceContext = nullptr;
		ID3D11RenderTargetView*		m_renderTargetView = nullptr;
		ID3D11Texture2D*			m_depthStencilBuffer = nullptr;
		ID3D11DepthStencilState*	m_depthStencilState = nullptr;
		ID3D11DepthStencilState*    m_depthDisabledStencilState = nullptr;
		ID3D11DepthStencilView*		m_depthStencilView = nullptr;
		ID3D11RasterizerState*		m_rasterState = nullptr;
		ID3D11RasterizerState*		m_rasterState_WireFrame = nullptr;
		ID3D11RasterizerState*		m_rasterState_NoCulling = nullptr;

		ID3D11BlendState* m_alphaEnableBlendingState = nullptr;
		ID3D11BlendState* m_alphaDisableBlendingState = nullptr;

		XMMATRIX					m_worldMatrix;
	};
}