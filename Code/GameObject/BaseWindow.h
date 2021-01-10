#pragma once
#include"pch.h"
#include"MethodHelper.h"


#define INFO_PADDING 10
#define GET_COMPONENTFRAME_SIZEX(frameXsize) frameXsize - (INFO_PADDING*2) 
#define GET_NEXT_Y_POSITION(sizeY) static_cast<LONG>(sizeY + INFO_PADDING)




enum IDC_ITEM
{
	GROUP_PARENT = 12400,
	TRANSFORM_PARENT,
	NAME_EDITBOX,
	ENABLE_CHECKBOX,
	ENABLE_CHECKBOX2,
	ENABLE_CHECKBOX3,
	ENABLE_CHECKBOX4,
	ENABLE_CHECKBOX5,
	ENABLE_CHECKBOX6,
	ENABLE_CHECKBOX7,
	ENABLE_CHECKBOX8,
	TRANSFORM_POS_X,
	TRANSFORM_POS_Y,
	TRANSFORM_POS_Z,
	TRANSFORM_ROT_X,
	TRANSFORM_ROT_Y,
	TRANSFORM_ROT_Z,
	VECTERTMP_X,
	VECTERTMP_Y,
	VECTERTMP_Z,
	VECTERTMP_W,
	VECTERTMP2_X,
	VECTERTMP2_Y,
	VECTERTMP2_Z,
	VECTERTMP2_W,
	VECTERTMP3_X,
	VECTERTMP3_Y,
	VECTERTMP3_Z,
	VECTERTMP3_W,
	VECTERTMP4_X,
	VECTERTMP4_Y,
	VECTERTMP4_Z,
	VECTERTMP4_W,
	VECTERTMP5_X,
	VECTERTMP5_Y,
	VECTERTMP5_Z,
	VECTERTMP5_W,
	VECTERTMP6_X,
	VECTERTMP6_Y,
	VECTERTMP6_Z,
	VECTERTMP6_W,
	VECTERTMP7_X,
	VECTERTMP7_Y,
	VECTERTMP7_Z,
	VECTERTMP7_W,
	VECTERTMP8_X,
	VECTERTMP8_Y,
	VECTERTMP8_Z,
	VECTERTMP8_W,
	EDITTMP,
	EDITTMP2,
	EDITTMP3,
	EDITTMP4,
	EDITTMP5,
	EDITTMP6,
	EDITTMP7,
	EDITTMP8,
	FILEPATH,
	FILEPATH2,
	FILEPATH3,
	FILEPATH4,
	FILEPATH5,
	FILEPATH6,
	FILEPATH7,
	FILEPATH8,
	COLOR_SELECT,
	COLOR_SELECT1,
	COLOR_SELECT2,
	COLOR_SELECT3,
	COLOR_SELECT4,
	COLOR_SELECT5,
	COLOR_SELECT6,
	COLOR_SELECT7,
	COLOR_SELECT8,
	BUTTONETMP1,
	BUTTONETMP2,
	BUTTONETMP3,
	BUTTONETMP4,
	BUTTONETMP5,
	BUTTONETMP6,
	BUTTONETMP7,
	BUTTONETMP8
};

class BaseWindow
{
public:
	BaseWindow();
	virtual ~BaseWindow();


public:
	virtual bool Inite();
	virtual void Destroy();
	virtual void Update();
	
	void SetWindowMove(Vector::Vector2 pos, Vector::Vector2 size, Vector::Vector4 padding);
	void SetWindowMove(_In_ POINT& pos, _In_ POINT& size, Vector::Vector4 padding);
	void SetWindowMove();

	void SetWindowPosition(Vector::Vector2 pos);
	void SetWindowSize(Vector::Vector2 size);
	void SetWindowPadding(Vector::Vector4 padding);

protected:
	HWND CreateGroupBox(_In_ HWND hwnd, _In_ HINSTANCE hins, _In_ const char* name, _In_ int idc, _In_ RECT& size);
	HWND CreatePushButton(_In_ HWND hwnd, _In_ HINSTANCE hins, _In_ const char* name, _In_ int idc, _In_ RECT& size, _In_ bool group);
	HWND CreateEditBox(_In_ HWND hwnd, _In_ HINSTANCE hins, _In_ const char* name, _In_ int idc, _In_ RECT& size, _In_ bool readonly);
	HWND CreateCheckBox(_In_ HWND hwnd, _In_ HINSTANCE hins, _In_ const char* name, _In_ int idc, _In_ RECT& size, _In_ bool group, _In_ bool check);

public:
	HINSTANCE			m_hinstance;
	HWND				m_hwnd = NULL;

	Vector::Vector2 m_Position;
	Vector::Vector2 m_WindowSize;
	/*Left, Top, Right, Bottom ¼ø¼­*/
	Vector::Vector4 m_Padding;
};