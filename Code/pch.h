#pragma once


#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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