#include "StatusWindow.h"
#include "Engine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Component/SceneComponent.h"
#include "UI/UIWidget.h"

CStatusWindow::CStatusWindow()
	: m_Check(nullptr)
	, m_Frame(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_PosZ(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
	, m_SizeZ(nullptr)
{
}

CStatusWindow::~CStatusWindow()
{
}

void CStatusWindow::SetSelectComponent(CSceneComponent* component)
{
	m_SelectComponent = component;
}

void CStatusWindow::SetSelectWidget(CUIWidget* widget)
{
	m_SelectWidget = widget;
}

bool CStatusWindow::Init()
{
	m_Check = CreateWidget<CEditorCheckBox>("활성화 버튼");
	m_Frame = CreateWidget<CEditorInputText>("프레임", 80.f, 30.f);
	m_Frame->SetInputType(EImGuiInputType::Float);
	m_Frame->ReadOnly(true);

	m_PosX = CreateWidget<CEditorInputText>("X 위치", 80.f, 30.f);
	m_PosX->SetInputType(EImGuiInputType::Float);
	m_PosX->ReadOnly(true);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("line");
	m_SizeX = CreateWidget<CEditorInputText>("X 크기", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	m_SizeX->ReadOnly(true);

	m_PosY = CreateWidget<CEditorInputText>("Y 위치", 80.f, 30.f);
	m_PosY->SetInputType(EImGuiInputType::Float);
	m_PosY->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_SizeY = CreateWidget<CEditorInputText>("Y 크기", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);
	m_SizeY->ReadOnly(true);

	m_PosZ = CreateWidget<CEditorInputText>("Z 위치", 80.f, 30.f);
	m_PosZ->SetInputType(EImGuiInputType::Float);
	m_PosZ->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_SizeZ = CreateWidget<CEditorInputText>("Z 크기", 80.f, 30.f);
	m_SizeZ->SetInputType(EImGuiInputType::Float);
	m_SizeZ->ReadOnly(true);
	return true;
}

void CStatusWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
	if(!m_Check->GetCheck())
	{
		return;
	}
	float FPS = CEngine::GetInst()->GetFPS();
	m_Frame->SetFloat(FPS);
	if (m_SelectComponent)
	{
		Vector3 pos=m_SelectComponent->GetWorldPos();
		m_PosX->SetFloat(pos.x);
		m_PosY->SetFloat(pos.y);
		m_PosZ->SetFloat(pos.z);
		Vector3 size = m_SelectComponent->GetWorldScale();
		m_SizeX->SetFloat(size.x);
		m_SizeY->SetFloat(size.y);
		m_SizeZ->SetFloat(size.z);
	}
	else if (m_SelectWidget)
	{
		Vector2 pos = m_SelectWidget->GetPos();
		m_PosX->SetFloat(pos.x);
		m_PosY->SetFloat(pos.y);
		m_PosZ->SetFloat(0.f);
		Vector2 size = m_SelectWidget->GetSize();
		m_SizeX->SetFloat(size.x);
		m_SizeY->SetFloat(size.y);
		m_SizeZ->SetFloat(0.f);
	}
}
