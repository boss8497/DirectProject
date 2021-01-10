#pragma once
#include"pch.h"
#pragma comment(lib,"winmm.lib")

class TimeClass
{
public:
	static TimeClass& Instance()
	{
		static TimeClass* m_instance = new TimeClass();
		return *m_instance;
	}

private:
	TimeClass() { this->Inite(); };
	void Inite();
	void UpdateFPS();
	void UpdateDeletaTime();
public:
	void Update();


public:
	float DeletaTime = 0.0f;
	int FPS = 0;

private:
	DWORD previousTime;
	int FPSCount = 0;


	LARGE_INTEGER countDeletaTime;
	LARGE_INTEGER currDeletaTime;
	LARGE_INTEGER preDeletaTime;
};