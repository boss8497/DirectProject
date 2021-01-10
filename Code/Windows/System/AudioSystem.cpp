#include "pch.h"
#include "AudioSystem.h"

void AudioSystem::Destroy()
{
	this->DestroyIntanceList();
	this->DestroySoundMap();
}

bool AudioSystem::ChangeDevice(int _index)
{
	/*index 0 = auto*/
	int index = _index - 1;

	if (0 > index)
	{
		printf("AudioSystem : ChangeDevice index -1\n");
		return false;
	}

	m_audioEngine->Reset(nullptr, m_deviceList[index].deviceId.c_str());

	this->OnNewAudioDevice(true);

	return true;
}

void AudioSystem::RefreshDeviceList()
{
	m_deviceList = AudioEngine::GetRendererDetails();
}

void AudioSystem::OnNewAudioDevice(bool flag)
{
	this->m_retryAudio = flag;
}

SoundEffectInstance* AudioSystem::CreateInstance(std::wstring path)
{
	SoundEffect* tmp = m_Soundmap[path];
	if (tmp == nullptr)
	{
		tmp = new DirectX::SoundEffect(m_audioEngine, path.c_str());
		m_Soundmap.insert(std::make_pair(path, tmp));
	}

	m_InstanceList.push_back(tmp->CreateInstance());
	return m_InstanceList.back().get();
}

void AudioSystem::DeleteInstance(SoundEffectInstance * ptr)
{
	for (auto it = m_InstanceList.begin(); it != m_InstanceList.end(); ++it)
	{
		if ((*it).get() == ptr)
		{
			(*it).reset();
			(*it).release();
			m_InstanceList.erase(it);
			break;
		}
	}
}

void AudioSystem::Update()
{
	bool loop = false;
	if (m_retryAudio)
	{
		m_retryAudio = false;
		if (m_audioEngine->Reset())
		{

			for (auto it = m_InstanceList.begin(); it != m_InstanceList.end(); ++it)
			{
				loop = (*it)->IsLooped();
				(*it)->Play(loop);
			}
		}
	}
	else if (!m_audioEngine->Update())
	{
		if (m_audioEngine->IsCriticalError())
		{
			/*오디오 장치를 잃어버렸을때*/
			m_retryAudio = true;
		}
	}
}

void AudioSystem::DestroyIntanceList()
{
	for (auto it = m_InstanceList.begin(); it != m_InstanceList.end();)
	{
		(*it).release();
		it = m_InstanceList.erase(it);
	}
}

void AudioSystem::DestroySoundMap()
{
	for (auto it = m_Soundmap.begin(); it != m_Soundmap.end();)
	{
		delete (*it).second;
		it = m_Soundmap.erase(it);
	}
}
