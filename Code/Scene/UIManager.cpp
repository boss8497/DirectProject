#include "pch.h"
#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

bool UIManager::Inite()
{
	m_ui_list = new std::list<BaseObject*>();

	return true;
}

void UIManager::Destroy()
{
	if (m_ui_list)
	{
		this->DestroyUIList();
		delete m_ui_list;
		m_ui_list = nullptr;
	}
}

void UIManager::UpdateObject(UpdateQueue* uQueue)
{
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		std::vector<BaseObject*> ChildVec = this->GetObjChildQueue((*iter));

		for (unsigned int i = 0; i < ChildVec.size(); ++i)
		{
			ChildVec[i]->ComponentUpdate(uQueue);
			ChildVec[i]->Update();
		}
	}
}

void UIManager::CheckRootToChild()
{
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		if ((*iter)->GetParent() != nullptr)
		{
			iter = this->ListEraseObject((*iter));
		}
	}
}

void UIManager::AddRootObject(BaseObject * obj)
{
	if (obj->GetParent() == nullptr)
		m_ui_list->push_back(obj);
}

std::vector<BaseObject*> UIManager::GetObjChildQueue(BaseObject * obj)
{
	std::stack<BaseObject*> stack;
	std::vector<BaseObject*> vec;
	stack.push(obj);

	while (!stack.empty())
	{
		BaseObject* curObj = stack.top();
		stack.pop();

		vec.push_back(curObj);

		int childCount = curObj->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			stack.push(curObj->GetChild(i));
		}
	}

	return vec;
}

std::vector<BaseObject*> UIManager::GetObjChildUpdateQueue(BaseObject * obj)
{
	std::stack<BaseObject*> stack;
	std::vector<BaseObject*> vec;
	stack.push(obj);

	while (!stack.empty())
	{
		BaseObject* curObj = stack.top();
		stack.pop();

		if (curObj->Enable)
			vec.push_back(curObj);

		int childCount = curObj->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			BaseObject* curObjChild = curObj->GetChild(i);
			if (curObjChild->Enable)
				stack.push(curObjChild);
		}
	}

	return vec;
}

void UIManager::SetRenderQueue(RenderQueue * queue)
{
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		/*Enable Check*/
		if (!(*iter)->Enable)
			continue;


		std::vector<BaseObject*> ChildVec = this->GetObjChildUpdateQueue((*iter));

		for (unsigned int i = 0; i < ChildVec.size(); ++i)
		{
			Text* text = ChildVec[i]->GetCompponent<Text>();
			Texture2D* texture2d = ChildVec[i]->GetCompponent<Texture2D>();

			/*메터리얼이 있는 오브젝트만*/
			/*빈 오브젝트는 렌더 대상 X*/
			if (text)
			{
				queue->m_Render2DQ.push(ChildVec[i]);
			}
			else if (texture2d)
			{
				queue->m_Render2DQ.push(ChildVec[i]);
			}
		}
	}
}

bool UIManager::AddGameObject(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject * _obj)
{
	//부모가 없으면 리스트에 추가 ( 루트 노드 ) 있으면 추가하지 않음
	//부모가 있을시 추가안하는 이유는 렌더링시 자식 오브젝트들을 다 렌더함
	//근데 부모있는(루트노드)를 추가하면 렌더가 2번 발생하는 오류가 생김
	if(_obj->GetParent() == nullptr)
		this->m_ui_list->push_back(_obj);

	if (!this->ComponentInite(device, deviceContext, hwnd, screensize, _obj))
	{
		printf("UIManager : AddGameobject Error \n");
		return false;
	}

	return true;
}

std::list<BaseObject*>::iterator UIManager::ListEraseObject(BaseObject * in_obj)
{
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		if ((*iter) == in_obj)
		{
			return m_ui_list->erase(iter);
		}
	}
	return m_ui_list->end();
}

BaseObject * UIManager::FindObject(HTREEITEM hitem)
{
	std::queue<BaseObject*> objectQ;
	std::queue<BaseObject*> childQ;
	BaseObject* current;
	BaseObject* FindObejct = nullptr;


	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		BaseObject* tmp = *iter;
		objectQ.push(tmp);
	}

	while (!objectQ.empty())
	{
		unsigned int size = objectQ.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			current = objectQ.front();
			objectQ.pop();

			if (current->Hierarchy.m_Item == hitem)
			{
				FindObejct = current;
				return FindObejct;
			}

			for (int i = 0; i < current->GetChildCount(); ++i)
			{
				childQ.push(current->GetChild(i));
			}
		}
		std::swap(objectQ, childQ);
	}


	return FindObejct;
}

bool UIManager::ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize)
{
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end(); ++iter)
	{
		Texture2D* textrue = (*iter)->GetCompponent<Texture2D>();
		Text* text = (*iter)->GetCompponent<Text>();

		if (textrue)
		{
			if (!textrue->Inite(device, deviceContext, hwnd, (*iter)->GetCompponent<Transform>(), screensize))
			{
				printf("UIManager : Texture2DInite Error %s\n", (*iter)->Name.c_str());
				return false;
			}
		}
		else if (text)
		{
			if (!text->Inite(device, hwnd))
			{
				printf("UIManager : TextInite Error %s\n", (*iter)->Name.c_str());
				return false;
			}
		}
	}
	return true;
}

bool UIManager::ComponentInite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Vector::Vector2 screensize, BaseObject * _obj)
{
	Texture2D* textrue =  _obj->GetCompponent<Texture2D>();
	Text* text = _obj->GetCompponent<Text>();

	if (textrue)
	{
		if (!textrue->Inite(device, deviceContext, hwnd, _obj->GetCompponent<Transform>(), screensize))
		{
			printf("UIManager : Texture2DInite Error %s\n", _obj->Name.c_str());
			return false;
		}
	}
	else if (text)
	{
		if (!text->Inite(device, hwnd))
		{
			printf("UIManager : TextInite Error %s\n", _obj->Name.c_str());
			return false;
		}
	}
	return true;
}

void UIManager::DestroyUIList()
{
	//리스트 순회 오브젝트 삭제
	for (auto iter = m_ui_list->begin(); iter != m_ui_list->end();)
	{
		BaseObject* tmp = *iter;
		iter = m_ui_list->erase(iter);
		tmp->Destroy();
		delete tmp;
	}
}
