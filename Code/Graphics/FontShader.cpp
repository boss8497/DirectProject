#include "pch.h"
#include "FontShader.h"
#include"MethodHelper.h"
#include"Screen.h"

FontShader::FontShader()
{
}

FontShader::~FontShader()
{
}

FontShader::FontShader(const FontShader & copy)
{
}

bool FontShader::Inite(ID3D11Device * device, HWND hwnd, char * vs_filename, char * ps_filename)
{
	this->SetFileName(vs_filename, ps_filename);
	return this->LoadShader(device, hwnd);
}

void FontShader::Destroy()
{

	if (m_pixelBuffer)
	{
		m_pixelBuffer->Release();
		m_pixelBuffer = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}


	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}
}

bool FontShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, Transform * tr, ID3D11ShaderResourceView * texture, Vector::Vector4 color)
{
	if (!this->SetShaderParameters(deviceContext, tr, texture, color))
	{
		printf("TextShader : SetShaderParameters Error\n");
		return false;
	}

	this->RenderShader(deviceContext, indexCount);

	return true;
}

bool FontShader::LoadShader(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	ID3D10Blob* errorMessage = nullptr;
	std::wstring filePath;

	//버텍스 쉐이더 코드를 컴파일
	ID3D10Blob* vertexShaderBuffer = nullptr;
	MethodHelper::GetWC(filePath, m_VS_Filename);


	result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		printf("TextShader : D3DCompileFromFile - Vertex Error\n");
		return false;
	}


	//픽셀 쉐이더 코드 컴파일
	ID3D10Blob* pixelShaderBuffer = nullptr;
	MethodHelper::GetWC(filePath, m_PS_Filename);

	result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		printf("%s\n", reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));
		printf("TextShader : D3DCompileFromFile - Pixel Error\n");
		return false;
	}



	// 버퍼로부터 정점 셰이더를 생성한다.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// 버퍼에서 픽셀 쉐이더를 생성합니다.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// 정점 입력 레이아웃 구조체를 설정합니다.
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃의 요소 수를 가져옵니다.
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 정점 입력 레이아웃을 만듭니다.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제합니다.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근할 수 있게 합니다.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 텍스처 샘플러 상태 구조체를 생성 및 설정합니다.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// 텍스처 샘플러 상태를 만듭니다.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	//픽셀쉐이더 c버퍼
	D3D11_BUFFER_DESC pixelBufferDesc;
	pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pixelBufferDesc.MiscFlags = 0;
	pixelBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&pixelBufferDesc, NULL, &m_pixelBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool FontShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, Transform * tr, ID3D11ShaderResourceView * texture, Vector::Vector4 color)
{
	HRESULT result;

	//상수 버퍼의 내요을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	/*쉐이더 상수버퍼 cBuffer*/
	//상수 버퍼의 데이터에 대한 포인터를 가져옴
	ConstantBufferType* dataPtr = (ConstantBufferType*)mappedResource.pData;

	//상수버퍼에 행렬을 복사
	dataPtr->world = Matrix::Matrix::Transpose(Transform::World_Matrix).GetXMMATRIX();
	dataPtr->view = Matrix::Matrix::Transpose(Transform::ViewUI_Matrix).GetXMMATRIX();
	dataPtr->projection = DirectX::XMMatrixTranspose(Screen::Instance().GetOrthographicMatrix());

	//상수 버펄의 잠금을 품
	deviceContext->Unmap(m_matrixBuffer, 0);

	//정점 셰이더에서의 상수 버퍼의 위치를 설정
	UINT bufferNumber = 0;

	//마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿈
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//픽셀 셰이더에서 셰이더 텍스처 리소스를 설정
	deviceContext->PSSetShaderResources(0, 1, &texture);


	//픽셀 쉐이더 작ㅇ업
	if (FAILED(deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	PixelBufferType* dataPtr2 = (PixelBufferType*)mappedResource.pData;

	dataPtr2->pixelColor = color;

	// 정점 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(m_pixelBuffer, 0);

	bufferNumber = 0;

	//마지막으로 픽셀 셰이더의 상수 버퍼를 바뀐 값으로 바꿈
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer);
	return true;
}

void FontShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//정점 입력 레이아웃을 설정
	deviceContext->IASetInputLayout(m_layout);

	//삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//픽셀 쉐이더에서 샘플러 상태를 설정
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//삼각형을 그립니다.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

void FontShader::SetFileName(char * vs, char * ps)
{
	if (vs == nullptr)
	{
		ZeroMemory(m_VS_Filename, FILE_NAME_MAX);
		strcpy_s(m_VS_Filename, FILE_NAME_MAX, Default_Font_VS);
	}
	else
	{
		ZeroMemory(m_VS_Filename, FILE_NAME_MAX);
		strcpy_s(m_VS_Filename, FILE_NAME_MAX, vs);
	}

	if (ps == nullptr)
	{
		ZeroMemory(m_PS_Filename, FILE_NAME_MAX);
		strcpy_s(m_PS_Filename, FILE_NAME_MAX, Default_Font_PS);
	}
	else
	{
		ZeroMemory(m_PS_Filename, FILE_NAME_MAX);
		strcpy_s(m_PS_Filename, FILE_NAME_MAX, ps);
	}
}
