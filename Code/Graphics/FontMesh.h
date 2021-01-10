#pragma once
#include "DirectXHeader.h"


class FontMesh
{
private:
	struct VertexType
	{
		Vector::Vector3 position;
		Vector::Vector2 texture;
	};

public:
	FontMesh();
	~FontMesh();
	FontMesh(const FontMesh& copy);

public:
	bool Inite(ID3D11Device* device);
	void Destroy();


	bool UpdateVertexBuffer(ID3D11DeviceContext * deviceContext, void* vertices);
	void Render(ID3D11DeviceContext * deviceContext);

	int GetVertexCount();
	int GetIndexCount();
private:
	bool IniteBuffer(ID3D11Device* device);

public:
	ID3D11Buffer * m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
	int m_charCount;
	int m_vertexCount;
	int m_indexCount;
};