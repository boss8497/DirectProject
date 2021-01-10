#pragma once
#include"BaseObject.h"
#include"Component.h"
#include"Text.h"
#include"Texture2D.h"
#include"RenderQueue.h"
#include"UpdateQueue.h"

class UIManager
{
public:
	UIManager();
	~UIManager();
	UIManager(const UIManager& copy) = delete;

public:
	bool Inite();
	void Destroy();


public:
	void UpdateObject(UpdateQueue* uQueue);
	void CheckRootToChild();
	void AddRootObject(BaseObject* obj);

	//������Ʈ���� �ڽĵ��� vector����� �Ѱ��ִ� �Լ�
	std::vector<BaseObject*> GetObjChildQueue(BaseObject* obj);
	//������Ʈ���� �ڽĵ��� vector����� �Ѱ��ִ� �Լ� Enable False�� �߰� x
	std::vector<BaseObject*> GetObjChildUpdateQueue(BaseObject* obj);
	//������ť�� �����������ִ� ������Ʈ�鸸 ť�� ��� �ش�
	//������Ʈ�� �������Ʈ�� ���� ������ ������Ʈ �ڽĿ� ���� ������ ���� �� �ִ�.
	void SetRenderQueue(RenderQueue* queue);

	bool AddGameObject(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject* _obj);
	std::list<BaseObject*>::iterator ListEraseObject(BaseObject* in_obj);

	/*���̶�Ű��*/
	BaseObject* FindObject(HTREEITEM hitem);

	int m_render_count = 0;
private:
	bool ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize);
	bool ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject* _obj);
	void DestroyUIList();

private:
	std::list<BaseObject*>* m_ui_list = nullptr;

};