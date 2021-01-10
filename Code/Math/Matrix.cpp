#include "pch.h"
#include "Matrix.h"

Matrix::Matrix::Matrix()
{
	XMStoreFloat4x4(this, XMMatrixIdentity());
}

Matrix::Matrix::Matrix(FXMMATRIX & m)
{
	XMStoreFloat4x4(this, m);
}

Matrix::Matrix::~Matrix()
{
}

void Matrix::Matrix::SetScalingMatrix(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixScaling(v.x, v.y, v.z);
	XMStoreFloat4x4(this, m);
}

void Matrix::Matrix::SetRotationMatrix(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixRotationRollPitchYaw(XMConvertToRadians(v.x), 
		XMConvertToRadians(v.y), XMConvertToRadians(v.z));
	XMStoreFloat4x4(this, m);
}

void Matrix::Matrix::SetTranslationMatrix(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixTranslation(v.x, v.y, v.z);
	XMStoreFloat4x4(this, m);
}

Matrix::Matrix Matrix::Matrix::Transpose()
{
	XMMATRIX t = XMMatrixTranspose(XMLoadFloat4x4(this));
	Matrix result;
	XMStoreFloat4x4(&result, t);
	return result;
}

XMMATRIX Matrix::Matrix::GetXMMATRIX()
{
	XMMATRIX result = XMLoadFloat4x4(this);
	return result;
}

Matrix::Matrix Matrix::Matrix::Inverse()
{
	XMVECTOR det = XMMatrixDeterminant(this->GetXMMATRIX());
	XMMATRIX inverse = XMMatrixInverse(&det, this->GetXMMATRIX());
	return Matrix(inverse);
}

Matrix::Matrix Matrix::Matrix::Transpose(Matrix m)
{
	XMMATRIX t = XMMatrixTranspose(XMLoadFloat4x4(&m));
	Matrix result;
	XMStoreFloat4x4(&result, t);
	return result;
}

Matrix::Matrix Matrix::Matrix::Scaling(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixScaling(v.x, v.y, v.z);
	Matrix r;
	XMStoreFloat4x4(&r, m);
	return r;
}

Matrix::Matrix Matrix::Matrix::Rotation(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixRotationRollPitchYaw(XMConvertToRadians(v.x),
		XMConvertToRadians(v.y), XMConvertToRadians(v.z));
	Matrix r;
	XMStoreFloat4x4(&r, m);
	return r;
}

Matrix::Matrix Matrix::Matrix::Translation(Vector::Vector3 v)
{
	XMMATRIX m = XMMatrixTranslation(v.x, v.y, v.z);
	Matrix r;
	XMStoreFloat4x4(&r, m);
	return r;
}

Matrix::Matrix Matrix::Matrix::Inverse(Matrix & in_m)
{
	XMVECTOR det = XMMatrixDeterminant(in_m.GetXMMATRIX());
	XMMATRIX inverse = XMMatrixInverse(&det, in_m.GetXMMATRIX());
	return Matrix(inverse);
}

Matrix::Matrix Matrix::Matrix::SRT(Vector::Vector3& s, Vector::Vector3& r, Vector::Vector3& t)
{
	Matrix result = Matrix::Multiply(Matrix::Multiply(Matrix::Scaling(s),Matrix::Rotation(r)), Matrix::Translation(t));
	return result;
}

Matrix::Matrix Matrix::Matrix::Identity()
{
	Matrix m;
	XMStoreFloat4x4(&m, XMMatrixIdentity());
	return m;
}

Matrix::Matrix Matrix::Matrix::Multiply(Matrix & in_lMatrix, Matrix & in_RMatrix)
{
	XMMATRIX lmat = XMLoadFloat4x4(&in_lMatrix);
	XMMATRIX rmat = XMLoadFloat4x4(&in_RMatrix);
	Matrix result = XMMatrixMultiply(lmat, rmat);

	return result;
}

void Matrix::Matrix::XMMATRIX_Print(XMMATRIX m)
{
	XMFLOAT4X4 p;
	XMStoreFloat4x4(&p, m);

	printf("----------------------\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n----------------------\n",
		p.m[0][0], p.m[0][1], p.m[0][2], p.m[0][3],
		p.m[1][0], p.m[1][1], p.m[1][2], p.m[1][3]
		, p.m[2][0], p.m[2][1], p.m[2][2], p.m[2][3]
		, p.m[3][0], p.m[3][1], p.m[3][2], p.m[3][3]);

}

Matrix::Matrix Matrix::Matrix::operator*(Matrix& r_m)
{
	XMMATRIX lmat = XMLoadFloat4x4(this);
	XMMATRIX rmat = XMLoadFloat4x4(&r_m);
	Matrix result = XMMatrixMultiply(lmat, rmat);

	return result;
}

Matrix::Matrix Matrix::Matrix::operator+(Matrix m)
{
	Matrix result;
	XMStoreFloat4x4(&result,XMLoadFloat4x4(this) + XMLoadFloat4x4(&m));

	return result;
}

Matrix::Matrix Matrix::Matrix::operator*(float f)
{
	XMMATRIX m = XMLoadFloat4x4(this);
	m *= f;
	return Matrix(m);
}

Matrix::Matrix Matrix::Matrix::operator/(float f)
{
	XMMATRIX m = XMLoadFloat4x4(this);
	m = m / f;
	Matrix result(m);
	
	return result;
}
