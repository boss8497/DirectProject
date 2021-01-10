#pragma once


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include<iostream>
#include<typeinfo.h>
#include<map>
#include<string>
#include<list>
#include<fstream>
#include<tchar.h>
#include<locale.h>
#include<stack>
#include<unordered_map>
#include<queue>
#include<CommCtrl.h>
#include<commdlg.h>

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include"resource.h"
#include"Transform.h"
#include"Text.h"
#include"AudioComponent.h"
#include"GameObjectManager.h"
#include"GameObject.h"
#include"ApplicationEnum.h"
#include"resource.h"


#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"Comctl32.lib")