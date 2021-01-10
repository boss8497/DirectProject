#pragma once
#include"DirectXHeader.h"


class Texture
{
public:
	Texture();
	~Texture();


public:
	/*filename : nullptr = default_texture*/
	bool Inite(ID3D11Device* device, char* filename);
	bool Inite(ID3D11Device* device);
	void Destroy();



	ID3D11ShaderResourceView* GetTexture();

	bool ChangeTexture(ID3D11Device* device);
	void SetFileName(char* filename);
	bool LoadTextrue(ID3D11Device* device);

	const char* GetFilePath();
private:
	char m_FilePath[FILE_NAME_MAX];
	ID3D11ShaderResourceView* m_textureView = nullptr;
};