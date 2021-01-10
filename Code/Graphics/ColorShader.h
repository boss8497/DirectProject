#pragma once
#include"Shader.h"


class ColorShader : public Shader
{
public:
	ColorShader();
	~ColorShader();
	ColorShader(const ColorShader& copy) = delete;

public:
	bool Inite(ID3D11Device * device, HWND hwnd, const char* vs_filename, const char* ps_filename);
	bool LoadShader(ID3D11Device * device, HWND hwnd);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, Transform* tr);
};