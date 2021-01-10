#include"pch.h"
#include "FontMesh.h"

FontMesh::FontMesh()
{
}

FontMesh::~FontMesh()
{
}

FontMesh::FontMesh(const FontMesh & copy)
{
}

bool FontMesh::Inite(ID3D11Device * device)
{
	if (!this->IniteBuffer(device))
	{
		printf("FontMesh : IniteBuffer Error \n");
		return false;
	}

	return true;
}

void FontMesh::Destroy()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
}

bool FontMesh::UpdateVertexBuffer(ID3D11DeviceContext * deviceContext, void* vertices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	// 버텍스 버퍼를 쓸 수 있도록 잠급니다.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}


	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(mappedResource.pData, vertices, (sizeof(VertexType) * m_vertexCount));


	// 정점 버퍼의 잠금을 해제합니다.
	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}

void FontMesh::Render(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성 시킨다
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//랜더링 할 수 있도록 입력 어셈블러에서 인덱ㄱ스 버퍼를 활성
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//정점 버퍼로 그릴 기본형을 설정합니다. 삼각형으로
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int FontMesh::GetVertexCount()
{
	return m_vertexCount;
}

int FontMesh::GetIndexCount()
{
	return m_indexCount;
}

bool FontMesh::IniteBuffer(ID3D11Device * device)
{
	/*6은 사각형 하나의 Vertx숫자(삼각형 2개)*/
	m_indexCount = m_vertexCount = 6 * TEXT_LENGTH_MAX;

	VertexType* vertices = new VertexType[m_vertexCount];
	unsigned long* indices = new unsigned long[m_indexCount];

	memset(vertices, 0, sizeof(VertexType)*m_vertexCount);
	memset(indices, 0, sizeof(unsigned long)*m_indexCount);


	for (int i = 0; i < m_vertexCount; i++)
	{
		indices[i] = i;
	}




	//정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//정점 버퍼 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		printf("FontMesh CreateVertexBuffer Erorr \n");
		return false;
	}


	//정적 인덱스 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	//인덱스 버퍼 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		printf("FontMesh CreateIndexBuffer Erorr \n");
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
