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

	//��Ʈ ������Ʈ���� ������Ʈ�� �ڽ����� �̵��ߴ��� Ȯ��
	void CheckRootToChild();
	

	//������Ʈ���� �ڽĵ��� vector����� �Ѱ��ִ� �Լ�
	std::vector<BaseObject*> GetObjChildQueue(BaseObject* obj);
	//������Ʈ���� �ڽĵ��� vector����� �Ѱ��ִ� �Լ� Enable False�� �߰� x
	std::vector<BaseObject*> GetObjChildUpdateQueue(BaseObject* obj);
	//������ť�� �����������ִ� ������Ʈ�鸸 ť�� ��� �ش�
	//������Ʈ�� �������Ʈ�� ���� ������ ������Ʈ �ڽĿ� ���� ������ ���� �� �ִ�.
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