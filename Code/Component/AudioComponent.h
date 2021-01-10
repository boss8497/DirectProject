#pragma once
#include"pch.h"
#include"DirectXHeader.h"
#include"AudioSystem.h"
#include"Component.h"


class AudioComponent : public Component<BaseObject>
{
public:
	AudioComponent();
	~AudioComponent();
	AudioComponent(const AudioComponent& copy);



public:
	bool Inite(std::wstring path = Default_Sound, bool loop = false, bool play = false);
	void Destroy();
	void Update(); 
	void Update(UpdateQueue* uQueue);

	void PlayOnShot();
	void Play();
	void Loop(bool flag);
	void Stop(bool flag = true);
	void SetFilePath(std::wstring path, bool cFlag = true);
	void DeleteSound();


	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
	/*Member*/
private:
	float m_Volume = 0.05f;
	bool m_SoundChangeFlag = false;
	bool m_loop = false;
	SoundEffectInstance* m_audio = nullptr;
	std::wstring m_FilePath;
};