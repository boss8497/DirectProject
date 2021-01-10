#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"BaseObject.h"
#include"DirectionalLight.h"
#include"StructureHeader.h"
#include"RenderQueue.h"
#include"UpdateQueue.h"

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
	GameObjectManager(const GameObjectManager& copy) = delete;

public:
	bool Inite();
	void Destroy();


public:
	void UpdateObject(UpdateQueue* uQueue);

	bool AddGameObject(ID3D11Device* device, HWND hwnd, BaseObject* _obj);
	std::list<BaseObject*>::iterator ListEraseObject(BaseObject* _obj);
	void AddRootObject(BaseObject* obj);

	//루트 오브젝트였던 오브젝트가 자식으로 이동했는지 확인
	void CheckRootToChild();
	

	//오브젝트에서 자식들을 vector에담아 넘겨주는 함수
	std::vector<BaseObject*> GetObjChildQueue(BaseObject* obj);
	//오브젝트에서 자식들을 vector에담아 넘겨주는 함수 Enable False는 추가 x
	std::vector<BaseObject*> GetObjChildUpdateQueue(BaseObject* obj);
	//렌더링큐에 렌더정보가있는 오브젝트들만 큐에 담아 준다
	//오브젝트가 빈오브젝트일 수도 있으며 오브젝트 자식에 렌더 정보가 있을 수 있다.
	void SetRenderQueue(RenderQueue* queue);


	BaseObject* FindObject(HTREEITEM hitem);
	const std::list<BaseObject*>* GetGameObejctList();
private:
	void DestroyGameObjectList();
public:
	bool Material_Inite(ID3D11Device* device, HWND hwnd, BaseObject* parent);


	BaseObject* m_TargetObject = nullptr;
private:
	std::list<BaseObject*>* m_gameobj_list = nullptr;

	int m_render_count = 0;

};