#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DirectXTK.lib")
/*input*/
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
/*sound*/


#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<dinput.h>
#include<mmsystem.h>

#include"AligendAllocationPolicy.h"

//../DirectProject/Font/gulrim_0.dds
///Defaul/Texture/

#define Default_Texture "../DirectProject/Default/Texture/Default_Texture.dds"
#define Default_Texture2D "../DirectProject/Default/Texture/Default_Image.jpg"
#define Default_Mesh "../DirectProject/Default/Mesh/cube.txt"

//Sphere Skybox
#define Default_SkyBox_Texture "../DirectProject/Default/Texture/skymap.dds"
//Cube SkyBox
#define Default_BOX_SkyBox_Texture "../DirectProject/Default/Texture/skyboxdefault.png"

#define Default_PS "../DirectProject/DefaultPS.hlsl"
#define Default_VS "../DirectProject/DefaultVS.hlsl"
#define Default_NOLIGHT_PS "../DirectProject/NoLightPS.hlsl"
#define Default_NOLIGHT_VS "../DirectProject/NoLightVS.hlsl"
#define Default_COLOR_PS "../DirectProject/ColorPS.hlsl"
#define Default_COLOR_VS "../DirectProject/ColorVS.hlsl"
#define Default_FBX_VS "../DirectProject/FbxVS.hlsl"

#define Default_Font_VS "../DirectProject/DefaultFontVS.hlsl"
#define Default_Font_PS "../DirectProject/DefaultFontPS.hlsl"
#define Default_Font_Size 16


#define Default_2D_VS "../DirectProject/Default2DTextureVS.hlsl"
#define Default_2D_PS "../DirectProject/Default2DTexturePS.hlsl"

#define Default_Font "../DirectProject/Font/gulrim.fnt"
#define Default_Font_Texture "../DirectProject/Font/gulrim_0.dds"


#define Default_Sound L"../DirectProject/Default/Sound/sound01.wav"

#define BoneMatrixCount 50
#define TEXT_LENGTH_MAX 255
#define FILE_NAME_MAX 128

#define RadianToDegree(radian) DirectX::XMConvertToDegrees(radian)
#define DegreeToRadian(degree) DirectX::XMConvertToRadians(degree)

#define _WIN32_WINNT 0x0A00


#include"DDSTextureLoader.h"
#include"SpriteFont.h"
#include"WICTextureLoader.h"
#include"Audio.h"
#include<mmdeviceapi.h>

using namespace DirectX;