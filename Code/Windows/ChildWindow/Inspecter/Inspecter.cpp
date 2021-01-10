#include "pch.h"
#include "Inspecter.h"
#include "SkyBoxClass.h"

Inspecter::Inspecter()
{
}

Inspecter::~Inspecter()
{
}

bool Inspecter::Inite(HWND phwnd)
{
	this->m_Phwnd = phwnd;

	return true;
}

void Inspecter::Destroy()
{
}

void Inspecter::Update()
{
	this->UpdateItemList();
}

bool Inspecter::CreateInspecter(_In_ HWND phwnd, _In_ HINSTANCE phins, _In_ BaseObject* object)
{
	POINT NextPos = { 0,0 };
	InspacterItem* itemTmp = nullptr;
	InspacterItem* itemFrame = nullptr;
	float compFrameXsize = 0.0f;

	m_currentObj = object;

	this->InspecterClear();

	//InvalidateRect(phwnd, NULL, TRUE);

	//Frame과 Enable은 Component가 아니라 미리 생성

	//Create Frame
	itemFrame = new InspacterItem();
	itemFrame->CreateInspecterFrame(phwnd, phins, NextPos);
	itemFrame->m_ComponentID = 0;

	m_ItemList.push_back(itemFrame);

	//윈도우 x사이즈
	compFrameXsize = GET_COMPONENTFRAME_SIZEX(itemFrame->m_WindowSize.x);


	NextPos.x += INFO_PADDING;
	NextPos.y += INFO_PADDING * 2;
	//Create ObjectName
	itemTmp = new InspacterItem();
	POINT size = { (LONG)compFrameXsize ,20 };
	UpdateItem* NameEditUpdate = new UpdateItem();

	itemTmp->CreateInspecterEditBox(phwnd, phins, NextPos, size, IDC_ITEM::NAME_EDITBOX, 0,
		"Name", object->Name.c_str(), false);


	NameEditUpdate->Inite(itemTmp->m_hwnd, (void*)object, UpdateType::UPDATE_EDITBOX_NAME,
		UpdateValueType::BASEOBJECT_YTYPE, IDC_ITEM::NAME_EDITBOX);
	this->m_UpdateItem.push_back(NameEditUpdate);
	NameEditUpdate = nullptr;


	NextPos.y += GET_NEXT_Y_POSITION(itemTmp->m_WindowSize.y);

	itemTmp->m_ComponentID = 0;
	m_ItemList.push_back(itemTmp);
	itemTmp = nullptr;




	//Create Enable
	itemTmp = new InspacterItem();
	itemTmp->CreateInspecterEnable(phwnd, phins, object->Enable, "Enable", &(itemFrame->m_WindowSize), NextPos);
	itemTmp->m_ComponentID = 0;

	m_ItemList.push_back(itemTmp);
	itemTmp = nullptr;




	for (auto iter = object->GetComponentMap()->rbegin(); iter != object->GetComponentMap()->rend(); ++iter)
	{
		///////////
		itemTmp = (*iter).second->CreateInspecter(&this->m_UpdateItem, phwnd, phins, NextPos, compFrameXsize);
		m_ItemList.push_back(itemTmp);

		//다음위치 정해주기
		NextPos.y = GET_NEXT_Y_POSITION(NextPos.y + itemTmp->m_WindowSize.y);
	}

	return true;
}

void Inspecter::InspecterEnableEvent(InspacterItem * cItem)
{
	if (SendMessage(cItem->m_hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED)
	{
		this->m_currentObj->Enable = true;
	}
	else if (SendMessage(cItem->m_hwnd, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
	{
		this->m_currentObj->Enable = false;
	}
}

void Inspecter::InspecterClear()
{
	UpdateItemSubProc::ResetSubProc();
	this->ClearItemList();
	this->ClearUpdateItem();
}

void Inspecter::ClearItemList()
{
	if (!m_ItemList.empty())
	{
		for (auto iter = m_ItemList.begin(); iter != m_ItemList.end();)
		{
			InspacterItem* tmp = (*iter);
			tmp->Destroy();
			iter = m_ItemList.erase(iter);
			delete tmp;
		}
	}
}

InspacterItem * Inspecter::FindInspecterItem(int idc)
{
	InspacterItem * tmp = nullptr;

	if (!m_ItemList.empty())
	{
		for (auto iter = m_ItemList.begin(); iter != m_ItemList.end(); ++iter)
		{
			if ((*iter)->m_IDC == idc)
			{
				tmp = (*iter);
				break;
			}
			tmp = (*iter)->FindItem(idc);
			if (tmp)
			{
				break;
			}
		}
	}

	return tmp;
}

void Inspecter::ClearUpdateItem()
{
	if (!m_UpdateItem.empty())
	{
		for (auto iter = m_UpdateItem.begin(); iter != m_UpdateItem.end();)
		{
			UpdateItem* tmp = (*iter);
			tmp->Destroy();
			iter = m_UpdateItem.erase(iter);
			delete tmp;
		}
	}
}

void Inspecter::UpdateItemList()
{
	for (auto iter = m_UpdateItem.begin(); iter != m_UpdateItem.end(); ++iter)
	{
		(*iter)->Update();
	}

}

UpdateItem * Inspecter::FindUpdateItem(int idc)
{
	UpdateItem* tmp = nullptr;

	if (!m_UpdateItem.empty())
	{

		for (auto iter = m_UpdateItem.begin(); iter != m_UpdateItem.end(); ++iter)
		{
			if ((*iter)->m_IDC == idc)
			{
				tmp = (*iter);
				break;
			}
		}

	}

	return tmp;
}

void Inspecter::InspecterEvent(HWND phwnd, WPARAM wparam, LPARAM lparam)
{
	InspacterItem* currentItem = this->FindInspecterItem(LOWORD(wparam));
	
	switch (LOWORD(wparam))
	{
	case GROUP_PARENT:
		break;
	case TRANSFORM_PARENT:
		break;

	case ENABLE_CHECKBOX:
	case ENABLE_CHECKBOX2:
	case ENABLE_CHECKBOX3:
	case ENABLE_CHECKBOX4:
	case ENABLE_CHECKBOX5:
	case ENABLE_CHECKBOX6:
	case ENABLE_CHECKBOX7:
	case ENABLE_CHECKBOX8:
		this->InspecterEnableEvent(currentItem);
		break;


	case TRANSFORM_POS_X:
	case TRANSFORM_POS_Y:
	case TRANSFORM_POS_Z:
	case TRANSFORM_ROT_X:
	case TRANSFORM_ROT_Y:
	case TRANSFORM_ROT_Z:
	case VECTERTMP_X:
	case VECTERTMP_Y:
	case VECTERTMP_Z:
	case VECTERTMP_W:
	case VECTERTMP2_X:
	case VECTERTMP2_Y:
	case VECTERTMP2_Z:
	case VECTERTMP2_W:
	case VECTERTMP3_X:
	case VECTERTMP3_Y:
	case VECTERTMP3_Z:
	case VECTERTMP3_W:
	case VECTERTMP4_X:
	case VECTERTMP4_Y:
	case VECTERTMP4_Z:
	case VECTERTMP4_W:
	case VECTERTMP5_X:
	case VECTERTMP5_Y:
	case VECTERTMP5_Z:
	case VECTERTMP5_W:
	case VECTERTMP6_X:
	case VECTERTMP6_Y:
	case VECTERTMP6_Z:
	case VECTERTMP6_W:
	case VECTERTMP7_X:
	case VECTERTMP7_Y:
	case VECTERTMP7_Z:
	case VECTERTMP7_W:
	case VECTERTMP8_X:
	case VECTERTMP8_Y:
	case VECTERTMP8_Z:
	case VECTERTMP8_W:
	{
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
		{
			break;
		}
		case EN_SETFOCUS:
		{
			UpdateItem* uitem = this->FindUpdateItem(LOWORD(wparam));
			if (uitem != nullptr)
			{
				UpdateItemSubProc::SetSubProc(uitem);
			}
		}
		break;


		}//end switch
	}
		break;



	case NAME_EDITBOX:
	{
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
		{
			break;
		}
		case EN_SETFOCUS:
		{
			UpdateItem* uitem = this->FindUpdateItem(LOWORD(wparam));
			if (uitem != nullptr)
			{
				UpdateItemSubProc::SetSubProc(uitem);
			}
		}
		break;


		}//end switch
	}
	break;


	case EDITTMP:
	case EDITTMP2:
	case EDITTMP3:
	case EDITTMP4:
	case EDITTMP5:
	case EDITTMP6:
	case EDITTMP7:
	case EDITTMP8:
	{
		switch (HIWORD(wparam))
		{
		case EN_CHANGE:
		{
			break;
		}
		case EN_SETFOCUS:
		{
			UpdateItem* uitem = this->FindUpdateItem(LOWORD(wparam));
			if (uitem != nullptr)
			{
				UpdateItemSubProc::SetSubProc(uitem);
			}
		}
		break;


		}//end switch
	}
	break;
		break;




	case FILEPATH:
	case FILEPATH2:
	case FILEPATH3:
	case FILEPATH4:
	case FILEPATH5:
	case FILEPATH6:
	case FILEPATH7:
	case FILEPATH8:
		break;




	case COLOR_SELECT:
	case COLOR_SELECT1:
	case COLOR_SELECT2:
	case COLOR_SELECT3:
	case COLOR_SELECT4:
	case COLOR_SELECT5:
	case COLOR_SELECT6:
	case COLOR_SELECT7:
	case COLOR_SELECT8:
		break;




	case BUTTONETMP1:
	case BUTTONETMP2:
	case BUTTONETMP3:
	case BUTTONETMP4:
	case BUTTONETMP5:
	case BUTTONETMP6:
	case BUTTONETMP7:
	case BUTTONETMP8:
	{
		UpdateItem * uitem = this->FindUpdateItem(LOWORD(wparam));
		if (uitem != nullptr)
		{
			switch (uitem->m_Type)
			{
			case UPDATE_TEXT_EDIT:
				break;
			case UPDATE_BUTTON_OPEN_DIALOG:
				break;
			case UPDATE_BUTTON_OPEN_FILEPATH_DIALOG:
			{
				if (uitem->UpdateButtonEvent())
				{
					currentItem = this->FindInspecterItem(LOWORD(wparam));
					if (currentItem != nullptr)
					{
						switch (currentItem->m_ComponentID)
						{
						case DISABLE:
							break;
						case TRANSFORM:
							break;
						case MATERIAL:
						{
							char FilePath[255];
							ZeroMemory(FilePath, sizeof(FilePath));
							sprintf_s(FilePath, "%ws", uitem->m_PreviousText);
							this->m_currentObj->GetCompponent<Material>()->SetTextureFilePath(FilePath);
						}
							break;
						case TEXTURE2D:
						{
							char FilePath[255];
							ZeroMemory(FilePath, sizeof(FilePath));
							sprintf_s(FilePath, "%ws", uitem->m_PreviousText);
							this->m_currentObj->GetCompponent<Texture2D>()->SetTextureFilePath(FilePath);
						}
						break;

						case SKYBOXCOMPONENT:
						{
							char FilePath[255];
							ZeroMemory(FilePath, sizeof(FilePath));
							sprintf_s(FilePath, "%ws", uitem->m_PreviousText);
							this->m_currentObj->GetCompponent<SkyBoxClass>()->SetTextureFilePath(FilePath);
						}
						break;


						case SOUND_COMPONENT:
						{
							this->m_currentObj->GetCompponent<AudioComponent>()->SetFilePath(uitem->m_PreviousText);
						}
						break;


						default:
							break;
						}
					}
				}
			}
			break;

			case UPDATE_BUTTON_OPEN_FILEPATH_SOUND_DIALOG:
			{
				if (uitem->UpdateButtonEvent())
				{
					this->m_currentObj->GetCompponent<AudioComponent>()->SetFilePath(uitem->m_PreviousText);
				}
			}
			break;

			case UPDATE_BUTTON_OPEN_COLOR_DIALOG:
			{
				if (uitem->UpdateButtonEvent())
				{

				}

			}
			break;


			case UPDATE_FILEPATH_EDIT:
				break;

			case UPDATE_SOUND_STOP_BUTTON:
			case UPDATE_SOUND_PLAY_BUTTON:
				if (uitem->UpdateButtonEvent())
				{

				}
				break;


			default:
				break;
			}
		}

	}
	break;



	default:
		break;
	}
}
