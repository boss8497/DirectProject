#include "pch.h"
#include "InspecterItem.h"

InspacterItem::InspacterItem()
{
}

InspacterItem::~InspacterItem()
{
}

InspacterItem::InspacterItem(const InspacterItem & copy)
{
}

bool InspacterItem::Inite()
{
	return true;
}

void InspacterItem::Update()
{
}

void InspacterItem::Destroy()
{
	this->ClearList();
	if (this->m_hwnd)
	{
		DestroyWindow(this->m_hwnd);
	}
}

void InspacterItem::CreateInspecterFrame(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos)
{
	this->SetMoveInspecterFrame(phwnd, nextPos);
	RECT size;
	size.left = static_cast<LONG>(this->m_Position.x);
	size.top = static_cast<LONG>(this->m_Position.y);
	size.right = static_cast<LONG>(this->m_WindowSize.x);
	size.bottom = static_cast<LONG>(this->m_WindowSize.y);

	if (this->m_hwnd == NULL)
	{
		this->m_hwnd = this->CreateGroupBox(phwnd, phins, "Inspecter", IDC_ITEM::GROUP_PARENT, size);
	}

	this->m_IDC = IDC_ITEM::GROUP_PARENT;
}

void InspacterItem::SetMoveInspecterFrame(_In_ HWND phwnd, _Inout_ POINT& nextPos)
{
	RECT rect, size;
	GetClientRect(phwnd, &rect);
	nextPos.x = INFO_PADDING;
	nextPos.y = INFO_PADDING;

	size.left = nextPos.x;
	size.top = nextPos.y;

	size.right = rect.right - INFO_PADDING * 2;
	size.bottom = rect.bottom - INFO_PADDING * 2;

	this->m_Position.Set(static_cast<float>(size.left), static_cast<float>(size.top));
	this->m_WindowSize.Set(static_cast<float>(size.right), static_cast<float>(size.bottom));

	if (this->m_hwnd)
	{
		this->SetWindowMove(this->m_Position, this->m_WindowSize, Vector::Vector4());
	}
}

void InspacterItem::CreateComponentFrame(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos, _In_ POINT& cSize, _In_ const char* name)
{
	RECT size = { 0, };
	POINT winSize = { 0, };

	winSize = cSize;

	if (this->m_hwnd == NULL)
	{
		this->m_hwnd = this->CreateGroupBox(phwnd, phins, name, IDC_ITEM::GROUP_PARENT, size);
	}

	/*this->m_Position.Set(&nextPos);
	this->m_WindowSize.Set(&winSize);*/

	if (this->m_hwnd)
	{
		this->SetWindowMove(nextPos, winSize, Vector::Vector4());
	}
}

void InspacterItem::CreateInspecterEnable(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ bool enable, _In_ const char* name, _In_ Vector::Vector2* framesize, _Inout_ POINT& nextPos)
{
	RECT rect;

	this->m_hwnd = this->CreateCheckBox(phwnd, phins, name, IDC_ITEM::ENABLE_CHECKBOX, rect, false, enable);

	this->SetMoveInspecterEnable(framesize, nextPos);


	this->m_IDC = IDC_ITEM::ENABLE_CHECKBOX;
}

void InspacterItem::SetMoveInspecterEnable(_In_ Vector::Vector2* framesize, _Inout_ POINT& nextPos)
{
	Vector::Vector2 pos, size;

	nextPos.y += INFO_PADDING * 2;

	pos.Set(&nextPos);

	size.x = framesize->x - (float)INFO_PADDING * 2.0f;
	size.y = 20.0f;

	this->m_Position = pos;
	this->m_WindowSize = size;

	this->SetWindowMove(pos,size,Vector::Vector4());

	//Enable 버튼 밑으로는 이제 다 컴포넌트 정보들임 그래서 컴포넌트 y위치만 미리 잡아준다
	nextPos.y += GET_NEXT_Y_POSITION(size.y);
}

void InspacterItem::CreateInspecterEditBox(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos, _In_ POINT& size, _In_ int idc, _In_ int cid, _In_ const char* name, _In_ const char* editName, _In_ bool readonly)
{
	this->m_IDC = idc;
	this->m_ComponentID = cid;

	/*에디트 박스에는 static text가 붙는다*/
	InspacterItem* sText = new InspacterItem();
	float textSizeX = (float)size.x * 0.2f;
	float editSizeX = (float)size.x * 0.8f;
	RECT textRect = { 0, };
	POINT editPos = { 0, };
	POINT editSize = { 0, };
	textRect.left = nextPos.x;
	textRect.top = nextPos.y;
	textRect.right = static_cast<LONG>(textSizeX);
	textRect.bottom = size.y;
	

	sText->CreateInspecterStaticText(phwnd, phins, textRect, name);
	this->m_SubItemList.push_back(sText);

	//에디트박스 크기 위치
	editPos.x = textRect.right + nextPos.x;
	editPos.y = nextPos.y;

	editSize.x = (LONG)editSizeX;
	editSize.y = size.y;

	RECT editRect;
	editRect.left = editPos.x;
	editRect.top = editPos.y;
	editRect.right = editSize.x;
	editRect.bottom = editSize.y;

	this->m_hwnd = this->CreateEditBox(phwnd, phins, editName, this->m_IDC, editRect, readonly);

}

void InspacterItem::CreateInspecterButton(HWND phwnd, HINSTANCE phins, POINT & nextPos, POINT & size, int idc, int cid, const char * name)
{
	this->m_IDC = idc;
	this->m_ComponentID = cid;

	this->m_hwnd = this->CreatePushButton(phwnd, phins, name, idc, RECT(), false);
	this->SetWindowMove(nextPos, size, Vector::Vector4());
}

void InspacterItem::CreateInspecterStaticText(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ RECT& rect, _In_ const char* name)
{
	this->m_hwnd = CreateWindow(TEXT("static"), MethodHelper::GetWC(name), WS_CHILD | WS_VISIBLE,
		rect.left, rect.top, rect.right, rect.bottom, phwnd, NULL, phins, NULL);
}

void InspacterItem::ClearList()
{
	if (!this->m_SubItemList.empty())
	{
		for (auto iter = this->m_SubItemList.begin(); iter != this->m_SubItemList.end();)
		{
			InspacterItem* tmp = (*iter);
			tmp->Destroy();
			iter = this->m_SubItemList.erase(iter);
			delete tmp;
		}
	}
}

InspacterItem * InspacterItem::FindItem(int idc)
{
	InspacterItem* cItem = nullptr;

	if (!this->m_SubItemList.empty())
	{
		for (auto iter = this->m_SubItemList.begin(); iter != this->m_SubItemList.end();++iter)
		{
			InspacterItem* tmp = (*iter);
			if (tmp->m_IDC == idc)
			{
				cItem = tmp;
				break;
			}
		}
	}
	return cItem;
}
