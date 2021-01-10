#include "pch.h"
#include "FbxClass.h"

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
}

bool RenderQueue::Inite()
{
	return true;
}

void RenderQueue::Destroy()
{
	this->ClearQueue();
}

bool RenderQueue::Render3D(ID3D11DeviceContext * deviceContext, DirectionalLight * light)
{
	//랜더링 오브젝트 카운트
	int count = 0;

	while (!m_Render3DQ.empty())
	{
		BaseObject* curObj = m_Render3DQ.front();
		Material* mat = curObj->GetCompponent<Material>();
		FbxClass* fbx = curObj->GetCompponent<FbxClass>();

		if (mat)
		{
			if (!mat->Render(deviceContext, curObj->GetCompponent<Transform>(), light))
			{

			}
		}
		else if (fbx)
		{
			if (!fbx->Render(deviceContext, light))
			{

			}
		}

		//m_Render3DQ_Wire.push(curObj);

		m_Render3DQ.pop();
		++count;
	}

	return true;
}

bool RenderQueue::Render3DWire(ID3D11DeviceContext * deviceContext, DirectionalLight * light)
{
	while (!m_Render3DQ_Wire.empty())
	{
		BaseObject* curObj = m_Render3DQ_Wire.front();
		Material* mat = curObj->GetCompponent<Material>();
		FbxClass* fbx = curObj->GetCompponent<FbxClass>();

		if (mat)
		{
			if (!mat->Render_WireFrame(deviceContext, curObj->GetCompponent<Transform>(), light))
			{

			}
		}


		m_Render3DQ_Wire.pop();
	}
	return true;
}

bool RenderQueue::Render2D(ID3D11DeviceContext * deviceContext, D3D::DirectX3D * d3d, Vector::Vector2 & in_screensize)
{
	//랜더링 오브젝트 카운트
	int count = 0;

	while (!m_Render2DQ.empty())
	{
		BaseObject* curObj = m_Render2DQ.top();
		Text* text = curObj->GetCompponent<Text>();
		Texture2D* texture2d = curObj->GetCompponent<Texture2D>();

		//printf("%s\n", curObj->Name.c_str());
		if (text)
		{
			d3d->TurnOnAlphaBlending();
			if (!text->Render(deviceContext, curObj->GetCompponent<Transform>()))
			{

			}
			d3d->TurnOffAlphaBlending();
		}
		else if (texture2d)
		{

			if (!texture2d->Render(deviceContext, in_screensize, curObj->GetCompponent<Transform>()))
			{

			}
		}

		m_Render2DQ.pop();
		++count;
	}

	return true;
}

void RenderQueue::ClearQueue()
{
	std::queue<BaseObject*> tmp;
	std::priority_queue<BaseObject*, std::vector<BaseObject*>, CompareObejct> tmp2;

	std::swap(m_Render3DQ, tmp);
	std::swap(m_Render2DQ, tmp2);
}
