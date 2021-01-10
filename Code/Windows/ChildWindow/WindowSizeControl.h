#pragma once
#include"BaseWindow.h"
#include"Input.h"

#define MIN_WINDOW_SIZE 30

class WindowSizeControl
{
public:
	WindowSizeControl();
	WindowSizeControl(const WindowSizeControl& copy) = delete;
	~WindowSizeControl();

public:
	bool Inite(HWND parent, std::vector<BaseWindow*>* child);
	void Destroy();
	void Update();

	void MouseUpdate(HINSTANCE hinstance, MouseState isclick);
	void SetWindowScailing();
	void MouseEnterEvent();
	void MouseLeaveEvent();
private:
	void IniteWindowSize();
	int FindIndex();
	void ChildWindowResize();

private:
	HWND m_Parent;
	/*Application Ŭ������ ���� �����͸� ������ ����*/
	std::vector<BaseWindow*>* m_ChildWindow = nullptr;
	std::vector<float> m_CWindowRate;

	POINT PreviousMousePos;
	int ResizeIndexNumber = -1;
	bool ScaleOn = false;
	const float Padding = 5.0f;
};