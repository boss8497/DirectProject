#include "pch.h"
#include "UpdateItem.h"
#include "AudioComponent.h"

UpdateItem::UpdateItem()
{
	ZeroMemory(m_PreviousText, sizeof(m_PreviousText));
	ZeroMemory(m_NowText, sizeof(m_NowText));
}

UpdateItem::~UpdateItem()
{
}

bool UpdateItem::Inite(HWND hwnd, void * uptr, _In_ UpdateType type, _In_ UpdateValueType valuetype, _In_ int idc)
{
	m_Currhwnd = hwnd;
	m_ValueType = valuetype;
	m_UpdatePointer = uptr;
	m_IDC = idc;
	m_Type = type;


	switch (m_Type)
	{
	case UPDATE_TEXT_EDIT:
		this->SetNowText();
		SetWindowText(m_Currhwnd, m_NowText);
		memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));

		break;
	case UPDATE_BUTTON_OPEN_DIALOG:
		break;

	case UPDATE_BUTTON_OPEN_FILEPATH_DIALOG:
		this->SetNowText();
		memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
		break;

	case UPDATE_BUTTON_OPEN_FILEPATH_SOUND_DIALOG:
		this->SetNowText();
		memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
		break;


	case UPDATE_FILEPATH_EDIT:
		break;


	case UPDATE_BUTTON_OPEN_COLOR_DIALOG:

		break;

	case UPDATE_EDITBOX_NAME:
		this->SetNowText();
		memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
		break;


	case UpdateType::UPDATE_ENABLE_CHECK_BOX:
		break;

	default:
		break;
	}

	return true;
}

void UpdateItem::Update()
{
	switch (this->m_Type)
	{

	case UPDATE_TEXT_EDIT:
	{
		ZeroMemory(m_NowText, sizeof(m_NowText));
		this->SetNowText();
		if (wcscmp(m_PreviousText, m_NowText) != 0)
		{
			SetWindowText(m_Currhwnd, m_NowText);
			ZeroMemory(m_PreviousText, sizeof(m_PreviousText));
			memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
		}
	}
	break;

	case UPDATE_EDITBOX_NAME:
	{
		ZeroMemory(m_NowText, sizeof(m_NowText));
		this->SetNowText();
		if (wcscmp(m_PreviousText, m_NowText) != 0)
		{
			SetWindowText(m_Currhwnd, m_NowText);
			ZeroMemory(m_PreviousText, sizeof(m_PreviousText));
			memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
			HWND thwnd = ((BaseObject*)m_UpdatePointer)->Hierarchy.m_Thwnd;
			if (thwnd != nullptr)
			{
				TVITEM tvi;
				WCHAR objectName[255];

				memset(&tvi, 0, sizeof(tvi));
				memset(objectName, 0, 256);
				tvi.mask = TVIF_TEXT;
				tvi.pszText = objectName;
				tvi.cchTextMax = 256;
				tvi.hItem = ((BaseObject*)m_UpdatePointer)->Hierarchy.m_Item;

				SendMessage(thwnd, TVM_GETITEM, 0,
					(LPARAM)&tvi);

				tvi.pszText = m_PreviousText;
				tvi.cchTextMax = 256;
				SendMessage(thwnd, TVM_SETITEM, 0,
					(LPARAM)&tvi);
			}
		}

	}
	break;





	case UPDATE_BUTTON_OPEN_DIALOG:
		break;



	case UPDATE_BUTTON_OPEN_FILEPATH_DIALOG:
		break;


	case UPDATE_FILEPATH_EDIT:
		break;



	default:
		break;


	}
	
}

void UpdateItem::Destroy()
{
}

void UpdateItem::SetNowText()
{
	ZeroMemory(m_NowText, sizeof(m_NowText));

	switch (m_ValueType)
	{
	case INT_YTYPE:
		wsprintf(m_NowText, L"%d", *(int*)m_UpdatePointer);
		break;
	case FLOAT_YTYPE:
		_stprintf_s(m_NowText, sizeof(m_NowText), L"%f", *(float*)m_UpdatePointer);
		break;
	case LONG_YTYPE:
		wsprintf(m_NowText, L"%ld", *(LONG*)m_UpdatePointer);
		break;
	case DOUBLE_YTYPE:
		_stprintf_s(m_NowText, sizeof(m_NowText), L"%lf", *(double*)m_UpdatePointer);
		break;
	case CHAR_YTYPE:
		break;
	case CSTR_YTYPE:
	{

		wchar_t* wcName = MethodHelper::GetWC((char*)m_UpdatePointer);
		size_t size = sizeof(m_NowText) / 2;
		wcscpy_s(m_NowText,size, wcName);
		delete[] wcName;
	}
	break;

	case STRING_YTYPE:
	{

	}
	break;

	case BASEOBJECT_YTYPE:
	{
		wchar_t* wcName = MethodHelper::GetWC(((BaseObject*)m_UpdatePointer)->Name.c_str());

		size_t size = sizeof(m_NowText) / 2;
		wcscpy_s(m_NowText, size, wcName);
		delete[] wcName;
	}
	break;


	case HWND_WINDOWGETTEXT_YTYPE:
		GetWindowText((HWND)m_UpdatePointer, m_NowText, TEXT_SIZE);
		break;

	default:
		break;
	}
}

void UpdateItem::InspecterEditBoxUpdate()
{
	ZeroMemory(m_NowText, sizeof(m_NowText));
	GetWindowText(m_Currhwnd, m_NowText, sizeof(m_NowText));

	if (wcscmp(m_PreviousText, m_NowText) != 0)
	{
		switch (m_ValueType)
		{
		case INT_YTYPE:
			if (!MethodHelper::IsAlpha(m_NowText))
			{
				*(int*)m_UpdatePointer = _wtoi(m_NowText);
				memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
				SetWindowText(m_Currhwnd, m_PreviousText);
			}
			else
			{
				SetWindowText(m_Currhwnd, m_PreviousText);
			}

			break;


		case FLOAT_YTYPE:
			if (!MethodHelper::IsAlpha(m_NowText))
			{
				*(float*)m_UpdatePointer = wcstof(m_NowText, nullptr);
				memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
				SetWindowText(m_Currhwnd, m_PreviousText);
			}
			else
			{
				SetWindowText(m_Currhwnd, m_PreviousText);
			}
			break;


		case LONG_YTYPE:
			break;
		case DOUBLE_YTYPE:
			break;
		case CHAR_YTYPE:
			break;
		case CSTR_YTYPE:
		{
			if (wcscmp(m_NowText, m_PreviousText) != 0)
			{
				size_t strSize = TEXT_SIZE;
				char* UpdateCharptr = (char*)m_UpdatePointer;
				ZeroMemory(UpdateCharptr, strSize);
				sprintf_s(UpdateCharptr, strSize, "%ws", m_NowText);
			}

		}
		break;

		case STRING_YTYPE:
			break;

		case BASEOBJECT_YTYPE:
		{
			switch (this->m_Type)
			{
			case UPDATE_EDITBOX_NAME:
			{
				if (wcscmp(m_NowText, m_PreviousText) != 0)
				{
					//오브젝트 이름 바꿔주고
					((BaseObject*)m_UpdatePointer)->Name.assign(m_NowText, &m_NowText[wcslen(m_NowText)]);


					//에디트 이름 바꿔주고
					memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
					SetWindowText(m_Currhwnd, m_PreviousText);


					//트리 이름 바꿔주기
					HWND thwnd = ((BaseObject*)m_UpdatePointer)->Hierarchy.m_Thwnd;
					TVITEM tvi;
					WCHAR objectName[255];

					memset(&tvi, 0, sizeof(tvi));
					memset(objectName, 0, 256);
					tvi.mask = TVIF_TEXT;
					tvi.pszText = objectName;
					tvi.cchTextMax = 256;
					tvi.hItem = ((BaseObject*)m_UpdatePointer)->Hierarchy.m_Item;

					SendMessage(thwnd, TVM_GETITEM, 0,
						(LPARAM)&tvi);

					tvi.pszText = m_PreviousText;
					tvi.cchTextMax = 256;
					SendMessage(thwnd, TVM_SETITEM, 0,
						(LPARAM)&tvi);
				}
				else
				{
					SetWindowText(m_Currhwnd, m_PreviousText);
				}
			}
			break;
			}//end switch
		}
		break;

		default:
			break;
		}


	}
	else
	{
		printf("Eqqual\n");
	}
}

bool UpdateItem::UpdateButtonEvent()
{
	switch (m_Type)
	{
	case UPDATE_BUTTON_OPEN_DIALOG:
		break;



	case UPDATE_BUTTON_OPEN_FILEPATH_DIALOG:
	{
		OPENFILENAME OFN;
		ZeroMemory(&OFN, sizeof(OPENFILENAME));

		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = this->m_Currhwnd;
		//OFN.lpstrFilter = L"ALL Files(*.*)\0*.*\0";
		OFN.lpstrFilter = L"(*.jpg*;.png*;.dds)\0*.jpg;*.png;*.dds";
		OFN.lpstrFile = m_NowText;
		OFN.nMaxFile = TEXT_SIZE;

		//GetOpenFileName 이 절대 경로를 바꿔버린다 그래서 다시 원상 복귀해줘야됨
		wchar_t currDirec[TEXT_SIZE];
		ZeroMemory(currDirec, sizeof(currDirec));
		GetCurrentDirectory(TEXT_SIZE, currDirec);

		if (0 != GetOpenFileName(&OFN))
		{
			if (wcscmp(m_PreviousText, m_NowText) != 0)
			{
				SetWindowText((HWND)m_UpdatePointer, m_NowText);
				ZeroMemory(m_PreviousText, sizeof(m_PreviousText));
				memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
				SetCurrentDirectory(currDirec);
				return true;
			}
		}
		SetCurrentDirectory(currDirec);

	}
	break;

	case UPDATE_BUTTON_OPEN_FILEPATH_SOUND_DIALOG:
	{
		OPENFILENAME OFN;
		ZeroMemory(&OFN, sizeof(OPENFILENAME));

		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = this->m_Currhwnd;
		//OFN.lpstrFilter = L"ALL Files(*.*)\0*.*\0";
		OFN.lpstrFilter = L"(*.wav)\0*.wav";
		OFN.lpstrFile = m_NowText;
		OFN.nMaxFile = TEXT_SIZE;

		wchar_t currDirec[TEXT_SIZE];
		ZeroMemory(currDirec, sizeof(currDirec));
		GetCurrentDirectory(TEXT_SIZE, currDirec);

		if (0 != GetOpenFileName(&OFN))
		{
			if (wcscmp(m_PreviousText, m_NowText) != 0)
			{
				SetWindowText((HWND)m_UpdatePointer, m_NowText);
				ZeroMemory(m_PreviousText, sizeof(m_PreviousText));
				memcpy(m_PreviousText, m_NowText, sizeof(m_NowText));
				SetCurrentDirectory(currDirec);
				return true;
			}
		}

		SetCurrentDirectory(currDirec);
	}
	break;


	case UPDATE_BUTTON_OPEN_COLOR_DIALOG:
	{
		CHOOSECOLOR cColor;
		COLORREF colorTmp[16];

		ZeroMemory(&cColor, sizeof(CHOOSECOLOR));
		cColor.lStructSize = sizeof(CHOOSECOLOR);
		cColor.hwndOwner = this->m_Currhwnd;
		cColor.lpCustColors = colorTmp;
		cColor.Flags = 0;
		if (ChooseColor(&cColor) != 0)
		{
			Vector::Vector4 vColor;
			vColor.x = GetColorR(cColor.rgbResult) / 255.0f;
			vColor.y = GetColorG(cColor.rgbResult) / 255.0f;
			vColor.z = GetColorB(cColor.rgbResult) / 255.0f;
			*(Vector::Vector4*)m_UpdatePointer = vColor;
		}
	}
	break;


	case UPDATE_SOUND_STOP_BUTTON:
	{
		((AudioComponent*)m_UpdatePointer)->Stop();
	}
	break;


	case UPDATE_SOUND_PLAY_BUTTON:
	{
		((AudioComponent*)m_UpdatePointer)->Play();
	}
	break;


	default:
		break;
	}


	return false;
}
