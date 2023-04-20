#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "TransformWindow.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"
#include "Input.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorGUIManager.h"
#include "Component/CameraComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CObjectWindow::CObjectWindow()
    : m_Tree(nullptr)
    , m_WindowTree(nullptr)
{
}

CObjectWindow::~CObjectWindow()
{
}

bool CObjectWindow::AddItem(CGameObject* object, const std::string& name, const std::string& parentName)
{
	return m_Tree->AddItem(object, name, parentName);
}

bool CObjectWindow::AddItem(CUIWindow* window, const std::string& name, const std::string& parentName)
{
	return m_WindowTree->AddItem(window, name, parentName);
}

bool CObjectWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CGameObject*>>("ObjectTree");
	m_Tree->SetHideName("ObjectTree");
	m_Tree->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::TreeCallback);
	m_Tree->SetSize(400.f, 300.f);
	m_Tree->AddItem(nullptr, "Root");
	m_WindowTree = CreateWidget<CEditorTree<CUIWindow*>>("UITree");
	m_WindowTree->SetHideName("UITree");
	m_WindowTree->SetSelectCallback<CObjectWindow>(this, &CObjectWindow::UICallback);
	m_WindowTree->SetSize(400.f, 300.f);
	m_WindowTree->AddItem(nullptr, "Canvas");

	CInput::GetInst()->AddBindFunction<CObjectWindow>
		("Del", Input_Type::Down, this, &CObjectWindow::Delete, CSceneManager::GetInst()->GetScene());
	return true;
}

void CObjectWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectObject)
	{
		if (!m_SelectObject->GetActive())
		{
			m_SelectObject = nullptr;
		}
	}
	if (m_SelectWindow)
	{
		if (!m_SelectWindow->GetActive())
		{
			m_SelectWindow = nullptr;
		}
	}
}

void CObjectWindow::Clear()
{
	m_Tree->Clear();
	m_Tree->AddItem(nullptr, "Root");
	m_WindowTree->Clear();
	m_WindowTree->AddItem(nullptr, "Canvas");
}

void CObjectWindow::ClearSelect()
{
	m_SelectObject = nullptr;
	m_SelectWindow = nullptr;
}

void CObjectWindow::Delete()
{
	if (!m_SelectObject && !m_SelectWindow)
	{
		return;
	}
	if (m_SelectObject)
	{
		m_SelectObject->Destroy();
		m_Tree->DeleteItem(m_SelectObject->GetName());
	}
	else if (m_SelectWindow)
	{
		m_SelectWindow->Destroy();
		m_WindowTree->DeleteItem(m_SelectWindow->GetName());
	}
	ClearSelect();
}

void CObjectWindow::TreeCallback(CEditorTreeItem<CGameObject*>* node, const std::string& item)
{
	CComponentWindow* componentWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (!componentWindow)
	{
		return;
	}
	componentWindow->ClearSelect();
	componentWindow->Clear();
	m_SelectObject = node->GetCustomData();
	if (m_SelectObject)
	{
		CTransformWindow* transformWindow = 
			CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
		CSceneComponent* root = m_SelectObject->GetRootComponent();
		if (root&& transformWindow)
		{
			transformWindow->SetSelectComponent(root);
			transformWindow->SetPos(root->GetWorldPos());
			transformWindow->SetScale(root->GetWorldScale());
			transformWindow->SetRotation(root->GetWorldRot());
		}
		std::vector<HierarchyName>	vecName;
		m_SelectObject->GetAllComponentHierarchyName(vecName);
		std::string	name = m_SelectObject->GetName() + "(" + m_SelectObject->GetObjectTypeName() + ")";
		componentWindow->AddItem(nullptr, name);
		if (!vecName.empty())
		{
			std::string	parentName = name;
			name = vecName[0].name + "(" + vecName[0].className + ")";
			componentWindow->AddItem(vecName[0].component, name, parentName);
			size_t	size = vecName.size();
			std::vector<HierarchyName>	vecName1;
			for (size_t i = 1; i < size; ++i)
			{
				parentName = vecName[i].parentName + "(" + vecName[i].parentClassName + ")";
				name = vecName[i].name + "(" + vecName[i].className + ")";
				if (!componentWindow->AddItem(vecName[i].component, name, parentName))
				{
					vecName1.push_back(vecName[i]);
				}
			}
			size = vecName1.size();
			for (size_t i = 0; i < size; ++i)
			{
				parentName = vecName1[i].parentName + "(" + vecName1[i].parentClassName + ")";
				name = vecName1[i].name + "(" + vecName1[i].className + ")";
				componentWindow->AddItem(vecName1[i].component, name, parentName);
			}
		}
	}
}

void CObjectWindow::UICallback(CEditorTreeItem<CUIWindow*>* node, const std::string& item)
{
	CComponentWindow* componentWindow =
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if (!componentWindow)
	{
		return;
	}
	componentWindow->ClearSelect();
	componentWindow->Clear();
	m_SelectWindow = node->GetCustomData();
	if (m_SelectWindow)
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWindow(m_SelectWindow);
		}
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWindow(m_SelectWindow);
		}
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWindow(m_SelectWindow);
		}
		CUIProgressBarWindow* barWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWindow(m_SelectWindow);
		}
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWindow(m_SelectWindow);
		}
		std::vector<HierarchyWidgetName>	vecName;
		m_SelectWindow->GetAllWidgetHierarchyName(vecName);
		std::string	name = m_SelectWindow->GetWindowTypeName();
		componentWindow->AddWidget(nullptr, name);
		if (!vecName.empty())
		{
			std::string	parentName = name;
			size_t	size = vecName.size();
			for (size_t i = 0; i < size; ++i)
			{
				name = vecName[i].name + "(" + vecName[i].className + ")";
				componentWindow->AddWidget(vecName[i].widget, name, parentName);
			}
		}
	}
}