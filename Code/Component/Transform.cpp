#include"pch.h"
#include "Transform.h"
#include"MethodHelper.h"

Transform::Transform()
{
	this->m_CID = COMPONENT_ID::TRANSFORM;
}

Transform::Transform(BaseObject* obj)
{
	this->Scale = Vector::Vector3::Identity();
	this->Position = Vector::Vector3::Zero();
	this->Rotate = Vector::Vector3::Zero();
	this->Size = Vector::Vector3::Identity();
	this->gameobject = obj;
	this->m_CID = COMPONENT_ID::TRANSFORM;
}

Transform::~Transform()
{

}

bool Transform::Inite()
{
	return true;
}

void Transform::Update(UpdateQueue* uQueue)
{
}

void Transform::Destroy()
{
}

Vector::Vector3 Transform::GetForword()
{
	Matrix::Matrix rMatrix = Matrix::Matrix::Rotation(this->Rotate);
	Vector::Vector3 fVec = { 0.0f,0.0f,1.0f };
	Vector::Vector3 result = Matrix::Matrix::Multiply(fVec, rMatrix);

	return result;
}

void Transform::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

void Transform::SetPosition(Vector::Vector3 v)
{
	Position = v;
}

void Transform::SetRotate(float x, float y, float z)
{
	Rotate.x = x;
	Rotate.y = y;
	Rotate.z = z;
}

void Transform::SetRotate(Vector::Vector3 v)
{
	Rotate = v;
}

void Transform::SetScale(float x, float y, float z)
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

void Transform::SetScale(Vector::Vector3 v)
{
	Scale = v;
}

void Transform::SetSize(float x, float y)
{
	this->Size.x = x;
	this->Size.y = y;
}

void Transform::SetSize(Vector::Vector3 v)
{
	this->Size = v;
}

void Transform::SetWorldMatrix_UI()
{
	Vector::Vector3 ss = Vector::Vector3::Mul(Size, Scale);
	World = Matrix::Matrix::Multiply(Matrix::Matrix::Multiply(Transform::World_Matrix, (Matrix::Matrix::Scaling(ss)/2.0f))
		, Matrix::Matrix::Multiply(Matrix::Matrix::Rotation(Rotate), Matrix::Matrix::Translation(Position)));
}

void Transform::WorldMatrixImportSRT(Matrix::Matrix& in_m)
{
	XMMATRIX m = XMLoadFloat4x4(&in_m);
	XMVECTOR r, s, t;
	XMMatrixDecompose(&s, &r, &t, m);
	Vector::Vector4 quater(r);
	Vector::Vector3 euler;
	MethodHelper::QuaterinonToEuler(quater, euler);



	XMStoreFloat3(&Position, t);
	Rotate = euler;
	Rotate.x = RadianToDegree(euler.x);
	Rotate.y = RadianToDegree(euler.y);
	Rotate.z = RadianToDegree(euler.z);
	XMStoreFloat3(&Scale, s);
	XMStoreFloat3(&Size, s);
}

Matrix::Matrix Transform::GetWorldMatrix()
{
	/*재귀 호출을 줄이기 위해 사용*/
	//ex)    Wolrd = Local * Parent->GetWorldMatrix();
	//자식 트리 깊이가 100이면  스택 100개 쌓임
	std::vector<BaseObject*> ParentVector;
	BaseObject* Parent = this->gameobject->GetParent();
	Matrix::Matrix LocalMat = this->GetLocalMatrix();
	Matrix::Matrix ParentMat;

	if (Parent)
	{
		while (Parent)
		{
			ParentVector.push_back(Parent);
			Parent = Parent->GetParent();
		}

		for (int i = ParentVector.size() - 1; i >= 0; --i)
		{
			Matrix::Matrix tmp = ParentVector[i]->GetCompponent<Transform>()->GetLocalMatrix();
			ParentMat = Matrix::Matrix::Multiply(tmp, ParentMat);
		}
		LocalMat = Matrix::Matrix::Multiply(LocalMat, ParentMat);
	}

	ParentVector.clear();

	return LocalMat;
}

Matrix::Matrix Transform::GetLocalMatrix()
{
	Matrix::Matrix Local = Matrix::Matrix::SRT(Scale, Rotate, Position);
	return Local;
}

BaseObject * Transform::GameObject()
{
	return (BaseObject*)gameobject;
}



InspacterItem * Transform::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT & NextPos, float compFrameXsize)
{
	POINT transFrameSize = { (LONG)compFrameXsize, 80 };
	POINT transNPos = NextPos;
	InspacterItem* itemTmp = nullptr;

	itemTmp = new InspacterItem();
	itemTmp->CreateComponentFrame(phwnd, phins, transNPos, transFrameSize, "Transform");
	itemTmp->m_ComponentID = COMPONENT_ID::TRANSFORM;


	//포지션 텍스트 만들어주기
	POINT posSTextPos = transNPos;
	InspacterItem* posSText = new InspacterItem();
	posSTextPos.x += INFO_PADDING * 1;
	posSTextPos.y += INFO_PADDING * 2;
	RECT stSize = { posSTextPos.x,posSTextPos.y,(LONG)(compFrameXsize - INFO_PADDING * 2) / 2, 20 };


	posSText->CreateInspecterStaticText(phwnd, phins, stSize, "Position");
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
		IDC_ITEM::TRANSFORM_POS_X, 0, "X", MethodHelper::FloatToCstr(this->Position.x), false);
	itemTmp->m_SubItemList.push_back(editTmp);

	//포지션 x 업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editTmp->m_hwnd, (void*)&this->Position.x, 
		UpdateType::UPDATE_TEXT_EDIT ,UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_POS_X);
	upList->push_back(upitem);
	upitem = nullptr;



	//포지션 y
	editTmp = new InspacterItem();
	editPos.x += editSize.x;
	editTmp->CreateInspecterEditBox(phwnd, phins, editPos, editSize,
		IDC_ITEM::TRANSFORM_POS_Y, 0, "Y", MethodHelper::FloatToCstr(this->Position.y), false);
	itemTmp->m_SubItemList.push_back(editTmp);

	//포지션 y업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editTmp->m_hwnd, (void*)&this->Position.y, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_POS_Y);
	upList->push_back(upitem);
	upitem = nullptr;


	//포지션 z
	editTmp = new InspacterItem();
	editPos.x += editSize.x;
	editTmp->CreateInspecterEditBox(phwnd, phins, editPos, editSize,
		IDC_ITEM::TRANSFORM_POS_Z, 0, "Z", MethodHelper::FloatToCstr(this->Position.z), false);
	itemTmp->m_SubItemList.push_back(editTmp);

	//포지션 z업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editTmp->m_hwnd, (void*)&this->Position.z, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_POS_Z);
	upList->push_back(upitem);
	upitem = nullptr;

	//다음위치 정해주기
	transNPos.y += INFO_PADDING * 2;





	//로테이션 텍스트 만들어주기
	POINT posSRotPos = transNPos;
	InspacterItem* posRText = new InspacterItem();
	posSRotPos.x += INFO_PADDING * 1;
	posSRotPos.y += INFO_PADDING * 2;
	RECT strSize = { posSRotPos.x,posSRotPos.y,(LONG)(compFrameXsize - INFO_PADDING * 2) / 2, 20 };


	posRText->CreateInspecterStaticText(phwnd, phins, strSize, "Rotatetion");
	itemTmp->m_SubItemList.push_back(posRText);

	//다음위치 정해주기
	transNPos.y += INFO_PADDING * 2;

	//로테이션 x,y,z 에디트 박스 만들기
	POINT editRPos = transNPos;
	InspacterItem* editRTmp = new InspacterItem();
	POINT editRSize = { 0, };

	editRPos.x += INFO_PADDING * 1;
	editRPos.y += INFO_PADDING * 2;

	editRSize.y = INFO_PADDING * 2;
	editRSize.x = static_cast<LONG>((compFrameXsize - INFO_PADDING * 2) / 3);

	//로테이션 x
	editRTmp->CreateInspecterEditBox(phwnd, phins, editRPos, editRSize,
		IDC_ITEM::TRANSFORM_ROT_X, 0, "X", MethodHelper::FloatToCstr(this->Rotate.x), false);
	itemTmp->m_SubItemList.push_back(editRTmp);
	//로테이션 x업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editRTmp->m_hwnd, (void*)&this->Rotate.x, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_ROT_X);
	upList->push_back(upitem);
	upitem = nullptr;


	//로테이션 y
	editRTmp = new InspacterItem();
	editRPos.x += editRSize.x;
	editRTmp->CreateInspecterEditBox(phwnd, phins, editRPos, editRSize,
		IDC_ITEM::TRANSFORM_ROT_Y, 0, "Y", MethodHelper::FloatToCstr(this->Rotate.y), false);
	itemTmp->m_SubItemList.push_back(editRTmp);
	//로테이션 y업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editRTmp->m_hwnd, (void*)&this->Rotate.y, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_ROT_Y);
	upList->push_back(upitem);
	upitem = nullptr;



	//로테이션 z
	editRTmp = new InspacterItem();
	editRPos.x += editRSize.x;
	editRTmp->CreateInspecterEditBox(phwnd, phins, editRPos, editRSize,
		IDC_ITEM::TRANSFORM_ROT_Z, 0, "Z", MethodHelper::FloatToCstr(this->Rotate.z), false);
	itemTmp->m_SubItemList.push_back(editRTmp);
	//로테이션 z업데이트
	upitem = new UpdateItem();
	upitem->Inite(editRTmp->m_hwnd, (void*)&this->Rotate.z, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::TRANSFORM_ROT_Z);
	upList->push_back(upitem);
	upitem = nullptr;



	//다음위치 정해주기
	transNPos.y += INFO_PADDING * 2;

	//스케일
	POINT posScale = transNPos;
	InspacterItem* scaleText = new InspacterItem();
	posScale.x += INFO_PADDING * 1;
	posScale.y += INFO_PADDING * 2;
	strSize = { posScale.x,posScale.y,(LONG)(compFrameXsize - INFO_PADDING * 2) / 2, 20 };


	scaleText->CreateInspecterStaticText(phwnd, phins, strSize, "Scale");
	itemTmp->m_SubItemList.push_back(scaleText);


	//다음위치 정해주기
	transNPos.y += INFO_PADDING * 2;

	//로테이션 x,y,z 에디트 박스 만들기
	POINT editSPos = transNPos;
	InspacterItem* editSTmp = new InspacterItem();
	POINT editSSize = { 0, };

	editSPos.x += INFO_PADDING * 1;
	editSPos.y += INFO_PADDING * 2;

	editSSize.y = INFO_PADDING * 2;
	editSSize.x = static_cast<LONG>((compFrameXsize - INFO_PADDING * 2) / 3);

	//스케일 x
	editSTmp->CreateInspecterEditBox(phwnd, phins, editSPos, editSSize,
		IDC_ITEM::VECTERTMP6_X, 0, "X", MethodHelper::FloatToCstr(this->Scale.x), false);
	itemTmp->m_SubItemList.push_back(editSTmp);
	//스케일 x업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editSTmp->m_hwnd, (void*)&this->Scale.x, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP6_X);
	upList->push_back(upitem);
	upitem = nullptr;



	//스케일 y
	editSTmp  = new InspacterItem();
	editSPos.x += editSSize.x;
	editSTmp->CreateInspecterEditBox(phwnd, phins, editSPos, editSSize,
		IDC_ITEM::VECTERTMP6_Y, 0, "Y", MethodHelper::FloatToCstr(this->Scale.y), false);
	itemTmp->m_SubItemList.push_back(editSTmp);
	//스케일 y업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editSTmp->m_hwnd, (void*)&this->Scale.y, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP6_Y);
	upList->push_back(upitem);
	upitem = nullptr;


	//스케일 z
	editSTmp = new InspacterItem();
	editSPos.x += editSSize.x;
	editSTmp->CreateInspecterEditBox(phwnd, phins, editSPos, editSSize,
		IDC_ITEM::VECTERTMP6_Z, 0, "Z", MethodHelper::FloatToCstr(this->Scale.z), false);
	itemTmp->m_SubItemList.push_back(editSTmp);
	//스케일 z업데이트 셋팅
	upitem = new UpdateItem();
	upitem->Inite(editSTmp->m_hwnd, (void*)&this->Scale.z, UpdateType::UPDATE_TEXT_EDIT, UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP6_Z);
	upList->push_back(upitem);
	upitem = nullptr;



	//총 프레임 크기 정하고 윈도우 바꿔주기
	transFrameSize.y = editSPos.y + editSSize.y + INFO_PADDING - NextPos.y;
	itemTmp->SetWindowMove(NextPos, transFrameSize, Vector::Vector4());

	return itemTmp;
}