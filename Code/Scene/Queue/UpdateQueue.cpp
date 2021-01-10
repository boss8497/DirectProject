#include "pch.h"

UpdateQueue::UpdateQueue()
{
}

UpdateQueue::~UpdateQueue()
{
}

bool UpdateQueue::Inite()
{
	return true;
}

void UpdateQueue::Destroy()
{
}

bool UpdateQueue::Update(ID3D11Device* device)
{
	if (!this->UpdateTexture(device))
	{
		printf("UpdateQueue : UpdateTexture Error\n");
		return false;
	}
}

bool UpdateQueue::UpdateTexture(ID3D11Device* device)
{
	if (m_TextureQ.size() <= 0)
	{
		return true;
	}


	while (!m_TextureQ.empty())
	{
		Texture* texture = m_TextureQ.front();
		m_TextureQ.pop();

		if (!texture->ChangeTexture(device))
		{
			printf("UpdateQueue : UpdateTexture ChangeTexture Error\n");
			return false;
		}
	}

	std::queue<Texture*> tmpQ;
	std::swap(m_TextureQ, tmpQ);

	return true;
}

void UpdateQueue::Push(Texture * texture)
{
	m_TextureQ.push(texture);
}
