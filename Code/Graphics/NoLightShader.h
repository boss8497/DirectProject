#pragma once
#include"Shader.h"

class NoLightShader : public Shader
{
public:
	NoLightShader();
	~NoLightShader();
	NoLightShader(const NoLightShader& copy) = delete;
public:
	bool Inite(ID3D11Device * device, HWND hwnd, const char* vs_filename, const char* ps_filename);
	bool LoadShader(ID3D11Device * device, HWND hwnd);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView * texture, Transform* tr);
};