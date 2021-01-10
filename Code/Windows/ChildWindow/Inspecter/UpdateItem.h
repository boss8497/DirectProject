#pragma once
#include"pch.h"

//출처: https://fattarzan.tistory.com/entry/TIP-COLORREF-및-COLORREF에서-RGB-추출
#define GetColorR( rgb )    ( (BYTE)(rgb) )
#define GetColorG( rgb )    ( (BYTE)(((WORD)(rgb))>>8) )
#define GetColorB( rgb )    ( (BYTE)((rgb) >> 16 ) )
#define TEXT_SIZE 255


enum UpdateValueType
{
	INT_YTYPE,
	FLOAT_YTYPE,
	LONG_YTYPE,
	DOUBLE_YTYPE,
	CHAR_YTYPE,
	CSTR_YTYPE,
	STRING_YTYPE,
	WSTRING_YTYPE,
	HWND_YTYPE,
	HWND_WINDOWGETTEXT_YTYPE,
	VECTER3_YTYPE,
	BASEOBJECT_YTYPE,
	COMPONENET_PTR_YTYPE
};

enum UpdateType
{
	UPDATE_TEXT_EDIT,
	UPDATE_EDITBOX_NAME,
	UPDATE_BUTTON_OPEN_DIALOG,
	UPDATE_BUTTON_OPEN_FILEPATH_DIALOG,
	UPDATE_BUTTON_OPEN_FILEPATH_SOUND_DIALOG,
	UPDATE_BUTTON_OPEN_COLOR_DIALOG,
	UPDATE_FILEPATH_EDIT,
	UPDATE_ENABLE_CHECK_BOX,
	UPDATE_SOUND_STOP_BUTTON,
	UPDATE_SOUND_PLAY_BUTTON
};

class UpdateItem
{
public:
	UpdateItem();
	~UpdateItem();


public:
	bool Inite(_In_ HWND hwnd, _In_ void* uptr, _In_ UpdateType type, _In_ UpdateValueType valuetype, _In_ int idc);
	void Update();
	void Destroy();

public:
	void SetNowText();
	void InspecterEditBoxUpdate();



	bool UpdateButtonEvent();
public:

	/*다 할당없이 포인터만 받아서 씀*/
	HWND m_Currhwnd;
	void* m_UpdatePointer;
	wchar_t m_PreviousText[TEXT_SIZE];
	wchar_t m_NowText[TEXT_SIZE];
	UpdateValueType m_ValueType;
	UpdateType m_Type;
	int m_IDC;
};