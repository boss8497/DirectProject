#pragma once
#include"DirectXHeader.h"
#include"Transform.h"


class TextureShader
{
public:
	struct TextureBufferType
	{
		Vector::Vector4 color;
	};
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShader();
	~TextureShader();
	TextureShader(const TextureShader* copy) = delete;

	bool Inite(ID3D11Device * device, HWND hwnd, char* vs_filename, char* ps_filename);
	void Destroy();

public:
	bool LoadShader(ID3D11Device * device, HWND hwnd);


	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, Transform* tr, 
		TextureBufferType* textureBuffer, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	void SetFileName(char* vs, char* ps);
	void SetVSFilePath(char* vs);
	void SetPSFilePath(char* ps);
	const char* GetVSFilePath();
	const char* GetPSFilePath();

	bool ChangeVS(ID3D11Device * device);
	bool ChangePS(ID3D11Device * device);

private:
	char m_VS_Filename[FILENAME_MAX];
	char m_PS_Filename[FILENAME_MAX];

	ID3D11VertexShader * m_vertexShader = nullptr;
	ID3D11PixelShader * m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_textureBuffer = nullptr;

	ID3D11SamplerState* m_sampleState = nullptr;
};