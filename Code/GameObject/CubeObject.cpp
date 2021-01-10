
#include"pch.h"
#include "CubeObject.h"

CubeObject::CubeObject()
{
	this->GetNewQueue()->push(this);
	this->SetName("Cube");
	Material* mat = this->AddCompponent<Material>();
	mat->SetMeshFilePath(nullptr);
	mat->SetTextureFilePath(nullptr);
	mat->SetShaderFilePath(nullptr, nullptr);
}

CubeObject::~CubeObject()
{
}
