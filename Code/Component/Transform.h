#pragma once
#include"pch.h"
#include"Component.h"
#include"BaseObject.h"


class Transform : public Component<BaseObject>
{
public:
	Transform();
	Transform(BaseObject* obj);
	~Transform();

public:
	bool Inite();
	void Update(UpdateQueue* uQueue);
	void Destroy();
	//������ �ν����� ����
	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
public:
	Vector::Vector3 GetForword();

	void SetPosition(float x, float y, float z);
	void SetPosition(Vector::Vector3 v);

	void SetRotate(float x, float y, float z);
	void SetRotate(Vector::Vector3 v);

	void SetScale(float x, float y, float z);
	void SetScale(Vector::Vector3 v);

	void SetSize(float x, float y);
	void SetSize(Vector::Vector3 v);
	/*�Ⱦ��� �����ߵ�*/
	void SetWorldMatrix_UI();
	
	void WorldMatrixImportSRT(Matrix::Matrix& in_m);

	Matrix::Matrix GetWorldMatrix();
	Matrix::Matrix GetLocalMatrix();

	BaseObject* GameObject();
public:
	Vector::Vector3 Position;
	Vector::Vector3 Rotate;
	Vector::Vector3 Scale;
	Vector::Vector3 Size;
	/*�Ⱦ��� �����ߵ�*/
	Matrix::Matrix World;

	static Matrix::Matrix World_Matrix;
	static Matrix::Matrix Projection_Matrix;
	static Matrix::Matrix View_Matrix;
	static Matrix::Matrix ViewUI_Matrix;





	//Test setting

public:

	FbxAMatrix Fbx_worldMatrix;
};

