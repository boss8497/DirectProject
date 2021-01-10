#pragma once
#include"pch.h"
#include"Texture.h"


class UpdateQueue
{
public:
	UpdateQueue();
	~UpdateQueue();
	UpdateQueue(const UpdateQueue& copy) = delete;


	bool Inite();
	void Destroy();
	bool Update(ID3D11Device* device);


	bool UpdateTexture(ID3D11Device* device);
public:
	void Push(Texture* texture);

public:

	std::queue<Texture*> m_TextureQ;
};