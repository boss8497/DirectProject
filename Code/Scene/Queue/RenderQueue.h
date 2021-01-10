#pragma once
#include"pch.h"
#include"DirectX3D.h"
#include"Text.h"
#include"Texture2D.h"


/*비교연산 2D Depth순서에따라 렌더하기 위해*/
struct CompareObejct
{
	bool operator()(BaseObject* a, BaseObject* b)
	{
		int aDepth = 0;
		int bDepth = 0;
		Text* text = a->GetCompponent<Text>();
		Texture2D* texture2d = a->GetCompponent<Texture2D>();
		if (text)
		{
			aDepth = text->Depth;
		}
		else if (texture2d)
		{
			aDepth = texture2d->Depth;
		}
		else
		{
			return false;
		}


		text = b->GetCompponent<Text>();
		texture2d = b->GetCompponent<Texture2D>();


		if (text)
		{
			bDepth = text->Depth;
		}
		else if (texture2d)
		{
			bDepth = texture2d->Depth;
		}
		else
		{
			return false;
		}

		return aDepth < bDepth;
	}
};

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();
	RenderQueue(const RenderQueue& copy) = delete;

public:
	bool Inite();
	void Destroy();

public:

	bool Render3D(ID3D11DeviceContext* deviceContext, DirectionalLight* light);
	bool Render3DWire(ID3D11DeviceContext* deviceContext, DirectionalLight* light);
	bool Render2D(ID3D11DeviceContext * deviceContext, D3D::DirectX3D* d3d, Vector::Vector2& in_screensize);

	void ClearQueue();
public:
	std::queue<BaseObject*> m_Render3DQ;
	std::queue<BaseObject*> m_Render3DQ_Wire;
	std::priority_queue<BaseObject*, std::vector<BaseObject*>, CompareObejct> m_Render2DQ;
};