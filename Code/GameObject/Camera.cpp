#include "pch.h"
#include "Camera.h"
#include "Screen.h"

Camera::Camera()
{
	transform = GetCompponent<Transform>();
	this->GetNewQueue()->push(this);
	SetName("Main Camera");
}

Camera::Camera(const Camera &)
{
}

Camera::~Camera()
{
	transform = nullptr;
}

void Camera::SetPosition(float x, float y, float z)
{
	transform->Position.x = x;
	transform->Position.y = y;
	transform->Position.z = z;
}

void Camera::SetPosition(Vector::Vector3& in_pos)
{
	transform->Position = in_pos;
}

void Camera::SetRotation(float x, float y, float z)
{
	transform->Rotate.x = x;
	transform->Rotate.y = y;
	transform->Rotate.z = z;
}

void Camera::LookAt(BaseObject * obj)
{
	if (obj != nullptr)
	{
		Matrix::Matrix wordlMat = obj->GetCompponent<Transform>()->GetWorldMatrix();
		Matrix::Matrix tMat = Matrix::Matrix::Translation(Vector::Vector3(0, 0, 5.0f));
		Matrix::Matrix rMat = Matrix::Matrix::Rotation(Vector::Vector3(0, 180, 0));
		Matrix::Matrix result = (rMat*tMat) * wordlMat;

		this->GetCompponent<Transform>()->WorldMatrixImportSRT(result);
	}
}

Vector::Vector3 Camera::GetPosition()
{
	return transform->Position;
}

Vector::Vector3 Camera::GetRotation()
{
	return transform->Rotate;
}

void Camera::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	//위쪽을 가리키는 방향 벡터
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//xmvector 구조체에 로드
	upVector = XMLoadFloat3(&up);

	//3d월드에서 카메라의 위치를 설정
	position = transform->Position;

	//xmvector 구조체에 로드
	positionVector = XMLoadFloat3(&position);

	//LookAtPos
	//기본적으로 카메라가 찾고있는 방향를 설정
	//Lookat - Eye = Eye->Lookat.Normal 생략
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//로드
	lookAtVector = XMLoadFloat3(&lookAt);

	//회전값을 라디안 단위로 설정
	pitch = transform->Rotate.x * 0.0174532925f;
	yaw = transform->Rotate.y * 0.0174532925f;
	roll = transform->Rotate.z * 0.0174532925f;

	//회전 행렬을 만듬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//lookat 및 up벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 횐전되도록 합니다.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//회전 된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);


	XMFLOAT3 pos(0, 0, -1.0f);
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	XMVECTOR p = XMLoadFloat3(&pos);
	XMVECTOR l = XMLoadFloat3(&lookAt);
	XMVECTOR u = XMLoadFloat3(&up);

	m_UI_viewMatrix = XMMatrixLookToLH(p, l, u);
}

void Camera::GetViewMatrix(Matrix::Matrix & viewMattrix)
{
	viewMattrix = m_viewMatrix;
}

Matrix::Matrix Camera::GetViewMatrix()
{
	return this->m_viewMatrix;
}

Matrix::Matrix Camera::GetUIViewMatrix()
{
	return m_UI_viewMatrix;
}
