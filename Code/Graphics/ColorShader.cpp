#include "pch.h"
#include "ColorShader.h"

ColorShader::ColorShader()
{
}

ColorShader::~ColorShader()
{
}

bool ColorShader::Inite(ID3D11Device * device, HWND hwnd, const char * vs_filename, const char * ps_filename)
{
	this->SetFileName(vs_filename, ps_filename);
	return LoadShader(device, hwnd);
}

bool ColorShader::LoadShader(ID3D11Device * device, HWND hwnd)
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
		return false;
	}


	//�ȼ� ���̴� �ڵ� ������
	ID3D10Blob* pixelShaderBuffer = nullptr;
	MethodHelper::GetWC(filePath, m_PS_Filename);

	result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		return false;
	}



	//���۷� ���� ���� ���̴��� ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//���۷� ���� �ȼ� ���̴� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}



	//���� �Է� ���̾ƿ� ����ü�� ����
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;


	//���̾ƿ� ��� ��
	UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//���� �Է� ���̾ƿ��� ����
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);

	if (FAILED(result))
	{
		return false;
	}


	//���� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	//���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ�
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ���� �� �� �ְ� �մϴ�.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, Transform * tr)
{
	//��� ������ ������ �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		printf("Shader Map error\n");
		return false;
	}


	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	//������ۿ� ����� ����
	dataPtr->world = XMLoadFloat4x4(&Matrix::Matrix::Transpose(tr->GetWorldMatrix()));
	dataPtr->projection = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::Projection_Matrix));
	dataPtr->view = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::View_Matrix));

	//��� ������ ����� ǰ
	deviceContext->Unmap(m_matrixBuffer, 0);

	//���� ���̴������� ��� ������ ��ġ�� ����
	UINT bufferNumber = 0;

	//���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲ�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	return true;
}
