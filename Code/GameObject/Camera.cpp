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

	//������ ����Ű�� ���� ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//xmvector ����ü�� �ε�
	upVector = XMLoadFloat3(&up);

	//3d���忡�� ī�޶��� ��ġ�� ����
	position = transform->Position;

	//xmvector ����ü�� �ε�
	positionVector = XMLoadFloat3(&position);

	//LookAtPos
	//�⺻������ ī�޶� ã���ִ� ���⸦ ����
	//Lookat - Eye = Eye->Lookat.Normal ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//�ε�
	lookAtVector = XMLoadFloat3(&lookAt);

	//ȸ������ ���� ������ ����
	pitch = transform->Rotate.x * 0.0174532925f;
	yaw = transform->Rotate.y * 0.0174532925f;
	roll = transform->Rotate.z * 0.0174532925f;

	//ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//lookat �� up���͸� ȸ�� ��ķ� �����Ͽ� �䰡 �������� �ùٸ��� Ⱥ���ǵ��� �մϴ�.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//ȸ�� �� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//���������� �� ���� ������Ʈ �� ���Ϳ��� �� ���
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
