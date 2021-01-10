#pragma once
#include"pch.h"
#include"BaseObject.h"
#include"Material.h"

class GameObject : public BaseObject
{
public:
	GameObject();
	virtual ~GameObject();


public:
	virtual bool Inite() { return true; }
	virtual void Update() {}
	virtual void OnDestroy() {}
};