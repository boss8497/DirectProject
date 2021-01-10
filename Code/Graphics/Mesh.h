#pragma once
#include"Component.h"
#include"FbxUtilities.h"

enum MESH_TYPE
{
	MESH_RECT_TYPE = 0,
	MESH_SPHERE_TYPE,
	MESH_SKYBOX_TYPE
};


class Mesh : public AligendAllocationPolicy<16>
{
protected:
	struct ModelType : public AligendAllocationPolicy<16>
	{
		ModelType()
		{
			x = y = z = tu = tv = nx = ny = nz = 0.0f;
		}
		ModelType(float _x, float _y, float _z,
			float _nx, float _ny, float _nz,
			float _u, float _v)
		{
			x = _x;
			y = _y;
			z = _y;
			nx = _nx;
			ny = _ny;
			nz = _nz;
			tu = _u;
			tv = _v;
		}

		void SetPosition(Vector::Vector3 v)
		{
			x = v.x; y = v.y; z = v.z;
		}

		void SetNormal(Vector::Vector3 v)
		{
			nx = v.x; ny = v.y; nz = v.z;
		}

		void SetUV(Vector::Vector2 v)
		{
			tu = v.x; tv = v.y;
		}


		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};


public:
	Mesh();
	virtual ~Mesh();

public:
	virtual bool Inite(ID3D11Device* device, char* filename);
	virtual void Update(UpdateQueue* uQueue);
	virtual void Destroy();
	virtual bool Render(ID3D11DeviceContext * deviceContext);
	virtual bool CreateBuffer(ID3D11Device* device);


	bool FbxInite(ID3D11Device* device, FbxVertexTypeVS* CuttingVertices, int CuttingVertexSize,
		Indices* CuttingIndices);
	bool FbxRender(ID3D11DeviceContext * deviceContext);
	bool FbxCreateBuffer(ID3D11Device* device, FbxVertexTypeVS* CuttingVertices, int CuttingVertexSize,
		Indices* CuttingIndices);
public:
	void SetFilename(char* filename);
	int GetIndexCount();


	bool LoadCubeMesh();
	bool CreateSphere(ID3D11Device * device, float radius, int sliceCount, int stackCount);
protected:
	char m_filename[FILE_NAME_MAX];
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ModelType* m_model = nullptr;
	int m_vertexCount = 0;
	int m_indexCount = 0;
};