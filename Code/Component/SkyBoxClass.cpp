#include"pch.h"
#include"SkyBoxClass.h"


SkyBoxClass::SkyBoxClass()
{
	this->m_CID = SKYBOXCOMPONENT;
}

SkyBoxClass::~SkyBoxClass()
{
}

bool SkyBoxClass::Inite(ID3D11Device * device, HWND hwnd)
{
	m_SMesh = new SkyBoxMesh();
	m_SMesh->Inite(device);

	m_NShader = new NoLightShader();
	m_NShader->Inite(device, hwnd, Default_NOLIGHT_VS, Default_NOLIGHT_PS);


	//텍스쳐 초기화
	if (m_Texture == nullptr)
	{
		//Default_BOX_SkyBox_Texture : Cube
		//Default_SkyBox_Texture : Sphere
		this->SetTextureFilePath(Default_BOX_SkyBox_Texture);
		m_ChangeFlag = false;
		m_Texture = new Texture();
		if (!m_Texture->Inite(device, m_Textrue_FilePath))
		{
			printf("Textrue Inite Error");
			return false;
		}
	}

	return true;
}

void SkyBoxClass::Update(UpdateQueue * uQueue)
{
	if (this->m_ChangeFlag)
	{
		if (this->m_Texture != nullptr)
		{
			/*1. 먼저 어떤 파일명이 바뀌었는지 찾는다*/
			/*파일명이 변경이 됬다면 다시 초기화 한다*/
			if (std::strcmp(this->m_Texture->GetFilePath(), m_Textrue_FilePath) != 0)
			{
				m_Texture->SetFileName(m_Textrue_FilePath);
				uQueue->Push(m_Texture);
			}


			this->m_ChangeFlag = false;
		}
	}
}

void SkyBoxClass::Destroy()
{
	if (m_SMesh)
	{
		m_SMesh->Destroy();
		delete m_SMesh;
		m_SMesh = nullptr;
	}

	if (m_NShader)
	{
		m_NShader->Destroy();
		delete m_NShader;
		m_NShader = nullptr;
	}

	if (m_Texture)
	{
		m_Texture->Destroy();
		delete m_Texture;
		m_Texture = nullptr;
	}
}

bool SkyBoxClass::Render(ID3D11DeviceContext * deviceContext, Transform * tr, DirectionalLight * light)
{
	m_SMesh->Render(deviceContext);

	m_NShader->SetShaderParameters(deviceContext, m_Texture->GetTexture(), 
		this->gameobject->GetCompponent<Transform>());

	m_NShader->RenderShader(deviceContext, m_SMesh->GetIndexCount());


	return true;
}

void SkyBoxClass::SetTextureFilePath(const char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, Default_Texture);
	}
	else
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}

InspacterItem * SkyBoxClass::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT & NextPos, float compFrameXsize)
{
	InspacterItem* SkyBoxFrame = nullptr;
	POINT FrameSize = { (LONG)compFrameXsize, 150 };
	POINT nPos = NextPos;


	//프레임 생성
	SkyBoxFrame = new InspacterItem();
	SkyBoxFrame->CreateComponentFrame(phwnd, phins, nPos, FrameSize, "SkyBox");


	POINT FilePathPos = nPos;
	POINT FilePathSize = { 0, };
	InspacterItem* FilePathItem = nullptr;

	FilePathPos.x += INFO_PADDING;
	FilePathPos.y += INFO_PADDING * 2;

	FilePathSize.x = (LONG)compFrameXsize - INFO_PADDING * 2;
	FilePathSize.y = INFO_PADDING * 2;

	FilePathItem = new InspacterItem();
	FilePathItem->CreateInspecterEditBox(phwnd, phins, FilePathPos, FilePathSize
		, IDC_ITEM::EDITTMP, 0, "Path", this->m_Textrue_FilePath, true);

	SkyBoxFrame->m_SubItemList.push_back(FilePathItem);



	//다음 위치
	FilePathPos.y += FilePathSize.y;

	POINT BtnPos = FilePathPos;
	POINT BtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* Btn = nullptr;
	UpdateItem * BtnUpdate = nullptr;

	Btn = new InspacterItem();
	Btn->CreateInspecterButton(phwnd, phins, BtnPos, BtnSize, IDC_ITEM::BUTTONETMP1, COMPONENT_ID::SKYBOXCOMPONENT, "Find Texture File");
	SkyBoxFrame->m_SubItemList.push_back(Btn);


	//업데이트 이벤트 등록
	BtnUpdate = new UpdateItem();
	BtnUpdate->Inite(Btn->m_hwnd, (void*)FilePathItem->m_hwnd,
		UpdateType::UPDATE_BUTTON_OPEN_FILEPATH_DIALOG, UpdateValueType::HWND_WINDOWGETTEXT_YTYPE,
		IDC_ITEM::BUTTONETMP1);

	upList->push_back(BtnUpdate);


	return SkyBoxFrame;
}
