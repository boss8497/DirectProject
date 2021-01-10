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

	//정점 배열 생성
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


	//정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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
		printf("Mesh CreateVertexBuffer Erorr \n");
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


	//정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(FbxVertexTypeVS) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = CuttingVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//정점 버퍼 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
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

	//버텍스 카운트의 값까지 읽는다
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	//버텍스 카운트를 읽는다
	fin >> m_vertexCount;

	//인덱스의 수를 정점 수와 같게 설정
	m_indexCount = m_vertexCount;



	//
	if (m_model)
	{
		delete[] m_model;
		m_model = nullptr;
	}

	//읽어 들인 정점 개수를 사용해 모델을 만듬
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}


	//데이터의 시작 부분까지 읽는다.
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
	//정점 버퍼의 단위와 오프셋을 설정

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	//렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool Mesh::FbxRender(ID3D11DeviceContext * deviceContext)
{
	//정점 버퍼의 단위와 오프셋을 설정

	unsigned int stride = sizeof(FbxVertexTypeVS);
	unsigned int offset = 0;

	//렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정
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
	//정점 배열 생성
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



	//정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = verticesType;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//정점 버퍼 생성
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		printf("Mesh CreateVertexBuffer Erorr \n");
		return false;
	}

	//정적 인덱스 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indiceSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indicesType;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//인덱스 버퍼 생성
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		printf("Mesh CreateIndexBuffer Erorr \n");
		return false;
	}



	delete[] verticesType;
	delete[] indicesType;


	

	return true;
}
