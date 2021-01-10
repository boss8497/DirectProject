#pragma once
#include"Component.h"
#include"Transform.h"
#include"TimeClass.h"

class RotateObject : public Component<BaseObject>
{
public:
	float xRotateSpeed;
	float yRotateSpeed;
	float zRotateSpeed;
	float MaxAngle;
	Transform* tr = nullptr;

public:
	void Awake()
	{
		tr = this->gameobject->GetCompponent<Transform>();
		xRotateSpeed = 0.0f;
		yRotateSpeed = 0.1f;
		zRotateSpeed = 0.0f;
		MaxAngle = 360.0f;
	}

	void Update()
	{
		if (tr != nullptr)
		{
			//Rotate x
			tr->Rotate.x = tr->Rotate.x + 
				xRotateSpeed * TimeClass::Instance().DeletaTime;
			if (tr->Rotate.x > MaxAngle)
			{
				tr->Rotate.x -= MaxAngle;
			}

			//Rotate Z
			tr->Rotate.z = tr->Rotate.z + 
				zRotateSpeed * TimeClass::Instance().DeletaTime;
			if (tr->Rotate.z > MaxAngle)
			{
				tr->Rotate.z -= MaxAngle;
			}

			//Rotate Y
			tr->Rotate.y = tr->Rotate.y + 
				yRotateSpeed * TimeClass::Instance().DeletaTime;
			if (tr->Rotate.y > MaxAngle)
			{
				tr->Rotate.y -= MaxAngle;
			}
		}
	}




	//Inspecter Custom
	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize)
	{
		POINT transFrameSize = { (LONG)compFrameXsize, 80 };
		POINT transNPos = NextPos;
		InspacterItem* itemTmp = nullptr;

		itemTmp = new InspacterItem();
		itemTmp->CreateComponentFrame(phwnd, phins, transNPos, transFrameSize, "RotateComponent");
		itemTmp->m_ComponentID = COMPONENT_ID::TRANSFORM;


		//포지션 텍스트 만들어주기
		POINT posSTextPos = transNPos;
		InspacterItem* posSText = new InspacterItem();
		posSTextPos.x += INFO_PADDING * 1;
		posSTextPos.y += INFO_PADDING * 2;
		RECT stSize = { posSTextPos.x,posSTextPos.y,(LONG)(compFrameXsize - INFO_PADDING * 2) / 2, 20 };


		posSText->CreateInspecterStaticText(phwnd, phins, stSize, "RotateSpeed");
		itemTmp->m_SubItemList.push_back(posSText);

		//다음위치 정해주기
		transNPos.y += INFO_PADDING * 2;

		//포지션 x,y,z 에디트 박스 만들기
		POINT editPos = transNPos;
		InspacterItem* editTmp = new InspacterItem();
		POINT editSize = { 0, };
		UpdateItem* upitem = nullptr;

		editPos.x += INFO_PADDING * 1;
		editPos.y += INFO_PADDING * 2;

		editSize.y = INFO_PADDING * 2;
		editSize.x = static_cast<LONG>((compFrameXsize - INFO_PADDING * 2) / 3);



		//포지션 x
		editTmp->CreateInspecterEditBox(phwnd, phins, editPos, editSize,
			IDC_ITEM::VECTERTMP5_X, 0, "X", MethodHelper::FloatToCstr(this->xRotateSpeed), false);
		itemTmp->m_SubItemList.push_back(editTmp);

		//포지션 x 업데이트 셋팅
		upitem = new UpdateItem();
		upitem->Inite(editTmp->m_hwnd, (void*)&this->xRotateSpeed,
			UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP5_X);
		upList->push_back(upitem);
		upitem = nullptr;



		//포지션 y
		editTmp = new InspacterItem();
		editPos.x += editSize.x;
		editTmp->CreateInspecterEditBox(phwnd, phins, editPos, editSize,
			IDC_ITEM::VECTERTMP5_Y, 0, "Y", MethodHelper::FloatToCstr(this->yRotateSpeed), false);
		itemTmp->m_SubItemList.push_back(editTmp);

		//포지션 y업데이트 셋팅
		upitem = new UpdateItem();
		upitem->Inite(editTmp->m_hwnd, (void*)&this->yRotateSpeed, 
			UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP5_Y);
		upList->push_back(upitem);
		upitem = nullptr;


		//포지션 z
		editTmp = new InspacterItem();
		editPos.x += editSize.x;
		editTmp->CreateInspecterEditBox(phwnd, phins, editPos, editSize,
			IDC_ITEM::VECTERTMP5_Z, 0, "Z", MethodHelper::FloatToCstr(this->zRotateSpeed), false);
		itemTmp->m_SubItemList.push_back(editTmp);

		//포지션 z업데이트 셋팅
		upitem = new UpdateItem();
		upitem->Inite(editTmp->m_hwnd, (void*)&this->zRotateSpeed, 
			UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP5_Z);
		upList->push_back(upitem);
		upitem = nullptr;

		//다음위치 정해주기
		transNPos.y += INFO_PADDING * 2;


		return itemTmp;
	}
};