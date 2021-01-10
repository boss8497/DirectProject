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
	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}


	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(mappedResource.pData, vertices, (sizeof(VertexType) * m_vertexCount));


	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	return true;
}

void FontMesh::Render(ID3D11DeviceContext * deviceContext)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ�� ��Ų��
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//������ �� �� �ֵ��� �Է� ��������� �ε����� ���۸� Ȱ��
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���� ���۷� �׸� �⺻���� �����մϴ�. �ﰢ������
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
	/*6�� �簢�� �ϳ��� Vertx����(�ﰢ�� 2��)*/
	m_indexCount = m_vertexCount = 6 * TEXT_LENGTH_MAX;

	VertexType* vertices = new VertexType[m_vertexCount];
	unsigned long* indices = new unsigned long[m_indexCount];

	memset(vertices, 0, sizeof(VertexType)*m_vertexCount);
	memset(indices, 0, sizeof(unsigned long)*m_indexCount);


	for (int i = 0; i < m_vertexCount; i++)
	{
		indices[i] = i;
	}




	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//���� ���� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		printf("FontMesh CreateVertexBuffer Erorr \n");
		return false;
	}


	//���� �ε��� ������ ����ü�� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//�ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;


	//�ε��� ���� ����
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
