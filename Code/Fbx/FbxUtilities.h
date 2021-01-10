#pragma once
#include"pch.h"
#include"fbxsdk.h"
#include"BaseObject.h"

struct Keyframe
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	Keyframe* mNext;

	Keyframe() :
		mNext(nullptr)
	{}
};

struct Triangle
{
	std::vector<unsigned int> mIndices;
	std::string mMaterialName;
	unsigned int mMaterialIndex;

	bool operator<(const Triangle& rhs)
	{
		return mMaterialIndex < rhs.mMaterialIndex;
	}
	void Clear()
	{
		mIndices.clear();
	}
};																																	

struct VertexType
{
	Vector::Vector3 position;
	Vector::Vector2 texture;
	Vector::Vector3 normal;
};

struct FbxVertexType
{
	VertexType m_Vertice;
	int m_Skeleton_index[4];
	double m_BlendingWeight[4];
	int m_index;

	FbxVertexType()
	{
		for (int i = 0; i < 4; ++i)
		{
			m_Skeleton_index[i] = -1;
			m_BlendingWeight[i] = 0.0f;
		}
		m_index = 0;
	}

	FbxVertexType(const FbxVertexType& copy)
	{
		m_Vertice = copy.m_Vertice;
		for (int i = 0; i < 4; ++i)
		{
			m_Skeleton_index[i] = copy.m_Skeleton_index[i];
			m_BlendingWeight[i] = copy.m_BlendingWeight[i];
		}
		m_index = copy.m_index;
	}
};

struct FbxVertexTypeVS
{
	Vector::Vector3 position;
	Vector::Vector2 texture;
	Vector::Vector3 normal;
	Vector::Vector4 weight;
	int index[4];
	
	FbxVertexTypeVS()
	{
		for (int i = 0; i < 4; ++i)
		{
			index[i] = -1;
		}
	}
};

struct Indices
{
public:
	std::vector<unsigned long> m_indices;
};

struct NodeData
{
public:
	NodeData()
	{
		m_node = nullptr; m_obj = nullptr; m_parent = nullptr;
	}
	NodeData(FbxNode * node, BaseObject* obj, std::string name)
	{
		m_node = node; m_obj = obj; m_name = name;
		m_parent = nullptr;
	}
	~NodeData()
	{
	}

	void Destroy()
	{
		Destroy_Child();
		ReleaseParent();
	}
	void Destroy_Child()
	{
		std::stack<NodeData*> ObjectStack;
		NodeData* child = nullptr;

		if (this->GetChildCount() > 0)
		{
			for (int i = 0; i < this->GetChildCount(); ++i)
			{
				ObjectStack.push(m_child[i]);
			}
			/*first seed*/
			child = ObjectStack.top();
			ObjectStack.pop();

			while (true)
			{

				if (child->GetChildCount() > 0)
				{
					ObjectStack.push(child);

					for (int i = 0; i < child->GetChildCount(); ++i)
					{
						ObjectStack.push(child->GetChild(i));
					}
				}
				else
				{
					delete child;
					child = nullptr;
				}

				if (ObjectStack.empty())
					break;

				child = ObjectStack.top();
				ObjectStack.pop();
			}
		}
	}
	void ReleaseParent()
	{
		if (m_parent != nullptr)
		{
			m_parent->ReleaseChild(this);
		}
	}
	void ReleaseChild(NodeData * child)
	{

		for (auto it = m_child.begin(); it != m_child.end(); ++it)
		{
			if ((*it) == child)
			{
				m_child.erase(it);
				break;
			}
		}
	}
public:

	void AddChild(NodeData* child)
	{
		m_child.push_back(child);
		child->SetParent(this);
	}
	NodeData* GetChild(int index)
	{
		if (index >= GetChildCount())
		{
			return nullptr;
		}
		return m_child[index];
	}
	int GetChildCount()
	{
		return m_child.size();
	}
	void SetParent(NodeData* parent)
	{
		m_parent = parent;
	}
	void SetName(std::string name)
	{
		m_name = name;
	}
	NodeData* FindChild(std::string name)
	{
		std::stack<NodeData*> ObjectStack;

		/*트리 순회*/
		for (int i = 0; i < this->GetChildCount(); ++i)
		{
			NodeData* child = m_child[i];

			while (true)
			{
				if (child->m_name.compare(name) == 0)
				{
					return child;
				}

				for (int j = 0; j < child->GetChildCount(); ++j)
				{
					ObjectStack.push(child->GetChild(j));
				}


				if (ObjectStack.empty())
					break;

				child = ObjectStack.top();
				ObjectStack.pop();
			}

		}

		return nullptr;
	}


	/*Teesst*/
	void PrintChild()
	{
		for (int i = 0; i < GetChildCount(); ++i)
		{
			PrintName(GetChild(i), 0);
		}
	}
	void PrintName(NodeData* obj, int depth)
	{
		std::string str;
		for (int i = 0; i < depth; i++)
		{
			str += "     ";
		}

		str += obj->m_name;
		printf("%s : ", str.c_str());



		for (int i = 0; i < obj->GetChildCount(); i++)
		{
			PrintName(obj->GetChild(i), depth + 1);
		}
	}
public:
	std::string m_name;

	FbxNode * m_node;
	BaseObject* m_obj;

	NodeData* m_parent;
	std::vector<NodeData*> m_child;

	/*계속 사용될 데이터*/
	std::vector<NodeData*> m_skeleton;
};

struct SkeletonNode
{
	SkeletonNode(NodeData* node)
	{
		m_node = node;
		m_setBindpose = false;
	}
	~SkeletonNode()
	{
		m_node = nullptr;
		m_Animation = nullptr;
	}
	bool m_setBindpose;

	NodeData* m_node;
	Keyframe* m_Animation;
	FbxAMatrix m_GlobalBindposeInverse;

	FbxLongLong m_AnimationLength;
	std::string m_AnimationName;
};

struct MaterialInfo
{
	std::string m_Texture_FilePath;
};

struct SkeletonObjPtr
{
	std::vector<BaseObject*> m_objList;
	SkeletonObjPtr()
	{

	}
	SkeletonObjPtr(const SkeletonObjPtr& copy)
	{
		for (unsigned int i = 0; i < copy.m_objList.size(); ++i)
		{
			BaseObject* tmp = copy.m_objList[i];
			this->m_objList.push_back(tmp);
		}
	}
};

class FbxMeshData
{
public:
	FbxMeshData(NodeData* root)
	{
		m_rootNode = root;
		this->Createskeleton();
	}
	~FbxMeshData()
	{
	}

public:
	void Destroy()
	{
		this->DestroySkeletonNode();
		/*if (m_rootNode)
		{
			m_rootNode->Destroy();
			delete m_rootNode;
			m_rootNode = nullptr;
		}*/
	}
	void DestroySkeletonNode()
	{
		for (auto it = m_skeleton.begin(); it != m_skeleton.end();)
		{
			SkeletonNode* tmp = (*it);
			it = m_skeleton.erase(it);
			delete tmp;
		}
	}


public:
	void Createskeleton()
	{
		std::stack<NodeData*> ObjectStack;
		SkeletonNode* skeleton = nullptr;

		/*트리 순회*/
		for (int i = 0; i < m_rootNode->GetChildCount(); ++i)
		{
			NodeData* child = m_rootNode->m_child[i];

			while (true)
			{
				FbxNodeAttribute::EType lAttributeType;
				lAttributeType = (child->m_node->GetNodeAttribute()->GetAttributeType());

				switch (lAttributeType)
				{
				default:
					break;

				case FbxNodeAttribute::eSkeleton:
					skeleton = new SkeletonNode(child);
					m_skeleton.push_back(skeleton);
					break;
				}

				for (int j = 0; j < child->GetChildCount(); ++j)
				{
					ObjectStack.push(child->GetChild(j));
				}


				if (ObjectStack.empty())
					break;

				child = ObjectStack.top();
				ObjectStack.pop();
			}

		}
	}
	int GetSkeletonIndex(std::string name)
	{
		int index = -1;

		for (unsigned int i = 0; i < m_skeleton.size(); ++i)
		{
			if (m_skeleton[i]->m_node->m_name.compare(name) == 0)
			{
				index = i;
				break;
			}
		}

		return index;
	}


	void ClearData()
	{
		m_Vertex.clear();
		m_Triangle.clear();
		m_Texture_FilePath.clear();
	}


public:
	NodeData* m_rootNode;
	NodeData* m_NowNode;


	/*하나의 MeshNode의 데이터 계산 다 하고 비워줘야됨 다음거를 위해*/
	std::vector<FbxVertexType> m_Vertex;
	std::vector<Triangle> m_Triangle;
	int m_MaterialCount;
	std::vector<std::string> m_Texture_FilePath;
	int m_TriangleCount;
	

	/*MeshNode의 공용데이터*/
	std::vector<SkeletonNode*> m_skeleton;
};