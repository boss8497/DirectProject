#pragma once
#include"Component.h"
#include"Font.h"


class Text : public Component<BaseObject>
{
public:
	Text();
	~Text();
	Text(const Text &copy);


public:
	/*파일 지정 하기 위해서는 초기화 이전에 파일경로를 설정 해줘야 된다*/
	bool Inite(ID3D11Device* device, HWND hwnd);
	void Destroy();
	void Update(UpdateQueue* uQueue);



public:
	bool Render(ID3D11DeviceContext * deviceContext, Transform * tr);
	bool Render_Mesh(ID3D11DeviceContext * deviceContext, Transform * tr);
	bool Render_Shader(ID3D11DeviceContext * deviceContext, Transform * tr);


	void SetText(char* str);
	void Set_FilePath(char * font_filepath, char * vs_filepath, char * ps_filepath);

	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
public:
	Vector::Vector4 m_color;
	char m_text[TEXT_LENGTH_MAX];


	char m_Font_FilePath[FILENAME_MAX];
	char m_VS_FilePath[FILENAME_MAX];
	char m_PS_FilePath[FILENAME_MAX];

	int Depth = 0;
private:
	Font * m_font;
	float m_FontSize = 16;
public:
	static std::map<std::string, Font*>* m_FontMap;

	static void DestroyFontMap();
};