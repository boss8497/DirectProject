#pragma once
#include"pch.h"
#include"Texture.h"
#include"FontMesh.h"
#include"FontShader.h"

class Font
{
private:
	struct FontType
	{
	public:
		float u;
		float v;
		float ru;
		float bv;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float xadvance;

		float sizeRate;

	public:
		void SetSizeRate(float fontRate)
		{
			sizeRate = fontRate;
		}

		float GetXoffset()
		{
			return xoffset * sizeRate;
		}

		float GetYoffset()
		{
			return yoffset * sizeRate;
		}

		float GetWithd()
		{
			return width * sizeRate;
		}

		float GetHeight()
		{
			return height * sizeRate;
		}
	};

	struct FontInfo
	{
	public:
		char name[FILENAME_MAX];
		float size;
		bool bold;
		bool italic;
		char charset[FILENAME_MAX];
		bool unicode;
		float stretchH;
		float smooth;
		float aa;
		//x Left, y Top, z Right, w Bottom
		Vector::Vector4 padding;
		Vector::Vector2 spacing;
		float outline;
	};

	struct VertexType
	{
		Vector::Vector3 position;
		Vector::Vector2 texture;
	};

public:
	Font();
	~Font();
	Font(const Font& copy);

public:
	bool Inite(ID3D11Device* device, HWND hwnd, char * filename, char* vs_filename, char* ps_filename);
	void Destroy();


	void SetFontFileName(char* filename);
	bool Render_Mesh(ID3D11DeviceContext * deviceContext, char* text, Transform* tr, float fsize);
	bool Render_Shader(ID3D11DeviceContext* deviceContext, Transform* tr, Vector::Vector4 color);
private:
	bool LoadFont();
	bool UpdateText(ID3D11DeviceContext * deviceContext, char* text, Transform* tr, float fsize);

public:
	char m_font_fileName[FILENAME_MAX];
	char m_texture_fileName[FILENAME_MAX];
	char m_VS_Filename[FILENAME_MAX];
	char m_PS_Filename[FILENAME_MAX];

	FontInfo m_FontInfo;
	float m_text_size;
	std::wstring m_previous_text;

	std::map<int, FontType> m_fontmap;

	FontMesh* m_mesh = nullptr;
	FontShader* m_shader = nullptr;
	Texture* m_texture = nullptr;
};