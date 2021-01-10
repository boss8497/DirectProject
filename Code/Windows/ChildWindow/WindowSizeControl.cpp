#include "pch.h"
#include "WindowSizeControl.h"

WindowSizeControl::WindowSizeControl()
{
}

WindowSizeControl::~WindowSizeControl()
{
}

bool WindowSizeControl::Inite(HWND parent, std::vector<BaseWindow*>* child)
{
	this->m_Parent = parent;
	this->m_ChildWindow = child;

	this->IniteWindowSize();

	return true;
}

void WindowSizeControl::Destroy()
{
	m_Parent = nullptr;
	m_ChildWindow = nullptr;
}

void WindowSizeControl::Update()
{
}

void WindowSizeControl::MouseUpdate(HINSTANCE hinstance, MouseState mosueState)
{
	POINT mouse;
	/*최상위 윈도우의 표면(작업 영역)은 사이즈 늘리고 줄이는 곳에만 마우스가 배치 되기떄문에*/
	/*무조건 마우스가 위에 올라왔을때 크기 마우스로 바꿔주면 된다*/
	SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	GetCursorPos(&mouse);
	ScreenToClient(m_Parent, &mouse);

	switch (mosueState)
	{
	case MouseState::LB_DOWN:
		if (!ScaleOn)
		{
			this->ResizeIndexNumber = this->FindIndex();
			if (this->ResizeIndexNumber == -1)
			{
				printf("Error Resize System : WindowSizeControl::MouseUpdate\n");
			}
			else
			{
				this->MouseEnterEvent();
				ScaleOn = true;
				PreviousMousePos = mouse;
			}
		}
		break;


	case MouseState::LB_UP:
		if (ScaleOn)
		{
			ScaleOn = false;
			printf("ScaleOn : false \n");
		}
		break;


	case MouseState::RB_DOWN:
		break;


	case MouseState::RB_UP:
		break;


	case MouseState::NONE:
		if (ScaleOn)
		{
			this->ChildWindowResize();
		}
		break;

	default:
		break;
	}



	/*GetCursorPos(&mouse);
	ScreenToClient(m_Parent, &mouse);
	printf("x : %ld, y : %ld\n", mouse.x, mouse.y);*/

}

void WindowSizeControl::IniteWindowSize()
{
	if (m_ChildWindow == nullptr)
		return;

	int WindowCounnt = static_cast<int>(m_ChildWindow->size());
	RECT rt;
	Vector::Vector2 pos, size;
	GetClientRect(m_Parent, &rt);
	size.x = static_cast<float>(rt.right / WindowCounnt);
	size.y = static_cast<float>(rt.bottom);

	for (int i = 0; i < WindowCounnt; ++i)
	{
		if (i == 0)
		{
			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(0, 0, Padding, 0));
		}
		else if (i == WindowCounnt - 1)
		{
			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(Padding, 0, 0, 0));
		}
		else
		{
			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(Padding, 0, Padding, 0));
		}

		pos.x = pos.x + size.x;
	}


	for (int i = 0; i < WindowCounnt; ++i)
	{
		m_CWindowRate.push_back((*m_ChildWindow)[i]->m_WindowSize.x / rt.right);
	}
}

int WindowSizeControl::FindIndex()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(m_Parent, &mouse);

	int currentIndex = -1;
	float left, right;
	int WindowCounnt = static_cast<int>(m_ChildWindow->size());
	left = right = 0;

	for (int index = 0; index < WindowCounnt - 1; ++index)
	{
		left = (*m_ChildWindow)[index]->m_Position.x;
		right = (*m_ChildWindow)[index + 1]->m_Position.x + (*m_ChildWindow)[index + 1]->m_WindowSize.x - Padding;

		if (mouse.x > left && mouse.x < right)
			return index;
	}

	return currentIndex;
}

void WindowSizeControl::ChildWindowResize()
{
	POINT mouse;
	Vector::Vector2 MouseDistance = Input::Instance().GetMouseMoveDistance();
	RECT rt;
	GetClientRect(m_Parent, &rt);
	GetCursorPos(&mouse);
	ScreenToClient(m_Parent, &mouse);

	MouseDistance.x = static_cast<float>(mouse.x) - PreviousMousePos.x;

	float rightXsize = (*m_ChildWindow)[ResizeIndexNumber + 1]->m_WindowSize.x + MouseDistance.x * -1;
	float leftXsize = (*m_ChildWindow)[ResizeIndexNumber]->m_WindowSize.x + MouseDistance.x;
	bool MinSizeflag = false;

	if (rightXsize < MIN_WINDOW_SIZE)
	{
		rightXsize = MIN_WINDOW_SIZE;
		MinSizeflag = true;
	}
	if (leftXsize < MIN_WINDOW_SIZE)
	{
		leftXsize = MIN_WINDOW_SIZE;
		MinSizeflag = true;
	}

	if (!MinSizeflag)
	{
		m_CWindowRate[ResizeIndexNumber] = leftXsize / rt.right;
		m_CWindowRate[ResizeIndexNumber + 1] = rightXsize / rt.right;
	}

	PreviousMousePos = mouse;
	this->SetWindowScailing();
}

void WindowSizeControl::SetWindowScailing()
{
	if (m_ChildWindow == nullptr)
		return;

	RECT rt;
	int VecSize = static_cast<int>(m_ChildWindow->size());
	Vector::Vector2 pos, size;
	GetClientRect(m_Parent, &rt);



	for (int i = 0; i < VecSize; ++i)
	{
		size.x = static_cast<float>(rt.right * m_CWindowRate[i]);

		if (i == 0)
		{
			size.y = static_cast<float>(rt.bottom);

			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(0, 0, Padding, 0));
		}
		else if (i == VecSize - 1)
		{
			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(Padding, 0, 0, 0));
		}
		else
		{
			(*m_ChildWindow)[i]->SetWindowMove(pos, size, Vector::Vector4(Padding, 0, Padding, 0));
		}

		pos.x = pos.x + size.x;
	}
}

void WindowSizeControl::MouseEnterEvent()
{
	TRACKMOUSEEVENT tMouse;
	tMouse.cbSize = sizeof(TRACKMOUSEEVENT);
	tMouse.dwFlags = TME_LEAVE;
	tMouse.hwndTrack = this->m_Parent;
	tMouse.dwHoverTime = 10;
	TrackMouseEvent(&tMouse);
}

void WindowSizeControl::MouseLeaveEvent()
{
	if (ScaleOn)
	{
		ScaleOn = false;
		ResizeIndexNumber = -1;
	}
}
