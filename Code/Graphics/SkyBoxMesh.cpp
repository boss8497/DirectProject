#include "pch.h"
#include "SkyBoxMesh.h"

SkyBoxMesh::SkyBoxMesh()
{
}

SkyBoxMesh::~SkyBoxMesh()
{
}

bool SkyBoxMesh::Inite(ID3D11Device * device)
{
	this->LoadMesh(1.0f);
	this->CreateBuffer(device);
	//this->CreateShpereBox(device, 1.0f, 20, 20);

	return true;
}

void SkyBoxMesh::Update(UpdateQueue * uQueue)
{
}

bool SkyBoxMesh::Render(ID3D11DeviceContext * deviceContext)
{
	//���� ������ ������ �������� ����

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ��
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool SkyBoxMesh::LoadMesh(float size)
{
	std::vector<Vector::Vector3> vertex;
	//1
	vertex.push_back(Vector::Vector3(size, size, -size));
	vertex.push_back(Vector::Vector3(-size, size, -size));
	vertex.push_back(Vector::Vector3(size, size, size));
	vertex.push_back(Vector::Vector3(-size, size, size));

	//2
	vertex.push_back(Vector::Vector3(size, -size, size));
	vertex.push_back(Vector::Vector3(-size, -size, size));


	vertex.push_back(Vector::Vector3(size, -size, -size));
	vertex.push_back(Vector::Vector3(-size, -size, -size));



	vertex.push_back(Vector::Vector3(-size, size, -size));
	vertex.push_back(Vector::Vector3(-size, size, size));
	vertex.push_back(Vector::Vector3(-size, -size, -size));
	vertex.push_back(Vector::Vector3(-size, -size, size));


	//4
	vertex.push_back(Vector::Vector3(size, size, size));
	vertex.push_back(Vector::Vector3(size, size, -size));
	vertex.push_back(Vector::Vector3(size, -size, size));
	vertex.push_back(Vector::Vector3(size, -size, -size));


	////5
	vertex.push_back(Vector::Vector3(size, size, -size));
	vertex.push_back(Vector::Vector3(-size, size, -size));
	vertex.push_back(Vector::Vector3(size, -size, -size));
	vertex.push_back(Vector::Vector3(-size, -size, -size));

	this->m_model = new ModelType[vertex.size()];
	this->m_vertexCount = vertex.size();

	for (unsigned int i = 0; i < vertex.size(); ++i)
	{
		this->m_model[i].SetPosition(vertex[i]);
		this->m_model[i].SetNormal(Vector::Vector3::Normalize(vertex[i]));
	}


	float uStep = 0.25f;
	float vStep = 1.0f / 3.0f;

	
	this->m_model[0].SetUV(Vector::Vector2(uStep * 2.0f, 0.0f));
	this->m_model[1].SetUV(Vector::Vector2(uStep, 0));
	this->m_model[2].SetUV(Vector::Vector2(uStep * 2.0f, vStep));
	this->m_model[3].SetUV(Vector::Vector2(uStep, vStep));

	this->m_model[4].SetUV(Vector::Vector2(uStep * 2.0f, vStep * 2.0f));
	this->m_model[5].SetUV(Vector::Vector2(uStep, vStep * 2.0f));

	this->m_model[6].SetUV(Vector::Vector2(uStep * 2.0f, 1.0f));
	this->m_model[7].SetUV(Vector::Vector2(uStep, 1.0f));


	this->m_model[8].SetUV(Vector::Vector2(0.0f, vStep));
	this->m_model[9].SetUV(Vector::Vector2(uStep, vStep));
	this->m_model[10].SetUV(Vector::Vector2(0.0f, vStep * 2.0f));
	this->m_model[11].SetUV(Vector::Vector2(uStep, vStep * 2.0f));

	//4
	this->m_model[12].SetUV(Vector::Vector2(uStep * 2.0f, vStep));
	this->m_model[13].SetUV(Vector::Vector2(uStep * 3.0f, vStep));
	this->m_model[14].SetUV(Vector::Vector2(uStep * 2.0f, vStep * 2.0f));
	this->m_model[15].SetUV(Vector::Vector2(uStep * 3.0f, vStep * 2.0f));


	//5
	this->m_model[16].SetUV(Vector::Vector2(uStep * 3.0f, vStep));
	this->m_model[17].SetUV(Vector::Vector2(1.0f, vStep));
	this->m_model[18].SetUV(Vector::Vector2(uStep * 3.0f, vStep * 2.0f));
	this->m_model[19].SetUV(Vector::Vector2(1.0f, vStep * 2.0f));



	return true;
}

bool SkyBoxMesh::CreateBuffer(ID3D11Device * device)
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}



	//���� �迭 ����
	m_indexCount = 36;
	SkyBoxVertexType* vertices = new SkyBoxVertexType[m_vertexCount]; 
	unsigned long indices[36] =
	{
		0,3,2,
		1,3,0,
		2,5,4,
		3,5,2,
		4,7,6,
		5,7,4,
		10,11,8,
		8,11,9,
		14,15,12,
		12,15,13,
		16,19,17,
		16,18,19
	};

	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
	}

	delete[] m_model;
	m_model = nullptr;


	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SkyBoxVertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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
		printf("Mesh CreateVertexBuffer Erorr \n");
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
		printf("Mesh CreateIndexBuffer Erorr \n");
		return false;
	}


	delete[] vertices;
	vertices = 0;

	return true;
}

bool SkyBoxMesh::CreateShpereBox(ID3D11Device * device, float radius, int sliceCount, int stackCount)
{
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer != nullptr)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	std::vector<ModelType> vertices;
	std::vector<int> indices;
	ModelType topVertex, bottomVertex;

	topVertex.x = 0.0f;
	topVertex.y = radius;
	topVertex.z = 0.0f;

	topVertex.tu = 0.0f;
	topVertex.tv = 0.0f;

	topVertex.nx = 0.0f;
	topVertex.ny = 1.0f;
	topVertex.nz = 0.0f;


	bottomVertex.x = 0.0f;
	bottomVertex.y = -radius;
	bottomVertex.z = 0.0f;

	bottomVertex.tu = 0.0f;
	bottomVertex.tv = 1.0f;

	bottomVertex.nx = 0.0f;
	bottomVertex.ny = -1.0f;
	bottomVertex.nz = 0.0f;


	vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f * XM_PI / sliceCount;


	for (int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;


		for (int j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;
			ModelType v;


			v.x = radius * sinf(phi) * cosf(theta);
			v.y = radius * cosf(phi);
			v.z = radius * sinf(phi) * sinf(theta);

			Vector::Vector3 normal =
				Vector::Vector3::Normalize(Vector::Vector3(v.x, v.y, v.z));

			v.nx = normal.x;
			v.ny = normal.y;
			v.nz = normal.z;

			v.tu = theta / XM_2PI;
			v.tv = phi / XM_PI;

			vertices.push_back(v);
		}

	}
	vertices.push_back(bottomVertex);




	for (int i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}



	int baseIndex = 1;
	int ringVertexCount = sliceCount + 1;

	for (int i = 0; i < stackCount - 2; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);


			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}


	int southPoleIndex = (int)vertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}


	int vertexSize = (int)vertices.size();
	int indiceSize = (int)indices.size();
	//���� �迭 ����

	SkyBoxVertexType* verticesType = new SkyBoxVertexType[vertexSize];
	if (!verticesType)
	{
		printf("Mesh Vertex Erorr \n");
		return false;
	}


	unsigned long* indicesType = new unsigned long[indiceSize];
	if (!indicesType)
	{
		printf("Mesh New indices Erorr \n");
		return false;
	}


	for (int i = 0; i < vertexSize; ++i)
	{
		verticesType[i].position.Set(vertices[i].x, vertices[i].y, vertices[i].z);
		verticesType[i].texture.Set(vertices[i].tu, vertices[i].tv);

	}

	for (int i = 0; i < indiceSize; ++i)
	{
		indicesType[i] = static_cast<unsigned long>(indices[i]);
	}

	this->m_indexCount = indiceSize;



	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SkyBoxVertexType) * vertexSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = verticesType;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//���� ���� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		printf("Mesh CreateVertexBuffer Erorr \n");
		return false;
	}

	//���� �ε��� ������ ����ü�� ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indiceSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//�ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indicesType;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//�ε��� ���� ����
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		printf("Mesh CreateIndexBuffer Erorr \n");
		return false;
	}



	delete[] verticesType;
	delete[] indicesType;




	return true;
}
