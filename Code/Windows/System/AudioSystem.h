#pragma once
#include"pch.h"
#include"DirectXHeader.h"

class AudioSystem
{
public:
	static AudioSystem* Instance()
	{
		static AudioSystem* m_instance = new AudioSystem();
		return m_instance;
	}

	void Destroy();


	bool ChangeDevice(int _index);
	void RefreshDeviceList();
	void OnNewAudioDevice(bool flag);
	const AudioEngine* GetEngine() 
	{
		return m_audioEngine;
	}

	SoundEffectInstance* CreateInstance(std::wstring path);
	void DeleteInstance(SoundEffectInstance* ptr);

	void Update();
private:
	void DestroyIntanceList();
	void DestroySoundMap();


private:
	bool m_retryAudio = false;
	AudioEngine * m_audioEngine;
	std::vector<DirectX::AudioEngine::RendererDetail> m_deviceList;

	std::map<std::wstring, SoundEffect*> m_Soundmap;
	std::list<std::unique_ptr<SoundEffectInstance>> m_InstanceList;
private:
	AudioSystem()
	{
		AUDIO_ENGINE_FLAGS flags;
		flags = AudioEngine_Default;
#ifdef _DEBUG
		flags |= AudioEngine_Debug;
#endif
		m_audioEngine = new AudioEngine(flags);
	}
	AudioSystem(const AudioSystem& copy) = delete;
	~AudioSystem()
	{
		if (m_audioEngine)
			delete m_audioEngine;
	}
};