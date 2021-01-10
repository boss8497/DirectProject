#include "pch.h"
#include "Shader.h"
#include "MethodHelper.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Inite(ID3D11Device * device, HWND hwnd, const char * vs_filename, const char * ps_filename)
{
	this->SetFileName(vs_filename, ps_filename);

	return LoadShader(device, hwnd);
}

bool Shader::IniteFbx(ID3D11Device * device, HWND hwnd, const char * vs_filename, const char * ps_filename)
{
	this->SetFileName(vs_filename, ps_filename);
	return FbxLoadShader(device, hwnd);
}

void Shader::Destroy()
{
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_textureBuffer)
	{
		m_textureBuffer->Release();
		m_textureBuffer = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

bool Shader::LoadShader(ID3D11Device * device, HWND hwnd)
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


	//픽셀 쉐이더에있는 광원 동적 상수 버퍼의 설명을 설정
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듬
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}


	//텍스쳐 상수 버퍼
	D3D11_BUFFER_DESC textureBufferDesc;
	textureBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureBufferDesc.ByteWidth = sizeof(TextureBufferType);
	textureBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	textureBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureBufferDesc.MiscFlags = 0;
	textureBufferDesc.StructureByteStride = 0;

	//이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듬
	result = device->CreateBuffer(&textureBufferDesc, NULL, &m_textureBuffer);
	if (FAILED(result))
	{
		return false;
	}


	return true;
}

bool Shader::FbxLoadShader(ID3D11Device * device, HWND hwnd)
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
	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "BLENDWEIGHT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BLENINICES";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

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
	matrixBufferDesc.ByteWidth = sizeof(FbxMatrixBufferType);
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


	//픽셀 쉐이더에있는 광원 동적 상수 버퍼의 설명을 설정
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	//이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듬
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}




	//텍스쳐 상수 버퍼
	D3D11_BUFFER_DESC textureBufferDesc;
	textureBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureBufferDesc.ByteWidth = sizeof(TextureBufferType);
	textureBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	textureBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureBufferDesc.MiscFlags = 0;
	textureBufferDesc.StructureByteStride = 0;

	//이 클래스 내에서 정점 셰이더 상수 버퍼에 액세스 할 수 있도록 상수 버퍼 포인터를 만듬
	result = device->CreateBuffer(&textureBufferDesc, NULL, &m_textureBuffer);
	if (FAILED(result))
	{
		return false;
	}




	return true;
}

bool Shader::SetShaderParameters(ID3D11DeviceContext * deviceContext, Transform * tr, ID3D11ShaderResourceView * texture, DirectionalLight * light, TextureBufferType* textureBuffer)
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




	//light constant buffer를 잠글 수 있도록 기록
	if (FAILED(deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}


	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->diffuseColor = light->GetDiffuseColor();
	dataPtr2->ambientColor = light->GetAmbientColor();
	dataPtr2->lightDirection = light->GetDirection();
	dataPtr2->padding = 0.0f;

	//상수 버퍼 잠금을 해제
	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 3;

	//마지막으로 업데이트 된 값으로 픽셀 쉐이더에서 광원 상수 버퍼를 설정
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);




	////텍스쳐 상수버퍼
	if (FAILED(deviceContext->Map(m_textureBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	TextureBufferType* dataPtr3 = (TextureBufferType*)mappedResource.pData;

	*dataPtr3 = *textureBuffer;

	//상수 버퍼 잠금을 해제
	deviceContext->Unmap(m_textureBuffer, 0);
	
	bufferNumber = 4;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_textureBuffer);

	return true;
}

bool Shader::FbxSetShaderParameters(ID3D11DeviceContext * deviceContext, ID3D11ShaderResourceView * texture, DirectionalLight * light,
	SkeletonObjPtr& skeletonPtr, std::vector<Matrix::Matrix>& matrix, TextureBufferType* textureBuffer)
{
	//상수 버퍼의 내용을 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		printf("Shader Map error\n");
		return false;
	}

	FbxMatrixBufferType* dataPtr = (FbxMatrixBufferType*)mappedResource.pData;

	dataPtr->skeletonBase = matrix[0].GetXMMATRIX();
	//상수버퍼에 행렬을 복사
	for (int i = 0; i < BoneMatrixCount; ++i)
	{
		if (i >= static_cast<int>(skeletonPtr.m_objList.size()))
		{
			dataPtr->skeleont[i] = Matrix::Matrix::Identity().GetXMMATRIX();
		}
		else
		{
			Transform* tr = skeletonPtr.m_objList[i]->GetCompponent<Transform>();

			if (tr == nullptr)
				continue;

			Vector::Vector3 translate = tr->Fbx_worldMatrix.GetT();
			Vector::Vector3 scale = tr->Fbx_worldMatrix.GetS();
			Vector::Vector3 rotate = tr->Fbx_worldMatrix.GetR();

			dataPtr->skeleont[i] = Matrix::Matrix::SRT(scale, rotate, translate).GetXMMATRIX();
			//dataPtr->skeleont[i] = matrix[i].GetXMMATRIX();
			//dataPtr->skeleont[i] = tr->GetWorldMatrix().GetXMMATRIX();
			
		}
	}

	dataPtr->world = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Matrix::Matrix::Identity()));
	dataPtr->projection = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::Projection_Matrix));
	dataPtr->view = XMLoadFloat4x4(&Matrix::Matrix::Transpose(Transform::View_Matrix));
	//상수 버펄의 잠금을 품
	deviceContext->Unmap(m_matrixBuffer, 0);

	//정점 셰이더에서의 상수 버퍼의 위치를 설정
	UINT bufferNumber = 0;

	//마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿈
	deviceContext->VSSetConstantBuffers(bufferNumber++, 1, &m_matrixBuffer);

	//픽셀 셰이더에서 셰이더 텍스처 리소스를 설정
	deviceContext->PSSetShaderResources(0, 1, &texture);


	//light constant buffer를 잠글 수 있도록 기록
	if (FAILED(deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}


	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;

	dataPtr2->diffuseColor = light->GetDiffuseColor();
	dataPtr2->ambientColor = light->GetAmbientColor();
	dataPtr2->lightDirection = light->GetDirection();
	dataPtr2->padding = 0.0f;

	//상수 버퍼 잠금을 해제
	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 3;

	//마지막으로 업데이트 된 값으로 픽셀 쉐이더에서 광원 상수 버퍼를 설정
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);


	////텍스쳐 상수버퍼
	if (FAILED(deviceContext->Map(m_textureBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	TextureBufferType* dataPtr3 = (TextureBufferType*)mappedResource.pData;

	*dataPtr3 = *textureBuffer;

	//상수 버퍼 잠금을 해제
	deviceContext->Unmap(m_textureBuffer, 0);

	bufferNumber = 4;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_textureBuffer);

	return true;
}

void Shader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
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

void Shader::SetFileName(const char * vs, const char * ps)
{
	if (vs == nullptr)
	{
		ZeroMemory(m_VS_Filename, FILE_NAME_MAX);
		strcpy_s(m_VS_Filename, FILE_NAME_MAX, Default_VS);
	}
	else
	{
		ZeroMemory(m_VS_Filename, FILE_NAME_MAX);
		strcpy_s(m_VS_Filename, FILE_NAME_MAX, vs);
	}

	if (ps == nullptr)
	{
		ZeroMemory(m_PS_Filename, FILE_NAME_MAX);
		strcpy_s(m_PS_Filename, FILE_NAME_MAX, Default_PS);
	}
	else
	{
		ZeroMemory(m_PS_Filename, FILE_NAME_MAX);
		strcpy_s(m_PS_Filename, FILE_NAME_MAX, ps);
	}
}

