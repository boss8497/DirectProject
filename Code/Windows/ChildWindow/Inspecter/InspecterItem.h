#pragma once
#include"pch.h"
#include"BaseWindow.h"



class InspacterItem : public BaseWindow
{
public:
	InspacterItem();
	~InspacterItem();
	InspacterItem(const InspacterItem& copy);

public:
	bool Inite() override;
	void Update() override;
	void Destroy() override;

public:
	//Inpecter의 프레임
	void CreateInspecterFrame(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos );
	void SetMoveInspecterFrame(_In_ HWND phwnd, _Inout_ POINT& nextPos);

	//일반 컴포넌트 프레임
	void CreateComponentFrame(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos, _In_ POINT& cSize, _In_ const char* name);
	void SetMoveComponentFrame();

	void CreateInspecterEnable(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ bool enable, _In_ const char* name, _In_ Vector::Vector2* framesize, _Inout_ POINT& nextPos);
	void SetMoveInspecterEnable(_In_ Vector::Vector2* framesize, _Inout_ POINT& nextPos);


	void CreateInspecterEditBox(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos,
		_In_ POINT& size, _In_ int idc, _In_ int cid, _In_ const char* name, _In_ const char* editName, _In_ bool readonly);



	void CreateInspecterButton(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos,
		_In_ POINT& size, _In_ int idc, _In_ int cid, _In_ const char* name);


	void CreateInspecterColorButton(_In_ HWND phwnd, _In_ HINSTANCE phins, _Inout_ POINT& nextPos,
		_In_ POINT& size, _In_ int idc, _In_ int cid, _In_ const char* name);

	void CreateInspecterStaticText(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ RECT& rect, _In_ const char* name);
private:
	void ClearList();
public:
	InspacterItem* FindItem(int idc);

public:
	std::list<InspacterItem*> m_SubItemList;

	int m_IDC;
	int m_ComponentID;
};