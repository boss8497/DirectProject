#pragma once
#include"Component.h"
#include"ColorShader.h"


class GridTerrain : public Component<BaseObject>
{
private:
	struct GridVertexType
	{
		Vector::Vector3 position;
		Vector::Vector4 color;
	};


public:
	GridTerrain();
	~GridTerrain();
	GridTerrain(const GridTerrain& copy) = delete;

public:

	bool Inite(ID3D11Device* device, HWND hwnd, int width, int height);
	void Destroy();
	void Update(UpdateQueue* uQueue);


	bool CreateBuffer(ID3D11Device* device);
	bool Render(ID3D11DeviceContext * deviceContext);
	void RenderMesh(ID3D11DeviceContext * deviceContext);
public:
	int m_terrainWidth = 0;
	int m_terrainHeight = 0;
	int m_vertexCount = 0;
	int m_indexCount = 0;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;

	ColorShader* m_shader = nullptr;
};