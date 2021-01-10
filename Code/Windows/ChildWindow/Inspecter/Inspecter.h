#pragma once
#include"BaseWindow.h"
#include"InspecterItem.h"
#include"UpdateItem.h"
#include"UpdateItemSubProc.h"




/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
//이 클래스가 Inspecter의 Item생성 변경 이벤트등 모두 담당 한다
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
#define INFO_PADDING 10

class Inspecter : public BaseWindow
{
public:
	Inspecter();
	~Inspecter();


	virtual bool Inite(_In_ HWND phwnd);
	virtual void Destroy();
	virtual void Update();




public:
	bool CreateInspecter(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ BaseObject* object);
	//Event
public:
	void InspecterEvent(HWND phwnd, WPARAM wparam, LPARAM lparam);
	void InspecterEnableEvent(InspacterItem* cItem);

	void InspecterClear();
	//List Controll
private:
	void ClearItemList();
	InspacterItem* FindInspecterItem(int idc);

	void ClearUpdateItem();
	void UpdateItemList();
	UpdateItem* FindUpdateItem(int idc);

public:
	HWND m_Phwnd;


	//Test
	InspacterItem* testItem;
	InspacterItem* testEnable;

private:
	std::list<InspacterItem*> m_ItemList;
	std::list<UpdateItem*> m_UpdateItem;
	/*포인터만 받아옴*/
	BaseObject* m_currentObj = nullptr;
public:
	const BaseObject* GetCurrentObject() { return m_currentObj; }
	void SetCurrentObejct(_In_ BaseObject* obj) { m_currentObj = obj; }
};