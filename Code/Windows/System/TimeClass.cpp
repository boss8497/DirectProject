#include "pch.h"
#include "TimeClass.h"

void TimeClass::UpdateFPS()
{
	DWORD currTime = timeGetTime();
	++FPSCount;

	if (currTime >= (previousTime + 1000))
	{
		FPS = FPSCount;
		FPSCount = 0;
		previousTime = currTime;
	}
}

void TimeClass::UpdateDeletaTime()
{
	QueryPerformanceCounter(&currDeletaTime);
	DeletaTime = (static_cast<float>(currDeletaTime.QuadPart) - static_cast<float>(preDeletaTime.QuadPart))
		/ static_cast<float>(countDeletaTime.QuadPart) * 1000.0f;

	preDeletaTime = currDeletaTime;
}

void TimeClass::Update()
{
	this->UpdateFPS();
	this->UpdateDeletaTime();
}

void TimeClass::Inite()
{
	/*시간 초기화*/
	previousTime = timeGetTime();
	QueryPerformanceCounter(&currDeletaTime);
	QueryPerformanceCounter(&preDeletaTime);
	QueryPerformanceFrequency(&countDeletaTime);
}
