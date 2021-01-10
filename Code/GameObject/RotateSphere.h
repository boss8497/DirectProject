#pragma once
#include"RotateObject.h"
#include"GameObject.h"


class RotateShpere : public GameObject
{
public:
	RotateShpere()
	{
		Material* mat = AddCompponent<Material>();
		mat->m_MeshType = MESH_SPHERE_TYPE;
		mat->SetMeshFilePath(nullptr);
		mat->SetTextureFilePath(nullptr);
		mat->SetShaderFilePath(nullptr, nullptr);

		AddCompponent<RotateObject>();
	}
};


