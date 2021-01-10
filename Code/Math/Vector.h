#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"fbxsdk.h"

using namespace DirectX;

namespace Vector
{
#define FLOAT_EPSILON 0.000001f
#define DOUBLE_EPSILON 0.0000000000000001f

	class Vector2 : public XMFLOAT2, public AligendAllocationPolicy<16>
	{
		/*생성자*/
	public:
		Vector2() : XMFLOAT2(0, 0){};
		Vector2(float _x, float _y) : XMFLOAT2(_x, _y){}
		Vector2(XMFLOAT2 v1) : XMFLOAT2(v1){}
		Vector2(_In_reads_(2) const float *pArray) : XMFLOAT2(pArray){}
		Vector2(FXMVECTOR v1);
		Vector2(const Vector2& copy) = default;
		Vector2(XMFLOAT3 v) : XMFLOAT2(v.x, v.y) {}
		

		/*연산자*/
	public:
		Vector2			operator+(Vector2 v1);
		Vector2			operator+= (Vector2 v1);
		Vector2			operator-(Vector2 v1);
		Vector2			operator-=(Vector2 v1);
		Vector2			operator*(float f);
		Vector2			operator/(Vector2 v1);
		Vector2			operator/(float f);

		friend Vector2	operator*(float f, Vector2 v1);
		friend Vector2  operator/(float f, Vector2 v1);



		/*정적*/
	public:
		static float Length(Vector2& v1);
		static float DotProductToRadian(Vector2& v1, Vector2& v2);
		static float DotProductToDegrees(Vector2& v1, Vector2& v2);
		static Vector2 Cross(Vector2& v1, Vector2& v2);
		static Vector2 Normalize(Vector2& v1);
		static bool NearEqual(Vector2& v1, Vector2& v2);


		static Vector2 Identity();
		static Vector2 Zero();

	public:
		void Set(float _x, float _y = 0);
		void Set(POINT* point);

		/*디버그*/
	public:
		void print()
		{
			printf("[%f , %f]\n", x, y);
		}
	};


	/*프렌드*/
	Vector2 operator* (float f, Vector2 v1);
	Vector2 operator/ (float f, Vector2 v1);



	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/


	class Vector3 : public XMFLOAT3, public AligendAllocationPolicy<16>
	{
		/*생성자*/
	public:
		Vector3() : XMFLOAT3(0, 0, 0) {};
		Vector3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
		Vector3(XMFLOAT3 v1) : XMFLOAT3(v1) {}
		Vector3(_In_reads_(3) const float *pArray) : XMFLOAT3(pArray) {}
		Vector3(FXMVECTOR v1);
		Vector3(const Vector3& copy) = default;
		Vector3(const FbxVector4& fv4);


		/*연산자*/
	public:
		Vector3			operator+(Vector3 v1);
		Vector3			operator+= (Vector3 v1);
		Vector3			operator-(Vector3 v1);
		Vector3			operator-=(Vector3 v1);
		Vector3			operator*(float f);
		Vector3			operator/(Vector3 v1);
		Vector3			operator/(float f);

		friend Vector3	operator*(float f, Vector3 v1);
		friend Vector3  operator/(float f, Vector3 v1);



		/*정적*/
	public:
		static float Length(Vector3& v1);
		static float DotProductToRadian(Vector3& v1, Vector3& v2);
		static float DotProductToDegrees(Vector3& v1, Vector3& v2);
		static Vector3 Cross(Vector3& v1, Vector3& v2);
		static Vector3 Normalize(Vector3& v1);
		static bool NearEqual(Vector3& v1, Vector3& v2);
		static Vector3 Mul(Vector3& v1, Vector3 v2);

		static Vector3 Identity();
		static Vector3 Zero();
		static Vector3 Forward();
		static Vector3 Up();
		static Vector3 Right();

	public:
		void Normalize();
		void Set(float _x, float _y, float _z);

		/*디버그*/
	public:
		void print()
		{
			printf("[%f , %f, %f]\n", x, y, z);
		}
	};


	Vector3 operator* (float f, Vector3 v1);
	Vector3 operator/ (float f, Vector3 v1);


	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
	/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

	class Vector4 : public XMFLOAT4, public AligendAllocationPolicy<16>
	{
		/*생성자*/
	public:
		Vector4() : XMFLOAT4(0, 0, 0, 0) {};
		Vector4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		Vector4(XMFLOAT4 v1) : XMFLOAT4(v1) {}
		Vector4(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
		Vector4(FXMVECTOR v1);
		Vector4(const Vector4& copy) = default;



		/*연산자*/
	public:
		Vector4			operator+(Vector4 v1);
		Vector4			operator+= (Vector4 v1);
		Vector4			operator-(Vector4 v1);
		Vector4			operator-=(Vector4 v1);
		Vector4			operator*(float f);
		Vector4			operator/(Vector4 v1);
		Vector4			operator/(float f);

		friend Vector4	operator*(float f, Vector4 v1);
		friend Vector4  operator/(float f, Vector4 v1);



		/*정적*/
	public:
		static float Length(Vector4& v1);
		static float DotProductToRadian(Vector4& v1, Vector4& v2);
		static float DotProductToDegrees(Vector4& v1, Vector4& v2);
		static Vector4 Normalize(Vector4& v1);
		static bool NearEqual(Vector4& v1, Vector4& v2);



		static Vector4 Zero();

	public:
		void Set(float _x, float _y, float _z,float _w);


		/*디버그*/
	public:
		void print()
		{
			printf("[%f , %f , %f , %f]\n", x, y, z, w);
		}
	};


	Vector4 operator* (float f, Vector4 v1);
	Vector4 operator/ (float f, Vector4 v1);
}