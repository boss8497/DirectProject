#include "pch.h"
#include "AudioComponent.h"

AudioComponent::AudioComponent()
{
	this->m_CID = COMPONENT_ID::SOUND_COMPONENT;
}

AudioComponent::~AudioComponent()
{
}

AudioComponent::AudioComponent(const AudioComponent & copy)
{
}


bool AudioComponent::Inite(std::wstring path, bool loop, bool play)
{
	this->SetFilePath(path, false);
	m_audio = AudioSystem::Instance()->CreateInstance(path);
	this->Loop(loop);
	if (play)
		this->Play();

	m_audio->SetVolume(this->m_Volume);
	return true;
}

void AudioComponent::Destroy()
{
}

void AudioComponent::Update()
{
}

void AudioComponent::Update(UpdateQueue * uQueue)
{
	if (m_SoundChangeFlag)
	{
		this->Stop();
		this->DeleteSound();
		m_audio = AudioSystem::Instance()->CreateInstance(m_FilePath);
		this->Play();
		m_audio->SetVolume(this->m_Volume);
		m_SoundChangeFlag = false;
	}
}

void AudioComponent::PlayOnShot()
{
	m_audio->Play(m_loop);
}

void AudioComponent::Play()
{
	m_audio->Play(m_loop);
}

void AudioComponent::Loop(bool flag)
{
	m_loop = flag;
}

void AudioComponent::Stop(bool flag)
{
	m_audio->Stop(flag);
}

void AudioComponent::SetFilePath(std::wstring path, bool cFlag)
{
	this->m_SoundChangeFlag = cFlag;
	m_FilePath = path;
}

void AudioComponent::DeleteSound()
{
	AudioSystem::Instance()->DeleteInstance(m_audio);
}

InspacterItem * AudioComponent::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT & NextPos, float compFrameXsize)
{
	InspacterItem* FrameItem = nullptr;
	POINT FrameSize = { (LONG)compFrameXsize, 120 };
	POINT nPos = NextPos;


	//프레임 생성
	FrameItem = new InspacterItem();
	FrameItem->CreateComponentFrame(phwnd, phins, nPos, FrameSize, "Audio");


	POINT FilePathPos = nPos;
	POINT FilePathSize = { 0, };
	InspacterItem* FilePathItem = nullptr;
	char EditName[TEXT_SIZE];
	ZeroMemory(EditName, sizeof(EditName));
	sprintf_s(EditName, TEXT_SIZE, "%ws", this->m_FilePath.c_str());

	FilePathPos.x += INFO_PADDING;
	FilePathPos.y += INFO_PADDING * 2;

	FilePathSize.x = (LONG)compFrameXsize - INFO_PADDING * 2;
	FilePathSize.y = INFO_PADDING * 2;

	FilePathItem = new InspacterItem();
	FilePathItem->CreateInspecterEditBox(phwnd, phins, FilePathPos, FilePathSize
		, IDC_ITEM::EDITTMP, 0, "Path", EditName, true);

	FrameItem->m_SubItemList.push_back(FilePathItem);


	//다음 위치
	FilePathPos.y += FilePathSize.y;


	POINT BtnPos = FilePathPos;
	POINT BtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* Btn = nullptr;
	UpdateItem * BtnUpdate = nullptr;

	Btn = new InspacterItem();
	Btn->CreateInspecterButton(phwnd, phins, BtnPos, BtnSize, IDC_ITEM::BUTTONETMP1, 0, "Find File");
	FrameItem->m_SubItemList.push_back(Btn);


	//업데이트 이벤트 등록
	BtnUpdate = new UpdateItem();
	BtnUpdate->Inite(Btn->m_hwnd, (void*)FilePathItem->m_hwnd,
		UpdateType::UPDATE_BUTTON_OPEN_FILEPATH_SOUND_DIALOG, UpdateValueType::HWND_WINDOWGETTEXT_YTYPE,
		IDC_ITEM::BUTTONETMP1);

	upList->push_back(BtnUpdate);

	//다음 위치
	BtnPos.y += BtnSize.y;



	POINT StopBtnPos = BtnPos;
	POINT StopBtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* StopBtn = nullptr;
	UpdateItem * StopBtnUpdate = nullptr;

	StopBtn = new InspacterItem();
	StopBtn->CreateInspecterButton(phwnd, phins, StopBtnPos, StopBtnSize, IDC_ITEM::BUTTONETMP2, 0, "Stop");
	FrameItem->m_SubItemList.push_back(StopBtn);


	//업데이트 이벤트 등록
	StopBtnUpdate = new UpdateItem();
	StopBtnUpdate->Inite(StopBtn->m_hwnd, (void*)this,
		UpdateType::UPDATE_SOUND_STOP_BUTTON, UpdateValueType::COMPONENET_PTR_YTYPE,
		IDC_ITEM::BUTTONETMP2);

	upList->push_back(StopBtnUpdate);


	StopBtnPos.y += StopBtnSize.y;


	POINT PlayBtnPos = StopBtnPos;
	POINT PlayBtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* PlayBtn = nullptr;
	UpdateItem * PlayBtnUpdate = nullptr;

	PlayBtn = new InspacterItem();
	PlayBtn->CreateInspecterButton(phwnd, phins, PlayBtnPos, PlayBtnSize, IDC_ITEM::BUTTONETMP3, 0, "Play");
	FrameItem->m_SubItemList.push_back(PlayBtn);


	//업데이트 이벤트 등록
	PlayBtnUpdate = new UpdateItem();
	PlayBtnUpdate->Inite(PlayBtn->m_hwnd, (void*)this,
		UpdateType::UPDATE_SOUND_PLAY_BUTTON, UpdateValueType::COMPONENET_PTR_YTYPE,
		IDC_ITEM::BUTTONETMP3);

	upList->push_back(PlayBtnUpdate);


	return FrameItem;
}
