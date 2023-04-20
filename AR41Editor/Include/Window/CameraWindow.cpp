#include "CameraWindow.h"
#include "Device.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorComboBox.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CCameraWindow::CCameraWindow()
	: m_CameraCBIndex(0)
	, m_TargetArmCBIndex(0)
	, m_CameraCB(nullptr)
	, m_TargetArmCB(nullptr)
	, m_CameraViewDistance(nullptr)
	, m_TargetDistance(nullptr)
	, m_TargetArmOffset{}
{
}

CCameraWindow::~CCameraWindow()
{
}

void CCameraWindow::SetSelectComponent(CSceneComponent* component)
{
	if (component->GetComponentTypeName() == "CameraComponent")
	{
		m_SelectCamera = (CCameraComponent*)component;
		m_SelectArm = nullptr;
		int index = (int)m_SelectCamera->GetCameraType();
		std::string type= m_CameraCB->GetItem(index);
		m_CameraCB->SetPrevViewName(type);
		m_CameraCB->SetSelectIndex(index);
		m_CameraViewDistance->SetFloat(m_SelectCamera->GetCameraViewDistance());
	}
	else if (component->GetComponentTypeName() == "TargetArm")
	{
		m_SelectCamera = nullptr;
		m_SelectArm = (CTargetArm*)component;
		m_TargetArmOffset[0]->SetFloat(m_SelectArm->GetTargetOffset().x);
		m_TargetArmOffset[1]->SetFloat(m_SelectArm->GetTargetOffset().y);
		m_TargetArmOffset[2]->SetFloat(m_SelectArm->GetTargetOffset().z);
		int index = (int)m_SelectArm->GetTargetDistanceAxis();
		std::string type = m_TargetArmCB->GetItem(index);
		m_TargetArmCB->SetPrevViewName(type);
		m_TargetArmCB->SetSelectIndex(index);
		m_TargetDistance->SetFloat(m_SelectArm->GetTargetDistance());
	}
}

void CCameraWindow::SetSelectSprite(CSpriteComponent* component)
{
	m_SelectSprite = component;
}

bool CCameraWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("카메라");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CCameraWindow>(this, &CCameraWindow::CameraCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CCameraWindow>(this, &CCameraWindow::CameraChangeCallback);
	m_CameraCB = CreateWidget<CEditorComboBox>("CameraType",110.f);
	m_CameraCB->SetPrevViewName("Camera2D");
	m_CameraCB->AddItem("Camera2D");
	m_CameraCB->AddItem("Camera3D");
	m_CameraCB->AddItem("CameraUI");
	m_CameraCB->SetSelectPrevViewName(true);
	m_CameraCB->SetSelectCallback<CCameraWindow>(this, &CCameraWindow::CameraCBClickCallback);
	m_CameraViewDistance = CreateWidget<CEditorInputText>("CameraViewDistance", 90.f, 30.f);
	m_CameraViewDistance->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("타겟암");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("타겟암 변경", 80.f, 30.f);
	button->SetClickCallback<CCameraWindow>(this, &CCameraWindow::TargetArmChangeCallback);
	label = CreateWidget<CEditorLabel>("TargetOffset");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[0] = CreateWidget<CEditorInputText>("TargetOffsetX", 80.f, 30.f);
	m_TargetArmOffset[0]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[0]->SetHideName("TargetOffsetX");
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[1] = CreateWidget<CEditorInputText>("TargetOffsetY", 80.f, 30.f);
	m_TargetArmOffset[1]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[1]->SetHideName("TargetOffsetY");
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[2] = CreateWidget<CEditorInputText>("TargetOffsetZ", 80.f, 30.f);
	m_TargetArmOffset[2]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[2]->SetHideName("TargetOffsetZ");
	m_TargetArmCB = CreateWidget<CEditorComboBox>("TargetDistanceAxis", 100.f);
	m_TargetArmCB->SetPrevViewName("AXIS_X");
	m_TargetArmCB->AddItem("AXIS_X");
	m_TargetArmCB->AddItem("AXIS_Y");
	m_TargetArmCB->AddItem("AXIS_Z");
	m_TargetArmCB->AddItem("AXIS_MAX");
	m_TargetArmCB->SetSelectPrevViewName(true);
	m_TargetArmCB->SetSelectCallback<CCameraWindow>(this, &CCameraWindow::TargetArmCBClickCallback);
	m_TargetDistance = CreateWidget<CEditorInputText>("TargetDistance", 80.f, 30.f);
	m_TargetDistance->SetInputType(EImGuiInputType::Float);
	return true;
}

void CCameraWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectSprite)
	{
		if (!m_SelectSprite->GetActive())
		{
			m_SelectSprite = nullptr;
		}
	}
	if (m_SelectCamera)
	{
		if (!m_SelectCamera->GetActive())
		{
			m_SelectCamera = nullptr;
		}
	}
	if (m_SelectArm)
	{
		if (!m_SelectArm->GetActive())
		{
			m_SelectArm = nullptr;
		}
	}
}

void CCameraWindow::CameraCreateCallback()
{
	if (!m_SelectSprite)
	{
		return;
	}
	CCameraComponent* m_Camera = m_SelectSprite->GetOwner()->CreateComponent<CCameraComponent>("Camera");
	CTargetArm* m_Arm = m_SelectSprite->GetOwner()->CreateComponent<CTargetArm>("Arm");
	m_Camera->Start();
	m_Arm->Start();
	m_SelectSprite->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);
	Resolution RS = CDevice::GetInst()->GetResolution();
	float width = (float)RS.width / 2.f;
	float height = (float)RS.height / 2.f;
	m_Arm->SetTargetOffset(Vector3(-width, -height, 0.f));
	m_SelectSprite->GetScene()->GetCameraManager()->SetCurrentCamera(m_Camera);
}

void CCameraWindow::CameraChangeCallback()
{
	if (!m_SelectCamera)
	{
		return;
	}
	float size = m_CameraViewDistance->GetFloat();
	m_SelectCamera->SetCameraViewDistance(size);
	m_SelectCamera->SetCameraType((ECameraType)m_CameraCBIndex);
}

void CCameraWindow::TargetArmChangeCallback()
{
	if (!m_SelectArm)
	{
		return;
	}
	m_SelectArm->SetTargetOffset(
		m_TargetArmOffset[0]->GetFloat(), m_TargetArmOffset[1]->GetFloat(), m_TargetArmOffset[2]->GetFloat());
	m_SelectArm->SetTargetDistanceAxis((AXIS)m_TargetArmCBIndex);
	float size = m_TargetDistance->GetFloat();
	m_SelectArm->SetTargetDistance(size);
}

void CCameraWindow::CameraCBClickCallback(int index, const std::string& item)
{
	m_CameraCBIndex = index;
}

void CCameraWindow::TargetArmCBClickCallback(int index, const std::string& item)
{
	m_TargetArmCBIndex = index;
}
