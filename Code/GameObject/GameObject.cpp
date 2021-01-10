#include "pch.h"
#include "GameObject.h"
#include "TimeClass.h"

GameObject::GameObject()
{
	this->GetNewQueue()->push(this);
}

GameObject::~GameObject()
{
}