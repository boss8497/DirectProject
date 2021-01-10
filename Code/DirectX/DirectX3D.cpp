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
	//버퍼를 지울 색을 설정
	float color[4] = { m_backcolor.x, m_backcolor.y, m_backcolor.z, m_backcolor.w };
	//백버퍼 지우기
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//깊이 버퍼 지우기
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D::DirectX3D::EndScene()
{
	//렌더링이 완료되었으면 화면에 백 버퍼를 표시
	if (m_vsync_enabled)
	{
		//새로고침 비율 고정
		m_swapChain->Present(1, 0);
	}
	else
	{
		//가능한 빠르게
		m_swapChain->Present(0, 0);
	}
}

void D3D::DirectX3D::SetBackColor(Vector::Vector4 & color)
{
	m_backcolor = color;
}

void D3D::DirectX3D::Destroy()
{
	//종료전 윈도우 모드로 설정하지 않으면 스왑 체인을 해제 할 때 예외 발생
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
	//DirectX 그래픽 인터페이스 팩토리를 생성
	/*adapter를 생성하기 위해*/
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
	{
		return false;
	}

	//팩토리 객체를 사용해 첫번째 그래픽 카드 인터페이스 어뎁터 생성
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
	{
		return false;
	}

	//출력(모니터)에 대한 어뎁터
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
	{
		return false;
	}

	//모니터에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옴
	//ex)800x600, 1920x1080의 갯수
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
	{
		return false;
	}


	//사용 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트를 생성
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	//디스플레이 모드에 대한 리스트를 채움
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList)))
	{
		return false;
	}

	//모든 디시플레이 모드에 대한 화면 너비/높이에 맞는 디스플레이 모드를 찾음
	//찾으면 모니터의 새로고침 비율값 저장 (분모, 분자)
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

	//비디오카드의 구조체
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
	{
		return false;
	}

	//비디오카드 메모리 용량 단위를 메가바이트 단위로 저장
	m_videoCardMemory = (int)(adapterDesc.DedicatedSystemMemory / 1024 / 1024);

	//비디오카드 이름을 저장
	size_t stringLength = 0;
	//wcs to mbs 유니코드문자열을 아스키코드로 전환
	if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
	{
		return false;
	}

	factory->MakeWindowAssociation(hwnd, NULL);


	//모든 메모리 해제
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
	//스왑체인 구조체를 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//백버퍼를 1개만 사용 설정
	swapChainDesc.BufferCount = 1;

	//백버퍼의 넓이와 높이 저장
	swapChainDesc.BufferDesc.Width = static_cast<UINT>(screenSize.x);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(screenSize.y);

	//32bit 서페이스를 설정
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//백버퍼의 새로고침 비율 설정
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

	//백버퍼 사용 용도 지정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//랜더링에 사용될 윈동우 핸들 지정
	swapChainDesc.OutputWindow = hwnd;

	//멀티샘플링 끄기
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	//창모드 or 풀스크린 모드 설정
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	//스캔 라인 순서 및 크기를 지정하지 않음으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//출력 된 다음 백버퍼를 비우도록 지정
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//추가 옵션 플래그를 사용x
	swapChainDesc.Flags = 0;

	//피처 레벨을 Directx11로 설정
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


	/* 나중에 디바이스별 좋은 디바이스를 찾아 생성 해줘야됨 */
	//스왑 체인,Direct3D 장치 및 Direct3D 장치 컨텍스트를 만들어줌
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




	//백버퍼 포인터를 가져옴
	ID3D11Texture2D* backBufferPtr = nullptr;
	if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
	{
		return false;
	}

	//백버퍼가 랜더링타겟뷰 대상으로 설정
	//백버퍼 포인터로 랜더 타겟 뷰 생성
	if (FAILED(m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView)))
	{
		return false;
	}

	//백버퍼 포인터 해제
	backBufferPtr->Release();
	backBufferPtr = 0;


	//깊이 버퍼 구조체생성 초기화
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	//깊이 버퍼 구조체 작성
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


	//디스크립션을 사용해 깊이 버퍼의 텍스쳐 생성
	if (FAILED(m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer)))
	{
		return false;
	}


	//스텐실 상태 구조체 초기화
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태 구조체를 작성합니다
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀 정면의 스텐실 설정입니다
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀 뒷면의 스텐실 설정입니다
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//깊이 스텐실상태를 생성
	if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState)))
	{
		return false;
	}

	//깊이스텐실 상태를 설정
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	//깊이 스텐실 뷰의 구조체 초기화
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	//깊이스텐실뷰 구조체 설정
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	//깊이 스텐실 뷰를 생성
	if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView)))
	{
		return false;
	}

	//랜더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩하기
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);


	//2D렌더를 위한 Z 버퍼를 끄는 스텐실 상태 생성
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

	//장치를 사용해 상태를 만든다
	if (FAILED(m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState)))
	{
		return false;
	}

	//블렌딩
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


	/*보여지는 공간을 클리핑(자르는) 수행*/
	//렌더링을 위해 뷰포트를 설정
	D3D11_VIEWPORT viewport;
	viewport.Width = screenSize.x;
	viewport.Height = screenSize.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	//뷰포트를 생성
	m_deviceContext->RSSetViewports(1, &viewport);


	//월드 행렬을 항등 행렬로 초기화
	m_worldMatrix = DirectX::XMMatrixIdentity();

	return true;
}

bool D3D::DirectX3D::CreateRasterize()
{
	//어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기
	//그려지는 폴리곤과 방법을 결정할 래스터 구조체
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

	//래스터 라이저 상태를 만듬
	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState)))
	{
		return false;
	}

	//래스터 상태를 만들고 래스터 라이저 상태를 설정
	m_deviceContext->RSSetState(m_rasterState);

	

	//와이어 프레임 생성
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_NONE;

	if (FAILED(m_device->CreateRasterizerState(&rasterDesc, &m_rasterState_WireFrame)))
	{
		return false;
	}



	//컬링 해제
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
