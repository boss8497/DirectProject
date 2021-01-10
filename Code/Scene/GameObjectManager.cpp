#include "pch.h"
#include "GameObjectManager.h"
#include "Material.h"
#include "FbxClass.h"

GameObjectManager::GameObjectManager()
{
	/*m_gameobj_list = new std::list<BaseObject*>();
	if (m_gameobj_list)
	{
		printf("BaseObjectList Dynamic Create Error\n");
		return false;
	}*/

}

GameObjectManager::~GameObjectManager()
{
}

bool GameObjectManager::Inite()
{
	m_gameobj_list = new std::list<BaseObject*>();


	return true;
}

void GameObjectManager::Destroy()
{
	if (m_gameobj_list)
	{
		CheckRootToChild();
		DestroyGameObjectList();
		delete m_gameobj_list;
		m_gameobj_list = nullptr;
	}
}

BaseObject * GameObjectManager::FindObject(HTREEITEM hitem)
{
	std::queue<BaseObject*> objectQ;
	std::queue<BaseObject*> childQ;
	BaseObject* current;
	BaseObject* FindObejct = nullptr;


	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end(); ++iter)
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

const std::list<BaseObject*>* GameObjectManager::GetGameObejctList()
{
	return m_gameobj_list;
}

void GameObjectManager::DestroyGameObjectList()
{
	//리스트 순회 오브젝트 삭제
	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end();)
	{
		BaseObject* tmp = *iter;
		if (tmp != nullptr)
		{
			iter = m_gameobj_list->erase(iter);
			tmp->Destroy();
			delete tmp;
		}
	}
}

void GameObjectManager::UpdateObject(UpdateQueue* uQueue)
{
	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end(); ++iter)
	{
		std::vector<BaseObject*> ChildVec = this->GetObjChildQueue((*iter));

		for (unsigned int i = 0; i < ChildVec.size(); ++i)
		{
			ChildVec[i]->ComponentUpdate(uQueue);
			ChildVec[i]->Update();
		}
	}
}

bool GameObjectManager::AddGameObject(ID3D11Device * device, HWND hwnd, BaseObject * _obj)
{
	//초기화전 사용시 실패
	if (m_gameobj_list == nullptr)
	{
		printf("GameObjectManager : gameobjlist Create Error\n");
		return false;
	}

	//리스트에 추가(부모 오브젝트가 있으면 추가하지 않음)
	if(_obj->GetParent() == nullptr)
		m_gameobj_list->push_back(_obj);

	//후에 머터리얼있는지 확인해서 업데이트 해줌
	if (!this->Material_Inite(device, hwnd, _obj))
	{
		printf("GameObjectManager : AddGameObject Material_Inite Error\n");
		return false;
	}

	return true;
}

std::list<BaseObject*>::iterator GameObjectManager::ListEraseObject(BaseObject * _obj)
{
	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end(); ++iter)
	{
		if ((*iter) == _obj)
		{
			return m_gameobj_list->erase(iter);
		}
	}

	return m_gameobj_list->end();
}

void GameObjectManager::AddRootObject(BaseObject * obj)
{
	if(obj->GetParent() == nullptr)
		m_gameobj_list->push_back(obj);
}

void GameObjectManager::CheckRootToChild()
{
	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end(); ++iter)
	{
		if ((*iter)->GetParent() != nullptr)
		{
			iter = this->ListEraseObject((*iter));
		}
	}
}

std::vector<BaseObject*> GameObjectManager::GetObjChildQueue(BaseObject * obj)
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

std::vector<BaseObject*> GameObjectManager::GetObjChildUpdateQueue(BaseObject * obj)
{
	std::stack<BaseObject*> stack;
	std::vector<BaseObject*> vec;
	stack.push(obj);

	while (!stack.empty())
	{
		BaseObject* curObj = stack.top();
		stack.pop();

		if(curObj->Enable)
			vec.push_back(curObj);

		int childCount = curObj->GetChildCount();
		for (int i = 0; i < childCount; ++i)
		{
			BaseObject* curObjChild = curObj->GetChild(i);
			if(curObjChild->Enable)
				stack.push(curObjChild);
		}
	}

	return vec;
}

void GameObjectManager::SetRenderQueue(RenderQueue * queue)
{
	//리스트 순회하며 메터리얼(메쉬, 쉐이더) 렌더링
	for (auto iter = m_gameobj_list->begin(); iter != m_gameobj_list->end(); ++iter)
	{
		/*Enable Check*/
		if (!(*iter)->Enable)
			continue;

		std::vector<BaseObject*> ChildVec = this->GetObjChildUpdateQueue((*iter));


		for (unsigned int i = 0; i < ChildVec.size(); ++i)
		{
			Material* mat = ChildVec[i]->GetCompponent<Material>();
			FbxClass* fbx = ChildVec[i]->GetCompponent<FbxClass>();

			if(!ChildVec[i]->Enable)
				continue;

			/*메터리얼이 있는 오브젝트만*/
			/*빈 오브젝트는 렌더 대상 X*/
			if (mat)
			{
				queue->m_Render3DQ.push(ChildVec[i]);
			}
			else if (fbx)
			{
				queue->m_Render3DQ.push(ChildVec[i]);
			}
		}
	}
}

bool GameObjectManager::Material_Inite(ID3D11Device * device, HWND hwnd, BaseObject * parent)
{
	std::vector<BaseObject*> ChildVec = this->GetObjChildQueue(parent);

	for (unsigned int i = 0; i < ChildVec.size(); ++i)
	{
		Material* mat = ChildVec[i]->GetCompponent<Material>();
		FbxClass* fbx = ChildVec[i]->GetCompponent<FbxClass>();

		if (mat == nullptr && fbx == nullptr)
			continue;

		if (mat != nullptr)
		{
			if (!mat->Inite(device, hwnd))
			{
				printf("GameOject Name : %s - Material Inite Error \n", ChildVec[i]->Name.c_str());
				return false;
			}
		}
		else if (fbx != nullptr)
		{
			//고쳐야 될 부분 파일경로 선택 문제임
			if (!fbx->Inite(ChildVec[i], "../DirectProject/Default/Model/Komi Satobori/Komi Satobori.fbx"))
			{

			}
			else
			{
				fbx->Skinning_Inite(device, hwnd);
			}
		}
	}
	return true;
}
