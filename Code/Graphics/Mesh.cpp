#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

bool Mesh::Inite(ID3D11Device* device, char* filename)
{
	this->SetFilename(filename);

	if (!LoadCubeMesh())
	{
		printf("LoadMesh Erorr \n");
		return false;
	}

	if (!CreateBuffer(device))
	{
		printf("CreateBuffer Erorr \n");
		return false;
	}

	//this->CreateSphere(device, 1.0f, 20, 20);

	return true;
}

bool Mesh::FbxInite(ID3D11Device * device, FbxVertexTypeVS * CuttingVertices, int CuttingVertexSize, Indices * CuttingIndices)
{
	if (!FbxCreateBuffer(device, CuttingVertices, CuttingVertexSize, CuttingIndices))
	{
		printf("Mesh::FbxInite : FbxCreateBuffer Error\n");
		return false;
	}

	return true;
}

void Mesh::Update(UpdateQueue* uQueue)
{
}

void Mesh::Destroy()
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


	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}
}

void Mesh::SetFilename(char * filename)
{
	if (filename == nullptr)
	{
		ZeroMemory(m_filename, FILE_NAME_MAX);
		strcpy_s(m_filename, FILE_NAME_MAX, Default_Mesh);
	}
	else
	{
		ZeroMemory(m_filename, FILE_NAME_MAX);
		strcpy_s(m_filename, FILE_NAME_MAX, filename);
	}
}

bool Mesh::CreateBuffer(ID3D11Device * device)
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
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		printf("Mesh Vertex Erorr \n");
		return false;
	}


	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		printf("Mesh New indices Erorr \n");
		return false;
	}


	for (int i = 0; i < m_vertexCount; ++i)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}


	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
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

	delete[] indices;
	indices = 0;


	return true;
}

bool Mesh::FbxCreateBuffer(ID3D11Device* device, FbxVertexTypeVS* CuttingVertices, int CuttingVertexSize,
	Indices* CuttingIndices)
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

	m_vertexCount = CuttingVertexSize;
	m_indexCount = CuttingIndices->m_indices.size();
	unsigned long* indices = new unsigned long[m_indexCount];
	for (int i = 0; i < m_indexCount; ++i)
	{
		indices[i] = CuttingIndices->m_indices[i];
	}


	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(FbxVertexTypeVS) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = CuttingVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//���� ���� ����
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
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

	delete[] indices;
	indices = 0;
	return true;
}

bool Mesh::LoadCubeMesh()
{
	std::ifstream fin;
	fin.open(m_filename);

	if (fin.fail())
	{
		return false;
	}

	//���ؽ� ī��Ʈ�� ������ �д´�
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//���ؽ� ī��Ʈ�� �д´�
	fin >> m_vertexCount;

	//�ε����� ���� ���� ���� ���� ����
	m_indexCount = m_vertexCount;



	//
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}

	//�о� ���� ���� ������ ����� ���� ����
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}


	//�������� ���� �κб��� �д´�.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);


	for (int i = 0; i < m_vertexCount; ++i)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	fin.close();
	return true;
}

bool Mesh::Render(ID3D11DeviceContext * deviceContext)
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

bool Mesh::FbxRender(ID3D11DeviceContext * deviceContext)
{
	//���� ������ ������ �������� ����

	unsigned int stride = sizeof(FbxVertexTypeVS);
	unsigned int offset = 0;

	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ��
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ ����
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

int Mesh::GetIndexCount()
{
	return this->m_indexCount;
}

bool Mesh::CreateSphere(ID3D11Device * device, float radius, int sliceCount, int stackCount)
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
		indices.push_back(i+1);
		indices.push_back(i);
	}



	int baseIndex = 1;
	int ringVertexCount = sliceCount + 1;

	for (int i = 0; i < stackCount - 2; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i*ringVertexCount+j);
			indices.push_back(baseIndex + i*ringVertexCount+j+1);
			indices.push_back(baseIndex + (i+1)*ringVertexCount+j);


			indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j+1);
			indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}


	int southPoleIndex = (int)vertices.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex+i);
		indices.push_back(baseIndex+i+1);
	}


	int vertexSize = (int)vertices.size();
	int indiceSize = (int)indices.size();
	//���� �迭 ����
	VertexType* verticesType = new VertexType[vertexSize];
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
		verticesType[i].normal.Set(vertices[i].nx, vertices[i].ny, vertices[i].nz);
		verticesType[i].position.Set(vertices[i].x, vertices[i].y, vertices[i].z);
		verticesType[i].texture.Set(vertices[i].tu, vertices[i].tv);

	}

	for (int i = 0; i < indiceSize; ++i)
	{
		indicesType[i] = static_cast<unsigned long>( indices[i]);
	}

	this->m_indexCount = indiceSize;



	//���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexSize;
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
