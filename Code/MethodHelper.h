#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include<codecvt>


class MethodHelper
{
public:
	static wchar_t *GetWC(const char *c)
	{
		/*msdn*/
		/* https://docs.microsoft.com/ko-kr/cpp/text/how-to-convert-between-various-string-types?view=vs-2019 */
		size_t newsize = strlen(c) + 1;
		wchar_t * wcstring = new wchar_t[newsize];

		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, c, _TRUNCATE);
		
		return wcstring;
	}

	static void GetWC(std::wstring& wc, char *c)
	{
		/*msdn*/
		/* https://docs.microsoft.com/ko-kr/cpp/text/how-to-convert-between-various-string-types?view=vs-2019 */
		size_t newsize = strlen(c) + 1;
		wchar_t * wcstring = new wchar_t[newsize];

		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, c, _TRUNCATE);
		wc = wcstring;
		delete[] wcstring;
	}

	static void strelement(char* arry, char element)
	{
		int arrysize = strlen(arry);
		char *copyarry = new char[arrysize];
		int newsize = 0;

		memcpy(copyarry, arry, arrysize);
		ZeroMemory(arry, arrysize);

		for (int i = 0; i < arrysize; ++i)
		{
			if (copyarry[i] == element)
			{
				continue;
			}
			arry[newsize] = copyarry[i];
			++newsize;
		}
		delete[] copyarry;
	}


	//static void QuaterinonToEuler(Vector::Vector4& in_quater,Vector::Vector3& inout_euler)
	//{
	//	// roll (x-axis rotation)
	//	double sinr_cosp = 2 * (in_quater.w * in_quater.x + in_quater.y * in_quater.z);
	//	double cosr_cosp = 1 - 2 * (in_quater.x * in_quater.x + in_quater.y * in_quater.y);
	//	inout_euler.x = static_cast<float>( std::atan2(sinr_cosp, cosr_cosp));

	//	// pitch (y-axis rotation)
	//	double sinp = 2 * (in_quater.w * in_quater.y - in_quater.z * in_quater.x);
	//	if (std::abs(sinp) >= 1)
	//		inout_euler.y = static_cast<float>(std::copysign(M_PI / 2, sinp)); // use 90 degrees if out of range
	//	else
	//		inout_euler.y = static_cast<float>(std::asin(sinp));

	//	// yaw (z-axis rotation)
	//	double siny_cosp = 2 * (in_quater.w * in_quater.z + in_quater.x * in_quater.y);
	//	double cosy_cosp = 1 - 2 * (in_quater.y * in_quater.y + in_quater.z * in_quater.z);
	//	inout_euler.z = static_cast<float>(std::atan2(siny_cosp, cosy_cosp));
	//}

	static void QuaterinonToEuler(Vector::Vector4& in_quater, Vector::Vector3& inout_euler)
	{
		double sqw = in_quater.w * in_quater.w;
		double sqx = in_quater.x * in_quater.x;
		double sqy = in_quater.y * in_quater.y;
		double sqz = in_quater.z * in_quater.z;

		inout_euler.x = static_cast<float>( asin(2.0f*(in_quater.w * in_quater.x - in_quater.y * in_quater.z)));
		inout_euler.y = static_cast<float>(atan2(2.0f * (in_quater.x*in_quater.z + in_quater.w * in_quater.y), (-sqx - sqy + sqz + sqw)));
		inout_euler.z = static_cast<float>(atan2(2.0f * (in_quater.x*in_quater.y + in_quater.w * in_quater.z), (-sqx + sqy - sqz + sqw)));
	}

	static const char* FloatToCstr(float num)
	{
		char TransStr[255];
		ZeroMemory(TransStr, sizeof(TransStr));
		sprintf_s(TransStr, sizeof(TransStr), "%f.6", num);

		return TransStr;
	}

	static bool IsAlpha(wchar_t* wchar)
	{
		int length = wcslen(wchar);

		for (int i = 0; i < length; ++i)
		{
			if (iswalpha(wchar[i]) != 0)
			{
				return true;
			}

		}

		return false;
	}
};


/*쉐이더 리소스  크기 가져오는법*/
//void SkyBoxMesh::GetTextureShaderWidthandHeight(ID3D11ShaderResourceView * sv, int * width, int * height)
//{
//	ID3D11Resource * pResource = NULL;
//	ID3D11Texture2D* texture2D = nullptr;
//	D3D11_TEXTURE2D_DESC desc;
//
//	sv->GetResource(&pResource);
//	texture2D = (ID3D11Texture2D*)pResource;
//
//	texture2D->GetDesc(&desc);
//	*width = desc.Width;
//	*height = desc.Height;
//	texture2D->Release();
//}