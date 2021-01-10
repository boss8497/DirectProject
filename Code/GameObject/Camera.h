#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"BaseObject.h"

class Camera : public BaseObject
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();


	void SetPosition(float x, float y, float z);
	void SetPosition(Vector::Vector3& in_pos);
	void SetRotation(float x, float y, float z);
	void LookAt(BaseObject* obj);

	Vector::Vector3 GetPosition();
	Vector::Vector3 GetRotation();

	void Render();
	void GetViewMatrix(Matrix::Matrix&);
	Matrix::Matrix GetViewMatrix();
	Matrix::Matrix GetUIViewMatrix();


public:
	Transform * transform;
private:
	Matrix::Matrix m_viewMatrix;
	Matrix::Matrix m_UI_viewMatrix;
};