#include "pch.h"
#include "DirectX3D.h"

D3D::DirectX3D::DirectX3D()
{
}

bool D3D::DirectX3D::Inite(Vector::Vector2 screenSize, HWND hwnd, bool fullscreen, bool vsync)
{
	Vector::Vector2 screenRate;
	m_vsync_enabled = vsync;

	if (!CreateDevice(hwnd, screenSize, &screenRate))
	{
		return false;
	}

	if (!CreateSwapchangeAndDevice(hwnd, screenSize, screenRate, fullscreen))
	{
		return false;
	}

	if (!SetBackbufferAndView(screenSize, screenRate, false))
	{
		return false;
	}


	if (!CreateRasterize())
	{
		return false;
	}

	return true;
}

void D3D::DirectX3D::BeginScene()
{
	//���۸� ���� ���� ����
	float color[4] = { m_backcolor.x, m_backcolor.y, m_backcolor.z, m_backcolor.w };
	//����� �����
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//���� ���� �����
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D::DirectX3D::EndScene()
{
	//�������� �Ϸ�Ǿ����� ȭ�鿡 �� ���۸� ǥ��
	if (m_vsync_enabled)
	{
		//���ΰ�ħ ���� ����
		m_swapChain->Present(1, 0);
	}
	else
	{
		//������ ������
		m_swapChain->Present(0, 0);
	}
}

void D3D::DirectX3D::SetBackColor(Vector::Vector4 & color)
{
	m_backcolor = color;
}

void D3D::DirectX3D::Destroy()
{
	//������ ������ ���� �������� ������ ���� ü���� ���� �� �� ���� �߻�
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}


	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}
	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}
	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}


ID3D11Device * D3D::DirectX3D::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext * D3D::DirectX3D::GetDeviceContext()
{
	return m_deviceContext;
}


void D3D::DirectX3D::GetWorldMatrix(XMMATRIX* m)
{
	*m = m_worldMatrix;
}


void D3D::DirectX3D::GetVideoCardInfo(char* name, int* memory)
{
	strcpy_s(name, 128, m_videoCardDescription);
	*memory = m_videoCardMemory;
}

void D3D::DirectX3D::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3D::DirectX3D::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}

void D3D::DirectX3D::TurnOnAlphaBlending()
{
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void D3D::DirectX3D::TurnOffAlphaBlending()
{
	float blendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

void D3D::DirectX3D::TurnOnWireFrame()
{
	this->m_deviceContext->RSSetState(this->m_rasterState_WireFrame);
}

void D3D::DirectX3D::TurnOffWireFrame()
{
	this->m_deviceContext->RSSetState(this->m_rasterState);
}

void D3D::DirectX3D::TurnOnNoCulling()
{
	this->m_deviceContext->RSSetState(this->m_rasterState_NoCulling);
}

void D3D::DirectX3D::TurnOffNoCulling()
{
	this->m_deviceContext->RSSetState(this->m_rasterState);
}

bool D3D::DirectX3D::CreateDevice(HWND hwnd, Vector::Vector2& screenSize, Vector::Vector2* screenRate)
{
	//DirectX �׷��� �������̽� ���丮�� ����
	/*adapter�� �����ϱ� ����*/
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
	{
		return false;
	}

	//���丮 ��ü�� ����� ù��° �׷��� ī�� �������̽� ��� ����
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
	{
		return false;
	}

	//���(�����)�� ���� ���
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		return false;
	}

	//����Ϳ� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� ������
	//ex)800x600, 1920x1080�� ����
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
	{
		return false;
	}


	//��� ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ�� ����
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//���÷��� ��忡 ���� ����Ʈ�� ä��
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList)))
	{
		return false;
	}

	//��� ����÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã��
	//ã���� ������� ���ΰ�ħ ������ ���� (�и�, ����)
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int)screenSize.x)
		{
			if (displayModeList[i].Height == (unsigned int)screenSize.y)
			{
				screenRate->x = static_cast<float>(displayModeList[i].RefreshRate.Numerator);
				screenRate->y = static_cast<float>(displayModeList[i].RefreshRate.Denominator);

				break;
			}
		}
	}

	//����ī���� ����ü
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	//����ī�� �޸� �뷮 ������ �ް�����Ʈ ������ ����
	m_videoCardMemory = (int)(adapterDesc.DedicatedSystemMemory / 1024 / 1024);

	//����ī�� �̸��� ����
	size_t stringLength = 0;
	//wcs to mbs �����ڵ幮�ڿ��� �ƽ�Ű�ڵ�� ��ȯ
	if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		return false;
	}

	factory->MakeWindowAssociation(hwnd, NULL);


	//��� �޸� ����
	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	return true;
}

bool D3D::DirectX3D::CreateSwapchangeAndDevice(HWND hwnd,Vector::Vector2& screenSize, Vector::Vector2& screenRate,bool fullscreen)
{
	//����ü�� ����ü�� �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//����۸� 1���� ��� ����
	swapChainDesc.BufferCount = 1;

	//������� ���̿� ���� ����
	swapChainDesc.BufferDesc.Width = static_cast<UINT>(screenSize.x);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(screenSize.y);

	//32bit �����̽��� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//������� ���ΰ�ħ ���� ����
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = static_cast<UINT>(screenRate.x);
		swapChainDesc.BufferDesc.RefreshRate.Denominator = static_cast<UINT>(screenRate.y);
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	//����� ��� �뵵 ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//�������� ���� ������ �ڵ� ����
	swapChainDesc.OutputWindow = hwnd;

	//��Ƽ���ø� ����
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//â��� or Ǯ��ũ�� ��� ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	//��ĵ ���� ���� �� ũ�⸦ �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//��� �� ���� ����۸� ��쵵�� ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//�߰� �ɼ� �÷��׸� ���x
	swapChainDesc.Flags = 0;

	//��ó ������ Directx11�� ����
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


	/* ���߿� ����̽��� ���� ����̽��� ã�� ���� ����ߵ� */
	//���� ü��,Direct3D ��ġ �� Direct3D ��ġ ���ؽ�Ʈ�� �������
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext)))
	{
		return false;
	}

	return true;
}

bool D3D::DirectX3D::SetBackbufferAndView(Vector::Vector2& screenSize, Vector::Vector2& screenRate, bool resize)
{
	if (resize)
	{
		DXGI_MODE_DESC desc;

		desc.Width = static_cast<UINT>(screenSize.x);
		desc.Height = static_cast<UINT>(screenSize.y);
		desc.RefreshRate.Denominator = 0;
		desc.RefreshRate.Numerator = 0;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		m_swapChain->ResizeTarget(&desc);

		m_deviceContext->OMSetRenderTargets(0, NULL, NULL);
		m_renderTargetView->Release();
		m_depthStencilBuffer->Release();
		m_depthStencilState->Release();
		m_depthStencilView->Release();

		m_swapChain->ResizeBuffers(1, static_cast<UINT>(screenSize.x), static_cast<UINT>(screenSize.y), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	}




	//����� �����͸� ������
	ID3D11Texture2D* backBufferPtr = nullptr;
	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
	{
		return false;
	}

	//����۰� ������Ÿ�ٺ� ������� ����
	//����� �����ͷ� ���� Ÿ�� �� ����
	if (FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
	{
		return false;
	}

	//����� ������ ����
	backBufferPtr->Release();
	backBufferPtr = 0;


	//���� ���� ����ü���� �ʱ�ȭ
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//���� ���� ����ü �ۼ�
	depthBufferDesc.Width = static_cast<UINT>(screenSize.x);
	depthBufferDesc.Height = static_cast<UINT>(screenSize.y);
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;


	//��ũ������ ����� ���� ������ �ؽ��� ����
	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
	{
		return false;
	}


	//���ٽ� ���� ����ü �ʱ�ȭ
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���ٽ� ���� ����ü�� �ۼ��մϴ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ� ������ ���ٽ� �����Դϴ�
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ� �޸��� ���ٽ� �����Դϴ�
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//���� ���ٽǻ��¸� ����
	if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
	{
		return false;
	}

	//���̽��ٽ� ���¸� ����
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	//���� ���ٽ� ���� ����ü �ʱ�ȭ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//���̽��ٽǺ� ����ü ����
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//���� ���ٽ� �並 ����
	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
	{
		return false;
	}

	//����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε��ϱ�
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	//2D������ ���� Z ���۸� ���� ���ٽ� ���� ����
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//��ġ�� ����� ���¸� �����
	if (FAILED(m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState)))
	{
		return false;
	}

	//����
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));


	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC1_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	if (FAILED(m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState)))
	{
		return false;
	}


	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	if (FAILED(m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState)))
	{
		return false;
	}


	/*�������� ������ Ŭ����(�ڸ���) ����*/
	//�������� ���� ����Ʈ�� ����
	D3D11_VIEWPORT viewport;
	viewport.Width = screenSize.x;
	viewport.Height = screenSize.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//����Ʈ�� ����
	m_deviceContext->RSSetViewports(1, &viewport);


	//���� ����� �׵� ��ķ� �ʱ�ȭ
	m_worldMatrix = DirectX::XMMatrixIdentity();

	return true;
}

bool D3D::DirectX3D::CreateRasterize()
{
	//� ������ ��� �׸� ������ �����ϴ� ������ȭ��
	//�׷����� ������� ����� ������ ������ ����ü
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	//������ ������ ���¸� ����
	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
	{
		return false;
	}

	//������ ���¸� ����� ������ ������ ���¸� ����
	m_deviceContext->RSSetState(m_rasterState);

	

	//���̾� ������ ����
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState_WireFrame)))
	{
		return false;
	}



	//�ø� ����
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState_NoCulling)))
	{
		return false;
	}

	return true;
}
