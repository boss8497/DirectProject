#include"pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(const DirectionalLight &)
{
}

DirectionalLight::~DirectionalLight()
{
}

bool DirectionalLight::Inite()
{
	m_ambientColor = Vector::Vector4(0, 0, 0, 0);
	m_diffuseColor = Vector::Vector4(0, 0, 0, 0);
	m_direction = Vector::Vector3::Zero();
	return true;
}

void DirectionalLight::Destroy()
{
}

void DirectionalLight::Update()
{
}

void DirectionalLight::SetAmbientColor(float _x, float _y, float _z, float _w)
{
	m_ambientColor = Vector::Vector4(_x, _y, _z, _w);
}

void DirectionalLight::SetDiffuseColor(float _x, float _y, float _z, float _w)
{
	m_diffuseColor = Vector::Vector4(_x, _y, _z, _w);
}

void DirectionalLight::SetDirection(float _x, float _y, float _z)
{
	m_direction = Vector::Vector3(_x, _y, _z);
}

Vector::Vector4 DirectionalLight::GetAmbientColor()
{
	return m_ambientColor;
}

Vector::Vector4 DirectionalLight::GetDiffuseColor()
{
	return m_diffuseColor;
}

Vector::Vector3 DirectionalLight::GetDirection()
{
	return m_direction;
}
