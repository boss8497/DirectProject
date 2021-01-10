#include "pch.h"
#include "Vector.h"

Vector::Vector2::Vector2(FXMVECTOR v1)
{
	DirectX::XMStoreFloat2(this, v1);
}
Vector::Vector2 Vector::Vector2::operator+(Vector2 v1)
{
	return Vector::Vector2(DirectX::XMVectorAdd
	(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&v1)));
}

Vector::Vector2 Vector::Vector2::operator+=(Vector2 v1)
{
	return Vector::Vector2(DirectX::XMVectorAdd
	(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&v1)));
}

Vector::Vector2 Vector::Vector2::operator-(Vector2 v1)
{
	return Vector::Vector2(DirectX::XMVectorSubtract
	(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&v1)));
}

Vector::Vector2 Vector::Vector2::operator-=(Vector2 v1)
{
	return Vector::Vector2(DirectX::XMVectorSubtract
	(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&v1)));
}

Vector::Vector2 Vector::Vector2::operator*(float f)
{
	return Vector::Vector2
	(DirectX::XMVectorScale(DirectX::XMLoadFloat2(this), f));
}

Vector::Vector2 Vector::Vector2::operator/(Vector2 v1)
{
	return Vector::Vector2(DirectX::XMVectorDivide
	(DirectX::XMLoadFloat2(this), DirectX::XMLoadFloat2(&v1)));
}

Vector::Vector2 Vector::Vector2::operator/(float f)
{
	XMVECTOR vS = DirectX::XMVectorReplicate(f);
	return Vector::Vector2(DirectX::XMVectorDivide(XMLoadFloat2(this), vS));
}

float Vector::Vector2::Length(Vector2& v1)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, DirectX::XMVector2Length(XMLoadFloat2(&v1)));

	return result.x;
}

float Vector::Vector2::DotProductToRadian(Vector2& v1, Vector2& v2)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Dot(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
	return result.x;
}

float Vector::Vector2::DotProductToDegrees(Vector2& v1, Vector2& v2)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Dot(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));

	return XMConvertToDegrees(result.x);
}

Vector::Vector2 Vector::Vector2::Cross(Vector2& v1, Vector2& v2)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Cross(XMLoadFloat2(&v1), XMLoadFloat2(&v2)));
	return Vector2(result);
}

Vector::Vector2 Vector::Vector2::Normalize(Vector2& v1)
{
	XMFLOAT2 result;
	XMStoreFloat2(&result, XMVector2Normalize(XMLoadFloat2(&v1)));
	return Vector2(result);
}

bool Vector::Vector2::NearEqual(Vector2& v1, Vector2& v2)
{
	XMVECTOR epsilon = XMVectorReplicate(FLOAT_EPSILON);
	return XMVector2NearEqual(XMLoadFloat2(&v1), XMLoadFloat2(&v2), epsilon);
}

Vector::Vector2 Vector::Vector2::Identity()
{
	Vector2 result(1, 1);
	return result;
}

Vector::Vector2 Vector::Vector2::Zero()
{
	Vector2 result(0, 0);
	return result;
}

void Vector::Vector2::Set(float _x, float _y)
{
	x = _x; y = _y;
}

void Vector::Vector2::Set(POINT* point)
{
	x = static_cast<float>(point->x);
	y = static_cast<float>(point->y);
}

Vector::Vector2 Vector::operator*(float f, Vector2 v1)
{
	return Vector::Vector2(XMVectorScale(XMLoadFloat2(&v1), f));
}

Vector::Vector2 Vector::operator/(float f, Vector2 v1)
{
	XMVECTOR vS = XMVectorReplicate(f);
	return Vector::Vector2(XMVectorDivide(XMLoadFloat2(&v1), vS));
}

	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/



Vector::Vector3::Vector3(FXMVECTOR v1)
{
	DirectX::XMStoreFloat3(this, v1);
}

Vector::Vector3::Vector3(const FbxVector4 & fv4)
{
	this->x = static_cast<float>(fv4.mData[0]);
	this->y = static_cast<float>(fv4.mData[1]);
	this->z = static_cast<float>(fv4.mData[2]);
}

Vector::Vector3 Vector::Vector3::operator+(Vector3 v1)
{
	return Vector::Vector3(XMVectorAdd(XMLoadFloat3(this), XMLoadFloat3(&v1)));
}

Vector::Vector3 Vector::Vector3::operator+=(Vector3 v1)
{
	return Vector::Vector3(XMVectorAdd(XMLoadFloat3(this), XMLoadFloat3(&v1)));
}

Vector::Vector3 Vector::Vector3::operator-(Vector3 v1)
{
	return Vector::Vector3(XMVectorSubtract(XMLoadFloat3(this), XMLoadFloat3(&v1)));
}

Vector::Vector3 Vector::Vector3::operator-=(Vector3 v1)
{
	return Vector::Vector3(XMVectorSubtract(XMLoadFloat3(this), XMLoadFloat3(&v1)));
}

Vector::Vector3 Vector::Vector3::operator*(float f)
{
	return Vector::Vector3(XMVectorScale(XMLoadFloat3(this), f));
}

Vector::Vector3 Vector::Vector3::operator/(Vector3 v1)
{
	return Vector::Vector3(XMVectorDivide(XMLoadFloat3(this), XMLoadFloat3(&v1)));
}

Vector::Vector3 Vector::Vector3::operator/(float f)
{
	XMVECTOR vS = XMVectorReplicate(f);
	return Vector::Vector3(XMVectorDivide(XMLoadFloat3(this), vS));
}

float Vector::Vector3::Length(Vector3 & v1)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Length(XMLoadFloat3(&v1)));

	return result.x;
}

float Vector::Vector3::DotProductToRadian(Vector3 & v1, Vector3 & v2)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
	return result.x;
}

float Vector::Vector3::DotProductToDegrees(Vector3 & v1, Vector3 & v2)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Dot(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));

	return XMConvertToDegrees(result.x);
}

Vector::Vector3 Vector::Vector3::Cross(Vector3 & v1, Vector3 & v2)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&v1), XMLoadFloat3(&v2)));
	return Vector3(result);
}

Vector::Vector3 Vector::Vector3::Normalize(Vector3 & v1)
{
	XMFLOAT3 result;
	XMStoreFloat3(&result, XMVector3Normalize(XMLoadFloat3(&v1)));
	return Vector3(result);
}

void Vector::Vector3::Normalize()
{
	XMStoreFloat3(this, XMVector3Normalize(XMLoadFloat3(this)));
}

bool Vector::Vector3::NearEqual(Vector3 & v1, Vector3 & v2)
{
	XMVECTOR epsilon = XMVectorReplicate(FLOAT_EPSILON);
	return XMVector3NearEqual(XMLoadFloat3(&v1), XMLoadFloat3(&v2), epsilon);
}

Vector::Vector3 Vector::Vector3::Mul(Vector3 & v1, Vector3 v2)
{
	XMVECTOR v = XMVectorMultiply(XMLoadFloat3(&v1), XMLoadFloat3(&v2));
	return Vector3(v);
}

Vector::Vector3 Vector::Vector3::Identity()
{
	Vector3 result(1, 1, 1);
	return result;
}

Vector::Vector3 Vector::Vector3::Zero()
{
	Vector3 result(0, 0, 0);
	return result;
}

Vector::Vector3 Vector::Vector3::Forward()
{
	const Vector3 fVec = { 0.0f,0.0f,1.0f };
	return fVec;
}

Vector::Vector3 Vector::Vector3::Up()
{
	const Vector3 uVec = { 0.0f,1.0f,0.0f };
	return uVec;
}

Vector::Vector3 Vector::Vector3::Right()
{
	const Vector3 rVec = { 1.0f,0.0f,0.0f };
	return rVec;
}

void Vector::Vector3::Set(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}


Vector::Vector3 Vector::operator*(float f, Vector3 v1)
{
	return Vector::Vector3(XMVectorScale(XMLoadFloat3(&v1), f));
}

Vector::Vector3 Vector::operator/(float f, Vector3 v1)
{
	XMVECTOR vS = XMVectorReplicate(f);
	return Vector::Vector3(XMVectorDivide(XMLoadFloat3(&v1), vS));
}











Vector::Vector4::Vector4(FXMVECTOR v1)
{
	DirectX::XMStoreFloat4(this, v1);
}

Vector::Vector4 Vector::Vector4::operator+(Vector4 v1)
{
	return Vector::Vector4(XMVectorAdd(XMLoadFloat4(this), XMLoadFloat4(&v1)));
}

Vector::Vector4 Vector::Vector4::operator+=(Vector4 v1)
{
	return Vector::Vector4(XMVectorAdd(XMLoadFloat4(this), XMLoadFloat4(&v1)));
}

Vector::Vector4 Vector::Vector4::operator-(Vector4 v1)
{
	return Vector::Vector4(XMVectorSubtract(XMLoadFloat4(this), XMLoadFloat4(&v1)));
}

Vector::Vector4 Vector::Vector4::operator-=(Vector4 v1)
{
	return Vector::Vector4(XMVectorSubtract(XMLoadFloat4(this), XMLoadFloat4(&v1)));
}

Vector::Vector4 Vector::Vector4::operator*(float f)
{
	return Vector::Vector4(XMVectorScale(XMLoadFloat4(this), f));
}

Vector::Vector4 Vector::Vector4::operator/(Vector4 v1)
{
	return Vector::Vector4(XMVectorDivide(XMLoadFloat4(this), XMLoadFloat4(&v1)));
}

Vector::Vector4 Vector::Vector4::operator/(float f)
{
	XMVECTOR vS = XMVectorReplicate(f);
	return Vector::Vector4(XMVectorDivide(XMLoadFloat4(this), vS));
}

float Vector::Vector4::Length(Vector4 & v1)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Length(XMLoadFloat4(&v1)));

	return result.x;
}

float Vector::Vector4::DotProductToRadian(Vector4 & v1, Vector4 & v2)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Dot(XMLoadFloat4(&v1), XMLoadFloat4(&v2)));
	return result.x;
}

float Vector::Vector4::DotProductToDegrees(Vector4 & v1, Vector4 & v2)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Dot(XMLoadFloat4(&v1), XMLoadFloat4(&v2)));

	return XMConvertToDegrees(result.x);
}

Vector::Vector4 Vector::Vector4::Normalize(Vector4 & v1)
{
	XMFLOAT4 result;
	XMStoreFloat4(&result, XMVector4Normalize(XMLoadFloat4(&v1)));
	return Vector4(result);
}

bool Vector::Vector4::NearEqual(Vector4 & v1, Vector4 & v2)
{
	XMVECTOR epsilon = XMVectorReplicate(FLOAT_EPSILON);
	return XMVector4NearEqual(XMLoadFloat4(&v1), XMLoadFloat4(&v2), epsilon);
}

Vector::Vector4 Vector::Vector4::Zero()
{
	Vector4 result(0, 0, 0, 0);
	return result;
}

void Vector::Vector4::Set(float _x, float _y, float _z, float _w)
{
	x = _x; y = _y; z = _z; w = _w;
}

Vector::Vector4 Vector::operator*(float f, Vector4 v1)
{
	return Vector::Vector4(XMVectorScale(XMLoadFloat4(&v1), f));
}

Vector::Vector4 Vector::operator/(float f, Vector4 v1)
{
	XMVECTOR vS = XMVectorReplicate(f);
	return Vector::Vector4(XMVectorDivide(XMLoadFloat4(&v1), vS));
}