#pragma once
#include"Mesh.h"


class SkyBoxMesh : public Mesh
{
	struct SkyBoxVertexType : public AligendAllocationPolicy<16>
	{
	public:
		SkyBoxVertexType()
		{
			position.Set(0, 0, 0);
			texture.Set(0, 0);
		}
		Vector::Vector3 position;
		Vector::Vector2 texture;
		Vector::Vector3 normal;
	};

public:
	SkyBoxMesh();
	~SkyBoxMesh();
	SkyBoxMesh(const SkyBoxMesh& copy) = delete;
	
public:
	bool Inite(ID3D11Device* device);
	void Update(UpdateQueue* uQueue);
	bool Render(ID3D11DeviceContext * deviceContext);


	bool CreateBuffer(ID3D11Device* device);
	

	bool CreateShpereBox(ID3D11Device * device, float radius, int sliceCount, int stackCount);

	//test
	bool LoadMesh(float size);
};