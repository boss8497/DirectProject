#include "pch.h"
#include "NoLightShader.h"

NoLightShader::NoLightShader()
{
}

NoLightShader::~NoLightShader()
{
}

bool NoLightShader::Inite(ID3D11Device * device, HWND hwnd, const char * vs_filename, const char * ps_filename)
{
	this->SetFileName(vs_filename, ps_filename);
	return LoadShader(device, hwnd);
}

bool NoLightShader::LoadShader(ID3D11Device * device, HWND hwnd)
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
		return false;
	}


	//픽셀 쉐이더 코드 컴파일
	ID3D10Blob* pixelShaderBuffer = nullptr;
	MethodHelper::GetWC(filePath, m_PS_Filename);

	result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		return false;
	}



	//버퍼로 부터 정점 셰이더를 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//버퍼로 부터 픽셀 셰이더 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}



	//정점 입력 레이아웃 구조체를 설정
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
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

	//D3D11_APPEND_ALIGNED_ELEMENT
	//다음 버퍼 위치를 자동 계산해줌
	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;


	//레이아웃 요소 수
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//정점 입력 레이아웃을 만듬
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		return false;
	}


	//버퍼 해제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	//정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//상수 버퍼 포인터를 만들어 이 클래스에서 정점 셰이더 상수 버퍼에 접근 할 수 있게 합니다.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

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

	result = device->CreateSamplerState(&samplerDesc, &this->m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

bool NoLightShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, Transform * tr)
{
	//상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		printf("Shader Map error\n");
		return false;
	}


	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	//상수버퍼에 행렬을 복사
	dataPtr->world = XMLoadFloat4x4(&Matrix::Matrix::Transpose(tr->GetWorldMatrix()));
	dataPtr->projection = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::Projection_Matrix));
	dataPtr->view = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::View_Matrix));

	//상수 버펄의 잠금을 품
	deviceContext->Unmap(m_matrixBuffer, 0);

	//정점 셰이더에서의 상수 버퍼의 위치를 설정
	UINT bufferNumber = 0;

	//마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿈
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//픽셀 셰이더에서 셰이더 텍스처 리소스를 설정
	deviceContext->PSSetShaderResources(0, 1, &texture);
	return true;
}
