#include "pch.h"
#include "Texture.h"
#include "MethodHelper.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::Inite(ID3D11Device * device, char * filename)
{
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = nullptr;
	}

	this->SetFileName(filename);

	return LoadTextrue(device);
}

bool Texture::Inite(ID3D11Device * device)
{
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = nullptr;
	}

	return LoadTextrue(device);
}

void Texture::Destroy()
{
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = nullptr;
	}
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
	return m_textureView;
}

bool Texture::ChangeTexture(ID3D11Device * device)
{
	Destroy();
	return LoadTextrue(device);
}

void Texture::SetFileName(char * filename)
{
	if (filename == nullptr)
	{
		ZeroMemory(m_FilePath, FILE_NAME_MAX);
		strcpy_s(m_FilePath, FILE_NAME_MAX, Default_Texture);
	}
	else
	{
		ZeroMemory(m_FilePath, FILE_NAME_MAX);
		strcpy_s(m_FilePath, FILE_NAME_MAX, filename);
	}
}

bool Texture::LoadTextrue(ID3D11Device * device)
{
	DirectX::CreateWICTextureFromFile(device, MethodHelper::GetWC(m_FilePath), NULL, &m_textureView);
	//DirectX::CreateDDSTextureFromFile(device, MethodHelper::GetWC(m_filename), NULL, &m_textureView);
	/*HRESULT result = DirectX::CreateDDSTextureFromFile(device, MethodHelper::GetWC(m_filename), NULL, &m_textureView);
	if (FAILED(result))
	{
		return false;
	}*/
	return true;
}

const char * Texture::GetFilePath()
{
	return this->m_FilePath;
}
