#include "pch.h"
#include "Font.h"
#include "MethodHelper.h"
#include "Screen.h"

Font::Font()
{
}

Font::~Font()
{
}

Font::Font(const Font & copy)
{
}

bool Font::Inite(ID3D11Device* device, HWND hwnd, char * filename, char* vs_filename, char* ps_filename)
{
	/*ÆùÆ® »çÀÌÁî ¸â¹ö ÃÊ±âÈ­ ÇÊ¿ä*/
	m_text_size = Default_Font_Size;

	this->SetFontFileName(filename);

	if (!this->LoadFont())
	{
		printf("Font : Inite - LoadFont Error\n");
		return false;
	}

	m_texture = new Texture;

	if (!m_texture->Inite(device, m_texture_fileName))
	{
		printf("Font : Inite - TextureInite Error\n");
		return false;
	}


	m_mesh = new FontMesh;
	if (!m_mesh->Inite(device))
	{
		printf("Font : Inite - FontMeshInite Error\n");
		return false;
	}

	m_shader = new FontShader;
	if (!m_shader->Inite(device, hwnd, vs_filename, ps_filename))
	{
		printf("Font : Inite - FontShader Error\n");
		return false;
	}

	return true;
}

void Font::Destroy()
{
	if (m_texture)
	{
		m_texture->Destroy();
		delete m_texture;
		m_texture = nullptr;
	}

	if (m_mesh)
	{
		m_mesh->Destroy();
		delete m_mesh;
		m_mesh = nullptr;
	}

	if (m_shader)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = nullptr;
	}
}

void Font::SetFontFileName(char * filename)
{
	if (filename == nullptr)
	{
		ZeroMemory(m_font_fileName, FILE_NAME_MAX);
		strcpy_s(m_font_fileName, FILE_NAME_MAX, Default_Font);
	}
	else
	{
		ZeroMemory(m_font_fileName, FILE_NAME_MAX);
		strcpy_s(m_font_fileName, FILE_NAME_MAX, filename);
	}
}

bool Font::Render_Mesh(ID3D11DeviceContext * deviceContext, char* text, Transform * tr, float fsize)
{
	this->UpdateText(deviceContext, text, tr, fsize);

	m_mesh->Render(deviceContext);

	return true;
}

bool Font::Render_Shader(ID3D11DeviceContext * deviceContext, Transform * tr, Vector::Vector4 color)
{
	if (!m_shader->Render(deviceContext, m_mesh->GetIndexCount(), tr, m_texture->GetTexture(), color))
	{
		printf("Font : Render_Shader Error \n");
		return false;
	}
	return true;
}

bool Font::LoadFont()
{
	std::ifstream fin;
	int tmp = 0;
	char input2;
	float bitmap_width = 0;
	float bitmap_height = 0;
	int charcount = 0;
	int unicode = 0;
	char tfilename[255];

	/*Font info Inite, File Open*/
	ZeroMemory(&m_FontInfo, sizeof(FontInfo));
	fin.open(m_font_fileName);


	/*Get Font Name*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.name;
	MethodHelper::strelement(m_FontInfo.name, '"');

	/*Get Font Size*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.size;

	/*Get Bold*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.bold;

	/*Get Italic*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.italic;

	/*Get Charset*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.charset;

	/*Get Unicode*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.unicode;


	/*Get stretchH*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.stretchH;


	/*Get smooth*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.smooth;


	/*Get aa*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.aa;


	/*Get pading*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.padding.x;
	fin.get(input2);
	fin >> m_FontInfo.padding.y;
	fin.get(input2);
	fin >> m_FontInfo.padding.z;
	fin.get(input2);
	fin >> m_FontInfo.padding.w;


	/*Get spacing*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.spacing.x;
	fin.get(input2);
	fin >> m_FontInfo.spacing.y;
	fin.get(input2);


	/*Get outline*/
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> m_FontInfo.outline;

	//////////////////////////////////////////////////

	//lineheight get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;


	//base get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;


	//scaleW get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> bitmap_width;


	//scaleH get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> bitmap_height;


	//pages get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;


	//packed get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;



	//alphaChnl get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;



	//R get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;



	//G get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;



	//B get
	fin.get(input2);
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> tmp;


	//filename get

	fin.get(input2);

	//id
	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> charcount;


	fin.get(input2);

	while (input2 != '=')
	{
		fin.get(input2);
	}
	/*ÀÌ°Å °æ·Î ¼³Á¤ ´Ù½Ã ¹Ù²æ¾ßµÊ*/
	ZeroMemory(m_texture_fileName, FILENAME_MAX);
	ZeroMemory(tfilename, 255);

	strcpy_s(m_texture_fileName, FILENAME_MAX, "../DirectProject/Font/");

	fin >> tfilename;
	MethodHelper::strelement(tfilename, '"');

	strcat_s(m_texture_fileName, FILENAME_MAX, tfilename);



	//Count get
	fin.get(input2);

	while (input2 != '=')
	{
		fin.get(input2);
	}
	fin >> charcount;







	for (int i = 0; i < charcount; ++i)
	{
		//id get À¯´ÏÄÚµå 10Áø¼ö º¯È¯ÄÚµå
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> unicode;

		//x get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].u;
		m_fontmap[unicode].u = m_fontmap[unicode].u / bitmap_width;

		//y get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].v;
		m_fontmap[unicode].v = m_fontmap[unicode].v / bitmap_height;


		//width get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].width;
		m_fontmap[unicode].ru = m_fontmap[unicode].u + m_fontmap[unicode].width / bitmap_width;


		//height get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].height;
		m_fontmap[unicode].bv = m_fontmap[unicode].v + m_fontmap[unicode].height / bitmap_height;


		//xoffset get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].xoffset;


		//yoffset get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].yoffset;


		//xadvance get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
		fin >> m_fontmap[unicode].xadvance;


		//page get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}


		//chnl get
		fin.get(input2);
		while (input2 != '=')
		{
			fin.get(input2);
		}
	}


	fin.close();
	return true;
}

bool Font::UpdateText(ID3D11DeviceContext * deviceContext, char* text, Transform* tr, float fsize)
{
	VertexType* vertices = nullptr;
	float InitX = ((Screen::Instance().Size.x / 2) * -1) + tr->Position.x;
	float InitY = (Screen::Instance().Size.y / 2) - tr->Position.y;
	float drawX = InitX;
	float drawY = InitY;
	float MinX = drawX;
	float MaxX = MinX + tr->Size.x;
	int text_length = 0;
	int index = 0;
	int code = 0;
	float offsetY = fsize;
	float offsetX = fsize;
	std::wstring now_text;
	MethodHelper::GetWC(now_text, text);

	_wsetlocale(LC_ALL, L"Korean");

	if (m_previous_text.compare(now_text) == 0)
	{
		if(m_text_size == fsize)
			return true;
	}

	m_text_size = fsize;

	m_previous_text = now_text;
	text_length = m_previous_text.length();


	vertices = new VertexType[m_mesh->GetVertexCount()];
	ZeroMemory(vertices, sizeof(VertexType)* m_mesh->GetVertexCount());

	m_previous_text = now_text;

	for (int i = 0; i < text_length; ++i)
	{
		code = (int)m_previous_text[i];

		switch (code)
		{
		case VK_SPACE:
			drawX += offsetX/2;
			break;
			// '\'n
		case 92:
		{
			int nextChar = (int)m_previous_text[i + 1];
			// 110 = n
			// \n Check
			if (nextChar == 110)
			{
				drawX = MinX;
				InitY -= m_text_size;
				++i;
				break;
			}
		}

		default:
			m_fontmap[code].SetSizeRate(m_text_size/ m_FontInfo.size);

			drawY = InitY - m_fontmap[code].GetYoffset();
			offsetY = drawY - m_fontmap[code].GetHeight();

			//Ã¹ »ï°¢Çü
			/*left top uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX, drawY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].u, m_fontmap[code].v);
			++index;


			/*right bottom uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX + m_fontmap[code].GetWithd(),offsetY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].ru, m_fontmap[code].bv);
			++index;

			/*left bottom uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX,offsetY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].u, m_fontmap[code].bv);
			++index;

			//µÎ¹øÂ° »ï°¢Çü
			/*left top uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX, drawY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].u, m_fontmap[code].v);
			++index;

			/*right top uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX + m_fontmap[code].GetWithd(), drawY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].ru, m_fontmap[code].v);
			++index;

			/*right bottom uvÁÂÇ¥*/
			vertices[index].position = Vector::Vector3(drawX + m_fontmap[code].GetWithd(),offsetY, 0.0f);
			vertices[index].texture = Vector::Vector2(m_fontmap[code].ru, m_fontmap[code].bv);
			++index;



			drawX = drawX + m_fontmap[code].GetWithd();

			if (drawX >= MaxX)
			{
				drawX = MinX;
				InitY -= m_text_size;
			}

			break;
		}
	}

	m_mesh->UpdateVertexBuffer(deviceContext, (void*)vertices);

	delete[] vertices;

	return true;
}
