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

	//���ؽ� ���̴� �ڵ带 ������
	ID3D10Blob* vertexShaderBuffer = nullptr;
	MethodHelper::GetWC(filePath, m_VS_Filename);


	result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		printf("TextShader : D3DCompileFromFile - Vertex Error\n");
		return false;
	}


	//�ȼ� ���̴� �ڵ� ������
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



	// ���۷κ��� ���� ���̴��� �����Ѵ�.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// ���ۿ��� �ȼ� ���̴��� �����մϴ�.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// ���� �Է� ���̾ƿ� ����ü�� �����մϴ�.
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

	// ���̾ƿ��� ��� ���� �����ɴϴ�.
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���� �Է� ���̾ƿ��� ����ϴ�.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� �����մϴ�.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ְ� �մϴ�.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ؽ�ó ���÷� ���� ����ü�� ���� �� �����մϴ�.
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

	// �ؽ�ó ���÷� ���¸� ����ϴ�.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	//�ȼ����̴� c����
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

	//��� ������ ������ �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	/*���̴� ������� cBuffer*/
	//��� ������ �����Ϳ� ���� �����͸� ������
	ConstantBufferType* dataPtr = (ConstantBufferType*)mappedResource.pData;

	//������ۿ� ����� ����
	dataPtr->world = Matrix::Matrix::Transpose(Transform::World_Matrix).GetXMMATRIX();
	dataPtr->view = Matrix::Matrix::Transpose(Transform::ViewUI_Matrix).GetXMMATRIX();
	dataPtr->projection = DirectX::XMMatrixTranspose(Screen::Instance().GetOrthographicMatrix());

	//��� ������ ����� ǰ
	deviceContext->Unmap(m_matrixBuffer, 0);

	//���� ���̴������� ��� ������ ��ġ�� ����
	UINT bufferNumber = 0;

	//���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲ�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	//�ȼ� ���̴����� ���̴� �ؽ�ó ���ҽ��� ����
	deviceContext->PSSetShaderResources(0, 1, &texture);


	//�ȼ� ���̴� �ۤ���
	if (FAILED(deviceContext->Map(m_pixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	PixelBufferType* dataPtr2 = (PixelBufferType*)mappedResource.pData;

	dataPtr2->pixelColor = color;

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_pixelBuffer, 0);

	bufferNumber = 0;

	//���������� �ȼ� ���̴��� ��� ���۸� �ٲ� ������ �ٲ�
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pixelBuffer);
	return true;
}

void FontShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	//���� �Է� ���̾ƿ��� ����
	deviceContext->IASetInputLayout(m_layout);

	//�ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� ����
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	//�ȼ� ���̴����� ���÷� ���¸� ����
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	//�ﰢ���� �׸��ϴ�.
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
