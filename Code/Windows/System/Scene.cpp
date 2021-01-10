#include "pch.h"
#include "Scene.h"
#include "Text.h"
#include"SkyBoxClass.h"
#include "GridTerrain.h"
#include "RotateSphere.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::Inite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, ItemWindow::ItemWindow* iWindow)
{
	_wsetlocale(LC_ALL, L"Korean");

	/*공용멤버 생성초기화*/
	if (!this->Inite_StaticMember())
	{
		printf("Scene : Inite_StaticMember error \n");
		return false;
	}



	/*초기화*/
	m_gm_manager = new GameObjectManager();
	if (m_gm_manager)
	{
		m_gm_manager->Inite();
		iWindow->m_PtrGM = m_gm_manager;
	}
	else
	{
		printf("Scene : GameObjectManager Dynamic cast error \n");
		return false;
	}

	m_ui_manager = new UIManager();
	if (m_ui_manager)
	{
		m_ui_manager->Inite();
		iWindow->m_PtrUM = m_ui_manager;
	}
	else
	{
		printf("Scene : UIManager Dynamic cast error \n");
		return false;
	}

	/*RenderQueue 초기화*/
	m_RenderQueue = new RenderQueue();
	if (!m_RenderQueue->Inite())
	{
		printf("Scene : RenderQueue Dynamic cast error \n");
		return false;
	}

	/*UpdateQueue 생성*/
	m_UpdateQueue = new UpdateQueue();
	if (!m_UpdateQueue->Inite())
	{
		printf("Scene : UpdateQueue Dynamic cast error \n");
		return false;
	}

	/*ADQueue 생성*/
	m_ADQueue = new ADQueue();
	if (!m_ADQueue->Inite())
	{
		printf("Scene : ADQueue Dynamic cast error \n");
		return false;
	}


	Main_Camera = new Camera();
	if (Main_Camera)
	{
		iWindow->m_MainCamera = Main_Camera;
	}
	else
	{
		printf("Scene : Camera Dynamic cast error \n");
		return false;
	}

	SkyBoxObject = new GameObject();
	if (SkyBoxObject)
	{
		SkyBoxObject->SetName("SkyBox");
		SkyBoxClass* skb = SkyBoxObject->AddCompponent<SkyBoxClass>();
		skb->Inite(device, hwnd);
		SkyBoxObject->GetCompponent<Transform>()->Scale.Set(200, 200, 200);
	}


	GridTerrainObject = new GameObject();
	if (GridTerrainObject)
	{
		GridTerrain* gt = GridTerrainObject->AddCompponent<GridTerrain>();
		GridTerrainObject->SetName("GridTerrain");
		gt->Inite(device, hwnd, 200, 200);
	}


	DirectionLight = new DirectionalLight();
	if(DirectionLight)
	{
		DirectionLight->Inite();
	}
	else
	{
		printf("Scene : DirectionalLight Dynamic cast error \n");
		return false;
	}


	/*게임 오브젝트 로드*/
	this->GamobjectLoad();
	this->UILoad();





	/*테스트 초기 위치*/
	/*Model rotatee 270*/
	Main_Camera->SetPosition(0.0f, 0.5f, -5.0f);
	Main_Camera->SetRotation(0, 0, 0);
	DirectionLight->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	DirectionLight->SetDiffuseColor(0.5f, 0.5f, 0.5f, 0.0f);
	DirectionLight->SetDirection(1.0f, 0.0f, 0.0f);

	return true;
}

void Scene::Destroy()
{
	/*test*/
	if (fbx)
	{
		fbx->Destroy();
		delete fbx;
	}

	if (m_gm_manager)
	{
		m_gm_manager->Destroy();
		delete m_gm_manager;
		m_gm_manager = nullptr;
	}

	if (m_ui_manager)
	{
		m_ui_manager->Destroy();
		delete m_ui_manager;
		m_ui_manager = nullptr;
	}


	/*if (Main_Camera)
	{
		delete Main_Camera;
		Main_Camera = nullptr;
	}*/

	if (GridTerrainObject)
	{
		GridTerrainObject->Destroy();
	}

	if (SkyBoxObject)
	{
		SkyBoxObject->Destroy();
	}


	if (DirectionLight)
	{
		DirectionLight->Destroy();
		delete DirectionLight;
		DirectionLight = nullptr;
	}

	if (m_RenderQueue)
	{
		m_RenderQueue->Destroy();
		delete m_RenderQueue;
		m_RenderQueue = nullptr;
	}

	if (m_UpdateQueue)
	{
		m_UpdateQueue->Destroy();
		delete m_UpdateQueue;
		m_UpdateQueue = nullptr;
	}
	
	if (m_ADQueue)
	{
		m_ADQueue->Destroy();
		delete m_ADQueue;
		m_ADQueue = nullptr;
	}

	/*공용멤버 삭제*/
	this->Destroy_StaticMember();

	/*Test*/
}

void Scene::Update(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, ViewWindow::ViewWindow * vWindow,
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	this->ObjectUpdate(device, deviceContext, hwnd, vWindow, tWindow, iWindow);


	iWindow->Update();

	if(vWindow->IsMouseEnter())
		this->MainCameraUpdate();
}

bool Scene::Inite_StaticMember()
{
	Text::m_FontMap = new std::map<std::string, Font*>();

	return true;
}

void Scene::Destroy_StaticMember()
{
	Text::DestroyFontMap();
	AudioSystem::Instance()->Destroy();
}

bool Scene::GamobjectLoad()
{
	GameObject* gm = nullptr;
	Material* mat = nullptr;

	gm = new GameObject();
	gm->SetName("FBX");
	gm->AddCompponent<FbxClass>();
	gm->GetCompponent<Transform>()->Scale.Set(0.01f, 0.01f, 0.01f);
	gm->GetCompponent<Transform>()->Rotate.Set(-90.0f ,0,0);

	/*Test Sound*/
	gm = new GameObject();
	gm->SetName("BGM");
	AudioComponent* ac = gm->AddCompponent<AudioComponent>();
	ac->Inite(L"../DirectProject/Default/Sound/you.wav", true, true);

	/*RotateShpere* rs = new RotateShpere();
	rs->SetName("RotateShpere");*/

	return true;
}

bool Scene::UILoad()
{
	UI* gm = nullptr;
	Texture2D* tx = nullptr;

	/*Text test*/
	gm = nullptr;
	gm = new UI;
	Text* text = nullptr;
	text = gm->AddCompponent<Text>();
	gm->SetName("FPS_Text");
	text->Depth = 0;
	countText = text;
	gm->GetCompponent<Transform>()->SetPosition(10, 10, 0);
	gm->GetCompponent<Transform>()->SetSize(400, 400);
	gm->GetCompponent<Transform>()->SetScale(Vector::Vector3::Identity());

	//list->push_back(gm);

	return true;
}

void Scene::ObjectUpdate(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd,
	ViewWindow::ViewWindow * vWindow,
	TreeWindow::TreeViewWinodw * tWindow, ItemWindow::ItemWindow * iWindow)
{
	//오브젝트 업데이트
	this->Gameobject_Update(device);
	this->UI_Update(device);

	m_UpdateQueue->UpdateTexture(device);

	this->m_ADQueue->NewObject(this->m_gm_manager, device, hwnd, vWindow, tWindow, iWindow);
	this->m_ADQueue->NewUIObject(this->m_ui_manager, device, deviceContext, hwnd, Screen::Instance().GetSize(), vWindow, tWindow, iWindow);
	
	if (!this->CheckDestroyItemWindw(tWindow))
	{
		printf("Not Delete ITem\n");
	}
	this->m_ADQueue->DestroyObject(this->m_gm_manager, this->m_ui_manager, vWindow, tWindow, iWindow);

	/*뷰, 직교행렬 업데이트*/
	Main_Camera->Render();
	Screen::Instance().UpdateScreen();

	//뷰, 직교, 월드좌표계 설정
	Transform::Projection_Matrix = Screen::Instance().GetProjectionMatrix();
	Transform::View_Matrix = Main_Camera->GetViewMatrix();
	Transform::ViewUI_Matrix = Main_Camera->GetUIViewMatrix();
}

void Scene::Gameobject_Update(ID3D11Device * device)
{
	m_gm_manager->CheckRootToChild();
	m_gm_manager->UpdateObject(this->m_UpdateQueue);
}

void Scene::UI_Update(ID3D11Device * device)
{
	m_ui_manager->CheckRootToChild();
	m_ui_manager->UpdateObject(this->m_UpdateQueue);
}

bool Scene::Render(ID3D11DeviceContext * deviceContext)
{
	m_gm_manager->SetRenderQueue(m_RenderQueue);
	if (!m_RenderQueue->Render3D(deviceContext, DirectionLight))
	{
		return false;
	}
	return true;
}

bool Scene::RenderWire(ID3D11DeviceContext * deviceContext)
{
	/*if (!m_RenderQueue->Render3DWire(deviceContext, DirectionLight))
	{
		return false;
	}*/
	if (m_gm_manager->m_TargetObject != nullptr)
	{
		Material* mat = m_gm_manager->m_TargetObject->GetCompponent<Material>();
		if (mat != nullptr)
		{
			mat->Render_WireFrame(deviceContext, m_gm_manager->m_TargetObject->GetCompponent<Transform>(),
				DirectionLight);
		}

	}

	return true;
}

bool Scene::UI_Render(ID3D11DeviceContext * deviceContext, D3D::DirectX3D* d3d)
{
	/*Test*/
	char text[FILENAME_MAX];
	ZeroMemory(text, FILENAME_MAX);
	//sprintf_s(text, "X:%ld,Y:%ld Hello World!! 안녀하세요!!", Input::Instance().Mouse_Position.x, Input::Instance().Mouse_Position.y);

	sprintf_s(text, "FPS : %d", TimeClass::Instance().FPS);
	countText->SetText(text);

	///////////////////
	m_ui_manager->SetRenderQueue(m_RenderQueue);
	if (!m_RenderQueue->Render2D(deviceContext, d3d, Screen::Instance().GetSize()))
	{
		return false;
	}

	return true;
}

bool Scene::MainCameraUpdate()
{
	float rlSpeed = 0.01f * TimeClass::Instance().DeletaTime;
	float fSpeed = 0.05f * TimeClass::Instance().DeletaTime;
	float RotateSpeed = 0.1f* TimeClass::Instance().DeletaTime;
	float x;
	float y;
	float z;

	Input::Instance().GetMouseMoveValue(&x, &y, &z);
	Vector::Vector3 MousePos = Main_Camera->GetPosition();

	if (Input::Instance().MouseButtonDown(MouseButton::Middle))
	{
		if (x > 0)
		{
			Vector::Vector3 rVec = Matrix::Matrix::Multiply(Vector::Vector3::Right(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
			rVec.Normalize();
			MousePos = MousePos - rVec * rlSpeed;
			Main_Camera->SetPosition(MousePos);
		}
		else if (x < 0)
		{
			Vector::Vector3 rVec = Matrix::Matrix::Multiply(Vector::Vector3::Right(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
			rVec.Normalize();
			MousePos = MousePos + rVec * rlSpeed;
			Main_Camera->SetPosition(MousePos);
		}
		
		if (y > 0)
		{
			Vector::Vector3 uVec = Matrix::Matrix::Multiply(Vector::Vector3::Up(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
			uVec.Normalize();
			MousePos = MousePos + uVec * rlSpeed;
			Main_Camera->SetPosition(MousePos);
		}
		else if (y < 0)
		{
			Vector::Vector3 uVec = Matrix::Matrix::Multiply(Vector::Vector3::Up(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
			uVec.Normalize();
			MousePos = MousePos - uVec * rlSpeed;
			Main_Camera->SetPosition(MousePos);
		}
	}

	if (z > 0)
	{
		Vector::Vector3 fVec = Matrix::Matrix::Multiply(Vector::Vector3::Forward(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
		MousePos = MousePos + fVec * fSpeed;
		Main_Camera->SetPosition(MousePos.x, MousePos.y, MousePos.z);
	}
	else if (z < 0)
	{
		Vector::Vector3 fVec = Matrix::Matrix::Multiply(Vector::Vector3::Forward(), Matrix::Matrix::Rotation(Main_Camera->GetRotation()));
		MousePos = MousePos - fVec * fSpeed;
		Main_Camera->SetPosition(MousePos.x, MousePos.y, MousePos.z);
	}

	if (Input::Instance().MouseButtonDown(MouseButton::Right))
	{
		if (abs(x) > abs(y))
		{
			if (x > 0)
			{
				Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
				Vector::Vector3 rVec = { 0, RotateSpeed, 0 };
				Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
				cWorld = rMat * cWorld;
				Main_Camera->transform->WorldMatrixImportSRT(cWorld);
			}
			else if (x < 0)
			{
				Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
				Vector::Vector3 rVec = { 0, -RotateSpeed, 0 };
				Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
				cWorld = rMat * cWorld;
				Main_Camera->transform->WorldMatrixImportSRT(cWorld);
			}
		}
		else
		{
			if (y > 0)
			{
				Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
				Vector::Vector3 rVec = { RotateSpeed, 0, 0 };
				Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
				cWorld = rMat * cWorld;
				Main_Camera->transform->WorldMatrixImportSRT(cWorld);
			}
			else if (y < 0)
			{

				Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
				Vector::Vector3 rVec = { -RotateSpeed, 0, 0 };
				Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
				cWorld = rMat * cWorld;
				Main_Camera->transform->WorldMatrixImportSRT(cWorld);
			}
		}
		/*
		if (x > 0)
		{
			Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
			Vector::Vector3 rVec = { 0, RotateSpeed, 0 };
			Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
			cWorld = rMat * cWorld;
			Main_Camera->transform->WorldMatrixImportSRT(cWorld);
		}
		else if (x < 0)
		{
			Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
			Vector::Vector3 rVec = { 0, -RotateSpeed, 0 };
			Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
			cWorld = rMat * cWorld;
			Main_Camera->transform->WorldMatrixImportSRT(cWorld);
		}else if (y > 0)
		{
			Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
			Vector::Vector3 rVec = { RotateSpeed, 0, 0 };
			Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
			cWorld = rMat * cWorld;
			Main_Camera->transform->WorldMatrixImportSRT(cWorld);
		}
		else if (y < 0)
		{

			Matrix::Matrix cWorld = Main_Camera->transform->GetWorldMatrix();
			Vector::Vector3 rVec = { -RotateSpeed, 0, 0 };
			Matrix::Matrix rMat = Matrix::Matrix::Rotation(rVec);
			cWorld = rMat * cWorld;
			Main_Camera->transform->WorldMatrixImportSRT(cWorld);
		}*/
	}


	

	if (Input::Instance().IsSpacePressed())
	{
		printf("Object Count : %d\n", BaseObject::ObjectCount);
	}

	return true;
}

bool Scene::CheckDestroyItemWindw(TreeWindow::TreeViewWinodw * tWindow)
{
	if (tWindow->DestroyItem != nullptr)
	{
		BaseObject* current = this->m_gm_manager->FindObject(tWindow->DestroyItem);

		//메인카메라
		if (current == Main_Camera)
		{
			tWindow->DestroyItem = nullptr;
			return false;
		}

		//스카이박스
		if (current == SkyBoxObject)
		{
			tWindow->DestroyItem = nullptr;
			return false;
		}

		if (current == nullptr)
		{
			current = m_ui_manager->FindObject(tWindow->DestroyItem);
			if (current == nullptr)
			{
				printf("Error Not Find Item\n");
				tWindow->DestroyItem = nullptr;
			}
			else
			{
				tWindow->DestroyItem = nullptr;
				current->Destroy();
			}
		}
		else
		{
			tWindow->DestroyItem = nullptr;
			current->Destroy();
		}


	}

	return true;
}

void Scene::SkyboxRender(ID3D11DeviceContext * deviceContext)
{
	this->SkyBoxObject->GetCompponent<SkyBoxClass>()->Render(deviceContext, nullptr, DirectionLight);

}

void Scene::GridTerrainRender(ID3D11DeviceContext * deviceContext)
{
	if (GridTerrainObject->Enable)
	{
		GridTerrainObject->GetCompponent<GridTerrain>()->Render(deviceContext);
	}
}
