#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"Screen.h"

#define INPUT_TRUE 0x80

enum MouseButton
{
	Left = 0,
	Right,
	Middle
};

class Input
{
public:
	static Input& Instance()
	{
		static Input* m_instance = new Input();
		return *m_instance;
	}

private:
	Input();
	~Input();
	Input(const Input& copy);

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	bool Inite(HINSTANCE hinstance, HWND hwnd);
	void Destroy();
	bool Update();

	bool IsEcapPressed();
	bool IsSpacePressed();
	bool IsReturenPressd();

	bool MouseButtonDown(MouseButton in_btn);
	void GetMouseMoveValue(float* inout_x, float* inout_y, float* inout_z);
	void GetMouseMoveValue(Vector::Vector3* out_value);
	Vector::Vector2 GetMouseMoveDistance();
 public:
	IDirectInput8 * m_directinput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;

	unsigned char m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;


	POINT Mouse_Position;
	Vector::Vector2* ScreenSize = nullptr;
};