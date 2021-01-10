#include "pch.h"
#include "GridTerrain.h"

GridTerrain::GridTerrain()
{
}

GridTerrain::~GridTerrain()
{
}

bool GridTerrain::Inite(ID3D11Device * device, HWND hwnd, int width, int height)
{
	m_terrainWidth = width;
	m_terrainHeight = height; 
	this->CreateBuffer(device);

	this->gameobject->GetCompponent<Transform>()->SetPosition(width * -0.5f, 0, height * -0.5f);

	m_shader = new ColorShader();

	return m_shader->Inite(device, hwnd, Default_COLOR_VS, Default_COLOR_PS);
}

void GridTerrain::Destroy()
{
	if (m_shader)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void GridTerrain::Update(UpdateQueue * uQueue)
{
}

bool GridTerrain::CreateBuffer(ID3D11Device * device)
{
	// 지형 메쉬의 정점 수를 계산합니다.
	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 8;

	// 인덱스 수를 꼭지점 수와 같게 설정합니다.
	m_indexCount = m_vertexCount;

	// 정점 배열을 만듭니다.
	GridVertexType* vertices = new GridVertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점 배열에 대한 인덱스를 초기화합니다.
	int index = 0;

	// 지형 데이터로 정점 및 인덱스 배열을 로드합니다.
	for (int j = 0; j<(m_terrainHeight - 1); j++)
	{
		for (int i = 0; i<(m_terrainWidth - 1); i++)
		{
			// LINE 1
			// 왼쪽 위.
			float positionX = (float)i;
			float positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// 오른쪽 위.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 2
			// 오른쪽 위.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// 오른쪽 아래.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 3
			// 오른쪽 아래.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// 왼쪽 아래.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// LINE 4
			// 왼쪽 아래.
			positionX = (float)i;
			positionZ = (float)j;

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;

			// 왼쪽 위.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			indices[index] = index;
			index++;
		}
	}

	// 정적 정점 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GridVertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 이제 버퍼가 생성되고 로드된 배열을 해제하십시오.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool GridTerrain::Render(ID3D11DeviceContext * deviceContext)
{
	this->RenderMesh(deviceContext);
	if (!this->m_shader->SetShaderParameters(deviceContext, this->gameobject->GetCompponent<Transform>()))
	{
		printf("Error :  GridTerrain::Render(ID3D11DeviceContext * deviceContext) \n");
		return false;
	}
	this->m_shader->RenderShader(deviceContext, this->m_indexCount);

	return true;
}

void GridTerrain::RenderMesh(ID3D11DeviceContext * deviceContext)
{
	// 정점 버퍼 보폭 및 오프셋을 설정합니다.
	unsigned int stride = sizeof(GridVertexType);
	unsigned int offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 버텍스 버퍼에서 렌더링되어야하는 프리미티브의 타입을 설정한다.이 경우 라인리스트이다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
