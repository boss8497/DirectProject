#pragma once
#include"pch.h"
#include"Component.h"
#include"Vector.h"

class DirectionalLight : Component<BaseObject>
{
public:
	DirectionalLight();
	DirectionalLight(const DirectionalLight&);
	~DirectionalLight();


	bool Inite();
	void Destroy();
	void Update();

	void SetAmbientColor(float _x, float _y, float _z, float _w);
	void SetDiffuseColor(float _x, float _y, float _z, float _w);
	void SetDirection(float _x, float _y, float _z);

	Vector::Vector4 GetAmbientColor();
	Vector::Vector4 GetDiffuseColor();
	Vector::Vector3 GetDirection();

private:
	Vector::Vector4 m_ambientColor;
	Vector::Vector4 m_diffuseColor;
	Vector::Vector3 m_direction;
};