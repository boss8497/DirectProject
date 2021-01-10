#pragma once
#include"DirectXHeader.h"
#include"Transform.h"


class FontShader
{
private:
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

public:
	FontShader();
	~FontShader();
	FontShader(const FontShader& copy);

public:
	bool Inite(ID3D11Device * device, HWND hwnd, char* vs_filename, char* ps_filename);
	void Destroy();

	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, Transform* tr, ID3D11ShaderResourceView* texture, Vector::Vector4 color);


public:
	bool LoadShader(ID3D11Device * device, HWND hwnd);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, Transform* tr, ID3D11ShaderResourceView* texture, Vector::Vector4 color);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	void SetFileName(char* vs, char* ps);

private:
	char m_VS_Filename[FILENAME_MAX];
	char m_PS_Filename[FILENAME_MAX];

	ID3D11VertexShader * m_vertexShader = nullptr;
	ID3D11PixelShader * m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_pixelBuffer = nullptr;

	ID3D11SamplerState* m_sampleState = nullptr;


};