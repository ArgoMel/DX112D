#include "ComponentWindow.h"
#include "TransformWindow.h"
#include "ImageWindow.h"
#include "DetailWindow.h"
#include "AnimationWindow.h"
#include "ColiderWindow.h"
#include "ColliderPixelWindow.h"
#include "CameraWindow.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"
#include "StatusWindow.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Component.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderObb2d.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
//#include "Component/RigidBody.h"
#include "Editor/EditorGUIManager.h"
#include "UI/UIWidget.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UINumber.h"
#include "UI/UIProgressBar.h"
#include "UI/UIText.h"
#include "GameObject/GameObject.h"

CComponentWindow::CComponentWindow()
	: m_Tree(nullptr)
	, m_WidgetTree(nullptr)
{
}

CComponentWindow::~CComponentWindow()
{
}

bool CComponentWindow::AddItem(CComponent* component, const std::string& name, const std::string& parentName)
{
	return m_Tree->AddItem(component, name, parentName);
}

bool CComponentWindow::AddWidget(CUIWidget* widget, const std::string& name, const std::string& parentName)
{
	return m_WidgetTree->AddItem(widget, name, parentName);
}

void CComponentWindow::Clear()
{
	m_Tree->Clear();
	m_WidgetTree->Clear();
}

void CComponentWindow::ClearSelect()
{
	m_SelectComponent = nullptr;
	m_SelectWidget = nullptr;
}

bool CComponentWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CComponent*>>("ComponentTree");
	m_Tree->SetHideName("ComponentTree");
	m_Tree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::TreeCallback);
	m_Tree->SetSize(400.f, 300.f);
	m_WidgetTree = CreateWidget<CEditorTree<CUIWidget*>>("WidgetTree");
	m_WidgetTree->SetHideName("WidgetTree");
	m_WidgetTree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::WidgetCallback);
	m_WidgetTree->SetSize(400.f, 300.f);

	CScene* scene = CSceneManager::GetInst()->GetScene();
	//CInput::GetInst()->AddBindFunction<CComponentWindow>
	//	("Del2", Input_Type::Down, this, &CComponentWindow::Delete, scene);
	CInput::GetInst()->AddBindFunction<CComponentWindow>
		("ChangePos", Input_Type::Down, this, &CComponentWindow::ChangePos, scene);
	return true;
}

void CComponentWindow::Update(float deltaTime)
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
}

void CComponentWindow::Delete()
{
	if(!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	if(m_SelectComponent)
	{
		m_SelectComponent->Destroy();
		m_Tree->DeleteItem(m_SelectComponent->GetName()+"("+ m_SelectComponent->GetComponentTypeName() + ")");
	}
	else if (m_SelectWidget)
	{
		m_SelectWidget->Destroy();
		m_WidgetTree->DeleteItem(m_SelectWidget->GetName() + "(" + m_SelectWidget->GetWidgetTypeName() + ")");
	}
	ClearSelect();
}

void CComponentWindow::ChangePos()
{
	if (!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	if (m_SelectComponent)
	{
		CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
		component->SetWorldPosition(mouseWorldPos);
	}
	else if (m_SelectWidget)
	{
		m_SelectWidget->SetPos(mouseWorldPos);
	}
}

void CComponentWindow::TreeCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item)
{
	m_SelectComponent = node->GetCustomData();
	if (!m_SelectComponent)
	{
		return;
	}
	CDetailWindow* detailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
	if (detailWindow)
	{
		detailWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());
	}
	CStatusWindow* statusWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CStatusWindow>("StatusWindow");
	if (statusWindow)
	{
		statusWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());
	}
	CCameraWindow* cameraWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CCameraWindow>("CameraWindow");
	if (cameraWindow)
	{
		cameraWindow->SetSelectSprite((CSpriteComponent*)m_SelectComponent.Get());
	}
	CColiderWindow* colliderWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CColiderWindow>("ColliderWindow");
	CColliderPixelWindow* colliderPixelWindow = CEditorGUIManager::GetInst()->
		FindEditorWindow<CColliderPixelWindow>("ColliderPixelWindow");
	if (m_SelectComponent->GetComponentTypeName() == "SpriteComponent")
	{
		CImageWindow* imageWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CImageWindow>("ImageWindow");
		if (imageWindow)
		{
			imageWindow->SetSelectComponent((CSpriteComponent*)m_SelectComponent.Get());
		}
		CAnimationWindow* animationWindow = CEditorGUIManager::GetInst()->
			FindEditorWindow<CAnimationWindow>("AnimationWindow");
		if (animationWindow)
		{
			animationWindow->SetSelectComponent((CSpriteComponent*)m_SelectComponent.Get());
		}
		if (colliderWindow)
		{
			colliderWindow->SetSelectSprite((CSpriteComponent*)m_SelectComponent.Get());
		}
		if (colliderPixelWindow)
		{

			colliderPixelWindow->SetSelectSprite((CSpriteComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderBox2D"&& colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderBox2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderSphere2D" && colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderSphere2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderOBB2D" && colliderWindow)
	{
		colliderWindow->SetSelectComponent((CColliderOBB2D*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "ColliderPixel"&& colliderPixelWindow)
	{
		colliderPixelWindow->SetSelectComponent((CColliderPixel*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "CameraComponent" && cameraWindow)
	{
		cameraWindow->SetSelectComponent((CCameraComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TargetArm" && cameraWindow)
	{
		cameraWindow->SetSelectComponent((CTargetArm*)m_SelectComponent.Get());
	}
	CTransformWindow* transformWindow = CEditorGUIManager::GetInst()->
		FindEditorWindow<CTransformWindow>("TransformWindow");
	if(!transformWindow)
	{
		return;
	}
	CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
	transformWindow->SetSelectComponent(component);
	if (component->GetParent())
	{
		transformWindow->SetPos(component->GetRelativePos());
		transformWindow->SetScale(component->GetRelativeScale());
		transformWindow->SetRotation(component->GetRelativeRot());
	}

	else
	{
		transformWindow->SetPos(component->GetWorldPos());
		transformWindow->SetScale(component->GetWorldScale());
		transformWindow->SetRotation(component->GetWorldRot());
	}
}

void CComponentWindow::WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	CStatusWindow* statusWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CStatusWindow>("StatusWindow");
	if (statusWindow)
	{
		statusWindow->SetSelectWidget(m_SelectWidget.Get());
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
		}
	}
}