#include "pch.h"
#include "Text.h"
#include "MethodHelper.h"

Text::Text()
{
	ZeroMemory(m_Font_FilePath, FILENAME_MAX);
	strcpy_s(m_Font_FilePath, Default_Font);
	ZeroMemory(m_VS_FilePath, FILENAME_MAX);
	strcpy_s(m_VS_FilePath, Default_Font_VS);
	ZeroMemory(m_PS_FilePath, FILENAME_MAX);
	strcpy_s(m_PS_FilePath, Default_Font_PS);
}

Text::~Text()
{
}

Text::Text(const Text & copy)
{
}

bool Text::Inite(ID3D11Device* device, HWND hwnd)
{
	std::string filePath;
	filePath = m_Font_FilePath;


	m_font = (*Text::m_FontMap)[filePath];

	if (m_font == nullptr)
	{
		m_font = new Font(); 
		if (!m_font->Inite(device, hwnd, m_Font_FilePath, m_VS_FilePath, m_PS_FilePath))
		{
			printf("Text : Font Inite error\n");
			return false;
		}
		(*Text::m_FontMap)[filePath] = m_font;
	}

	


	/*테스트*/
	m_color.Set(1, 1, 1, 1.0);
	ZeroMemory(m_text, TEXT_LENGTH_MAX);

	return true;
}

void Text::Destroy()
{
	if (m_font)
	{
		m_font = nullptr;
	}
}

void Text::Update(UpdateQueue* uQueue)
{
}

bool Text::Render(ID3D11DeviceContext * deviceContext, Transform * tr)
{

	if (!m_font->Render_Mesh(deviceContext, m_text, tr, this->m_FontSize))
	{
		printf("Text : Render_Mesh Error\n");
		return false;
	}
	if (!m_font->Render_Shader(deviceContext, tr, m_color))
	{
		printf("Text : Render_Shader Error\n");
		return false;
	}
	return true;
}

bool Text::Render_Mesh(ID3D11DeviceContext * deviceContext, Transform * tr)
{
	if (!m_font->Render_Mesh(deviceContext, m_text, tr, this->m_FontSize))
	{
		printf("Text : Render_Mesh Error\n");
		return false;
	}

	return true;
}

bool Text::Render_Shader(ID3D11DeviceContext * deviceContext, Transform * tr)
{
	if (!m_font->Render_Shader(deviceContext, tr, m_color))
	{
		printf("Text : Render_Shader Error\n");
		return false;
	}
	return true;
}

void Text::SetText(char * str)
{
	int length = std::strlen(str);
	ZeroMemory(m_text, sizeof(m_text));

	strcpy_s(m_text, str);
}

void Text::Set_FilePath(char * font_filepath, char * vs_filepath, char * ps_filepath)
{
	if (font_filepath == nullptr)
	{
		ZeroMemory(m_Font_FilePath, FILENAME_MAX);
		strcpy_s(m_Font_FilePath, Default_Font);
	}
	else
	{
		ZeroMemory(m_Font_FilePath, FILENAME_MAX);
		strcpy_s(m_Font_FilePath, font_filepath);
	}

	if (vs_filepath == nullptr)
	{
		ZeroMemory(m_VS_FilePath, FILENAME_MAX);
		strcpy_s(m_VS_FilePath, Default_Font_VS);
	}
	else
	{
		ZeroMemory(m_VS_FilePath, FILENAME_MAX);
		strcpy_s(m_VS_FilePath, vs_filepath);
	}

	if (ps_filepath == nullptr)
	{
		ZeroMemory(m_PS_FilePath, FILENAME_MAX);
		strcpy_s(m_PS_FilePath, Default_Font_PS);
	}
	else
	{
		ZeroMemory(m_PS_FilePath, FILENAME_MAX);
		strcpy_s(m_PS_FilePath, ps_filepath);
	}
}

InspacterItem * Text::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT & NextPos, float compFrameXsize)
{
	InspacterItem* TextFrame = nullptr;
	POINT TextFrameSize = { (LONG)compFrameXsize, 130 };
	POINT TextPos = NextPos;

	TextFrame = new InspacterItem();
	TextFrame->CreateComponentFrame(phwnd, phins, TextPos, TextFrameSize, "Text");



	TextPos.x += INFO_PADDING;
	TextPos.y += INFO_PADDING * 2;

	POINT TextEditPos = TextPos;
	POINT TextEditSize = { (LONG)compFrameXsize - INFO_PADDING * 2, INFO_PADDING * 2 };
	InspacterItem* TextEditItem = nullptr;
	UpdateItem* TextUpdate = nullptr;

	TextEditItem = new InspacterItem();
	TextEditItem->CreateInspecterEditBox(phwnd, phins, TextEditPos, TextEditSize,
		IDC_ITEM::EDITTMP, 0, "Text", this->m_text, false);
	TextFrame->m_SubItemList.push_back(TextEditItem);


	TextUpdate = new UpdateItem();
	TextUpdate->Inite(TextEditItem->m_hwnd, (void*)m_text, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::CSTR_YTYPE, IDC_ITEM::EDITTMP);
	upList->push_back(TextUpdate);


	TextEditPos.y += TextEditSize.y;

	//색상
	POINT ColorPos = TextEditPos;
	POINT ColorSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* ColorItem = nullptr;
	UpdateItem* ColorUpdate = nullptr;


	ColorItem = new InspacterItem();
	ColorItem->CreateInspecterButton(phwnd, phins, ColorPos, ColorSize, IDC_ITEM::BUTTONETMP2,
		0, "SelectColor");
	TextFrame->m_SubItemList.push_back(ColorItem);

	ColorUpdate = new UpdateItem();
	ColorUpdate->Inite(ColorItem->m_hwnd, (void*)&this->m_color, UpdateType::UPDATE_BUTTON_OPEN_COLOR_DIALOG,
		UpdateValueType::VECTER3_YTYPE, IDC_ITEM::BUTTONETMP2);
	upList->push_back(ColorUpdate);


	ColorPos.y += ColorSize.y;


	//뎁스
	POINT DepthPos = { ColorPos.x, ColorPos.y };
	POINT DepthSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* Depth = nullptr;
	UpdateItem* DepthUpdate = nullptr;
	char DepthChar[255];

	ZeroMemory(DepthChar, sizeof(DepthChar));
	sprintf_s(DepthChar, "%d", this->Depth);


	Depth = new InspacterItem();
	Depth->CreateInspecterEditBox(phwnd, phins, DepthPos, DepthSize,
		IDC_ITEM::VECTERTMP2_Z, 0, "Depth", DepthChar, false);
	TextFrame->m_SubItemList.push_back(Depth);


	DepthUpdate = new UpdateItem();
	DepthUpdate->Inite(Depth->m_hwnd, (void*)&this->Depth, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::INT_YTYPE, IDC_ITEM::VECTERTMP2_Z);
	upList->push_back(DepthUpdate);


	DepthPos.y += DepthSize.y;



	//폰트 사이즈
	POINT FontSizePos = DepthPos;
	POINT FontSizeSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* FontSize = nullptr;
	UpdateItem* FontSizeUpdate = nullptr;
	char FontSizeChar[255];

	ZeroMemory(FontSizeChar, sizeof(FontSizeChar));
	sprintf_s(FontSizeChar, "%f", this->m_FontSize);


	FontSize = new InspacterItem();
	FontSize->CreateInspecterEditBox(phwnd, phins, FontSizePos, FontSizeSize,
		IDC_ITEM::VECTERTMP2_X, 0, "Size", FontSizeChar, false);
	TextFrame->m_SubItemList.push_back(FontSize);


	FontSizeUpdate = new UpdateItem();
	FontSizeUpdate->Inite(FontSize->m_hwnd, (void*)&this->m_FontSize,
		UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP2_X);
	upList->push_back(FontSizeUpdate);


	FontSizePos.y += FontSizeSize.y;




	//사이즈 에디트
	POINT EditSizePos = FontSizePos;
	POINT EditSizeSize = { ((LONG)compFrameXsize - INFO_PADDING * 2) / 2,INFO_PADDING * 2 };
	InspacterItem* EditSize = nullptr;
	UpdateItem * EditSizeUpdate = nullptr;


	EditSize = new InspacterItem();
	EditSize->CreateInspecterEditBox(phwnd, phins, EditSizePos, EditSizeSize,
		IDC_ITEM::VECTERTMP3_X, 0, "X", 
		MethodHelper::FloatToCstr(this->gameobject->GetCompponent<Transform>()->Size.x), false);
	TextFrame->m_SubItemList.push_back(EditSize);


	EditSizeUpdate = new UpdateItem();
	EditSizeUpdate->Inite(EditSize->m_hwnd, (void*)&this->gameobject->GetCompponent<Transform>()->Size.x, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP3_X);
	upList->push_back(EditSizeUpdate);



	EditSizePos.x += EditSizeSize.x;

	EditSize = new InspacterItem();
	EditSize->CreateInspecterEditBox(phwnd, phins, EditSizePos, EditSizeSize,
		IDC_ITEM::VECTERTMP3_Y, 0, "Y", MethodHelper::FloatToCstr(this->gameobject->GetCompponent<Transform>()->Size.y), false);
	TextFrame->m_SubItemList.push_back(EditSize);


	EditSizeUpdate = new UpdateItem();
	EditSizeUpdate->Inite(EditSize->m_hwnd, (void*)&this->gameobject->GetCompponent<Transform>()->Size.y, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP3_Y);
	upList->push_back(EditSizeUpdate);


	return TextFrame;
}

void Text::DestroyFontMap()
{
	for (auto iter = m_FontMap->begin(); iter != m_FontMap->end();)
	{
		iter->second->Destroy();
		delete iter->second;
		iter = m_FontMap->erase(iter);
	}

	delete m_FontMap;
	m_FontMap = nullptr;
}
