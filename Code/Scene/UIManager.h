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

	//오브젝트에서 자식들을 vector에담아 넘겨주는 함수
	std::vector<BaseObject*> GetObjChildQueue(BaseObject* obj);
	//오브젝트에서 자식들을 vector에담아 넘겨주는 함수 Enable False는 추가 x
	std::vector<BaseObject*> GetObjChildUpdateQueue(BaseObject* obj);
	//렌더링큐에 렌더정보가있는 오브젝트들만 큐에 담아 준다
	//오브젝트가 빈오브젝트일 수도 있으며 오브젝트 자식에 렌더 정보가 있을 수 있다.
	void SetRenderQueue(RenderQueue* queue);

	bool AddGameObject(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject* _obj);
	std::list<BaseObject*>::iterator ListEraseObject(BaseObject* in_obj);

	/*하이라키용*/
	BaseObject* FindObject(HTREEITEM hitem);

	int m_render_count = 0;
private:
	bool ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize);
	bool ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject* _obj);
	void DestroyUIList();

private:
	std::list<BaseObject*>* m_ui_list = nullptr;

};