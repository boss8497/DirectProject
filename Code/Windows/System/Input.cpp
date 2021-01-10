#include "pch.h"
#include "Input.h"
#include "Screen.h"


Input::Input()
{
}

Input::~Input()
{
	this->Destroy();
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	//키보드 장치를 읽기
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		//포커스를 잃거나 가져오지 못하면 다시 가져오기
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	//망우스 장치를 읽기
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		//포커스를 잃거나 가져오지 못하면 다시 가져오기
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Input::ProcessInput()
{

}

bool Input::Inite(HINSTANCE hinstance, HWND hwnd)
{
	HRESULT result;

	ScreenSize = &(Screen::Instance().Size);


	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION
		, IID_IDirectInput8, (void**)&m_directinput, NULL);
	if (FAILED(result))
	{
		printf("Input : Inite Error\n");
		return false;
	}


	/*키보드 장치 생성*/
	result = m_directinput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		printf("Input : CreateDevice Keyboard Error\n");
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard); 
	if (FAILED(result))
	{
		printf("Input : KeyboardSetDataFormat Error\n");
		return false;
	}

	/*키보드 작동수준 설정*/
	/*DISCL_EXCLUSIVE : 다른 프로그램과 키보드 공유하지 않는다*/
	//result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		printf("Input : Keyboard SetCooperativeLevel Error\n");
		return false;
	}

	/*설정한 대로 키보드 할당 받기*/
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		printf("Input : Keyboard Acquire Error\n");
		return false;
	}


	/*마우스 장치 생성*/
	result = m_directinput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		printf("Input : CreateDevice Mouse Error\n");
		return false;
	}

	/*미리 정의된 마우스 형식 받기*/
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		printf("Input : Mouse SetDataFormat Error\n");
		return false;
	}

	/*다른 프로그램과 공유하는 마우스 수준 설정*/
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		printf("Input : Mouse SetCooperativeLevel Error\n");
		return false;
	}

	/*마우스 할당 받기*/
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		printf("Input : Mouse Acquire Error\n");
		return false;
	}

	return true;
}

void Input::Destroy()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	if (m_directinput)
	{
		m_directinput->Release();
		m_directinput = nullptr;
	}
}

bool Input::Update()
{
	//키보드 상태 읽기
	if (!this->ReadKeyboard())
	{
		return false;
	}

	//마웅스 상태 읽기
	if (!this->ReadMouse())
	{
		return false;
	}

	this->ProcessInput();


	return true;
}

bool Input::IsEcapPressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsSpacePressed()
{
	if (m_keyboardState[DIK_SPACE] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::IsReturenPressd()
{
	if (m_keyboardState[DIK_RETURN] & 0x80)
	{
		return true;
	}
	return false;
}

bool Input::MouseButtonDown(MouseButton in_btn)
{
	bool flag = false;
	int index = static_cast<int>(in_btn);
	switch (in_btn)
	{
	case Left:
		if (m_mouseState.rgbButtons[index] && INPUT_TRUE)
		{
			flag = true;
		}
		break;
	case Right:
		if (m_mouseState.rgbButtons[index] && INPUT_TRUE)
		{
			flag = true;
		}
		break;
	case Middle:
		if (m_mouseState.rgbButtons[index] && INPUT_TRUE)
		{
			flag = true;
		}
		break;
	}

	return flag;
}

void Input::GetMouseMoveValue(float * inout_x, float * inout_y, float* inout_z)
{
	*inout_x = static_cast<float>( m_mouseState.lX);
	*inout_y = static_cast<float>(m_mouseState.lY);
	*inout_z = static_cast<float>(m_mouseState.lZ);
}

void Input::GetMouseMoveValue(Vector::Vector3 * out_value)
{
	out_value->x = static_cast<float>(m_mouseState.lX);
	out_value->y = static_cast<float>(m_mouseState.lY);
	out_value->z = static_cast<float>(m_mouseState.lZ);
}

Vector::Vector2 Input::GetMouseMoveDistance()
{
	return Vector::Vector2(static_cast<float>(m_mouseState.lX), static_cast<float>(m_mouseState.lY));
}
