#pragma once
#include"DirectXHeader.h"
#include"Matrix.h"
#include"UpdateQueue.h"
#include"InspecterItem.h"
#include"UpdateItem.h"

enum COMPONENT_ID
{
	DISABLE = 0,
	TRANSFORM,
	MATERIAL,
	TEXTURE2D,
	SKYBOXCOMPONENT,
	SOUND_COMPONENT
};


template<class T>
class Component
{
public:
	Component() {}
	virtual ~Component() {}

	virtual void Awake() {}
	virtual bool Inite() { return true; };
	virtual void Destroy() {};
	virtual void Update(UpdateQueue* uQueue) {};
	virtual void Update() {}


	//¿©µµøÏ ¿ŒΩ∫∆Â≈Õ ∏ «Œ
	virtual InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize)
	{

		InspacterItem* itemTmp = nullptr;
		POINT disFrameSize = { (LONG)compFrameXsize, 20 };
		std::string className = typeid(*this).name();

		//typeid.name() = class xxxx
		//class(space) = 6byte delete
		className.erase(0, 6);

		itemTmp = new InspacterItem();
		itemTmp->CreateComponentFrame(phwnd, phins, NextPos, disFrameSize, className.c_str());

		return itemTmp;

	}
public:
	int m_CID = COMPONENT_ID::DISABLE;
	T * gameobject = nullptr;
};