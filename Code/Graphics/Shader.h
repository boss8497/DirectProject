#pragma once
#include"DirectXHeader.h"
#include"Transform.h"
#include"DirectionalLight.h"
#include"FbxUtilities.h"

class Shader
{
protected:
	struct BonesMatrixBufferType
	{
		XMMATRIX bones[256];
	};

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

	struct FbxMatrixBufferType
	{
		XMMATRIX skeletonBase;
		XMMATRIX skeleont[BoneMatrixCount];
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	struct TextureBufferType
	{
		Vector::Vector4 color;
	};

public:
	Shader();
	~Shader();

	virtual bool Inite(ID3D11Device * device, HWND hwnd,const char* vs_filename, const char* ps_filename);
	bool IniteFbx(ID3D11Device * device, HWND hwnd, const char* vs_filename, const char* ps_filename);
	void Destroy();


public:
	virtual bool LoadShader(ID3D11Device * device, HWND hwnd);
	bool FbxLoadShader(ID3D11Device * device, HWND hwnd);

	virtual bool SetShaderParameters(ID3D11DeviceContext* deviceContext, Transform* tr,
		ID3D11ShaderResourceView* texture,
		DirectionalLight* light, TextureBufferType* textureBuffer);
	bool FbxSetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture,
		DirectionalLight* light, SkeletonObjPtr& skeletonPtr, std::vector<Matrix::Matrix>& matrix, TextureBufferType* textureBuffer);

	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

	void SetFileName(const char* vs, const char* ps);

protected:
	char m_VS_Filename[FILENAME_MAX];
	char m_PS_Filename[FILENAME_MAX];

	ID3D11VertexShader * m_vertexShader = nullptr;
	ID3D11PixelShader * m_pixelShader = nullptr;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11SamplerState* m_sampleState = nullptr;
	ID3D11Buffer* m_matrixBuffer = nullptr;
	ID3D11Buffer* m_lightBuffer = nullptr;
	ID3D11Buffer* m_bonesBuffer = nullptr;
	ID3D11Buffer* m_textureBuffer = nullptr;
};