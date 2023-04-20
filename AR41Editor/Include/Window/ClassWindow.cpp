#include "ClassWindow.h"
#include "ObjectWindow.h"
#include "ComponentWindow.h"
#include "PathManager.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject\Player2D.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "../GameObject\BackGround.h"
#include "../GameObject\Boss.h"
#include "../GameObject\Marker.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/TileMapComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderOBB2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderPixel.h"
#include "Component/RigidBody.h"

CClassWindow::CClassWindow()
	: m_ComponentList(nullptr)
	, m_ObjectList(nullptr)
	, m_ComponentSelectName(nullptr)
	, m_ObjectSelectName(nullptr)
{
}

CClassWindow::~CClassWindow()
{
}

bool CClassWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("ObjectClass");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("오브젝트생성", 150.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CClassWindow>(this, &CClassWindow::ObjectCreateCallback);
	m_ObjectList = CreateWidget<CEditorListBox>("ObjectListBox");
	m_ObjectList->SetHideName("ObjectListBox");
	m_ObjectList->SetSize(200.f, 300.f);
	m_ObjectList->SetPageItemCount(6);
	m_ObjectList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ObjectClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ObjectSelectName = CreateWidget<CEditorInputText>("ObjectName",100.f,30.f);
	m_ObjectSelectName->SetHideName("ObjectName");
	m_ObjectSelectName->AddFlag(ImGuiInputTextFlags_ReadOnly);

	label = CreateWidget<CEditorLabel>("ComponentClass");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	label->SetSizeX(150.f);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("컴포넌트생성", 150.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CClassWindow>(this, &CClassWindow::ComponentCreateCallback);
	m_ComponentList = CreateWidget<CEditorListBox>("ComponentListBox");
	m_ComponentList->SetHideName("ComponentListBox");
	m_ComponentList->SetSize(200.f, 300.f);
	m_ComponentList->SetPageItemCount(6);
	m_ComponentList->SetSelectCallback<CClassWindow>(this, &CClassWindow::ComponentClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_ComponentSelectName = CreateWidget<CEditorInputText>("ComponentName", 100.f, 30.f);
	m_ComponentSelectName->SetHideName("ComponentName");
	m_ComponentSelectName->AddFlag(ImGuiInputTextFlags_ReadOnly);

	LoadGameObjectName();
	LoadComponentName();
	return true;
}

void CClassWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CClassWindow::ObjectClickCallback(int index, const std::string& item)
{
	m_SelectObjectItem = item;
	m_ObjectSelectName->SetText(item.c_str());
}

void CClassWindow::ComponentClickCallback(int index, const std::string& item)
{
	m_SelectComponentItem = item;
	m_ComponentSelectName->SetText(item.c_str());
}

void CClassWindow::ObjectCreateCallback()
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CGameObject* obj = nullptr;
	if (m_SelectObjectItem == "")
	{
		return;
	}
	CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if (m_SelectObjectItem == "GameObject")
	{
		obj = scene->CreateObject<CGameObject>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "Player2D")
	{
		obj = scene->CreateObject<CPlayer2D>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "Bullet")
	{
		obj = scene->CreateObject<CBullet>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "Monster")
	{
		obj = scene->CreateObject<CMonster>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "BackGround")
	{
		obj = scene->CreateObject<CBackGround>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "Boss")
	{
		obj = scene->CreateObject<CBoss>(m_SelectObjectItem);
	}
	else if (m_SelectObjectItem == "Marker")
	{
		obj = scene->CreateObject<CMarker>(m_SelectObjectItem);
	}
	if (window)
	{
		window->AddItem(obj, m_SelectObjectItem);
	}
}

void CClassWindow::ComponentCreateCallback()
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CObjectWindow* objectWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if(!objectWindow)
	{
		return;
	}
	CGameObject* selectObject = objectWindow->GetSelectObject();
	if (!selectObject)
	{
		return;
	}
	CComponentWindow* componentWindow = 
		CEditorGUIManager::GetInst()->FindEditorWindow<CComponentWindow>("ComponentWindow");
	if(!componentWindow)
	{
		return;
	}
	CSceneComponent* selectComponent = (CSceneComponent*)componentWindow->GetSelectComponent();
	if (!selectComponent)
	{
		selectComponent = selectObject->GetRootComponent();
	}
	std::string	name;
	CSceneComponent* newComponent = nullptr;
	if (m_SelectComponentItem == "SpriteComponent")
	{
		name = "SpriteComponent(SpriteComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CSpriteComponent>("SpriteComponent");
	}
	else if (m_SelectComponentItem == "SceneComponent")
	{
		name = "SceneComponent(SceneComponent)";
		newComponent = selectObject->CreateComponent<CSceneComponent>("SceneComponent");
	}
	else if (m_SelectComponentItem == "TargetArm")
	{
		name = "TargetArm(TargetArm)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CTargetArm>("TargetArm");
	}
	else if (m_SelectComponentItem == "CameraComponent")
	{
		name = "CameraComponent(CameraComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CCameraComponent>("CameraComponent");
	}
	else if (m_SelectComponentItem == "TileMapComponent")
	{
		name = "TileMapComponent(TileMapComponent)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CTileMapComponent>("TileMapComponent");
	}
	else if (m_SelectComponentItem == "RigidBody")
	{
		name = "RigidBody(RigidBody)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CRigidBody>("RigidBody");
	}
	else if (m_SelectComponentItem == "ColliderBox2D")
	{
		name = "ColliderBox2D(ColliderBox2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderBox2D>("ColliderBox2D");
	}
	else if (m_SelectComponentItem == "ColliderOBB2D")
	{
		name = "ColliderOBB2D(ColliderOBB2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderOBB2D>("ColliderOBB2D");
	}
	else if (m_SelectComponentItem == "ColliderSphere2D")
	{
		name = "ColliderSphere2D(ColliderSphere2D)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderSphere2D>("ColliderSphere2D");
	}
	else if (m_SelectComponentItem == "ColliderPixel")
	{
		name = "ColliderPixel(ColliderPixel)";
		newComponent = (CSceneComponent*)selectObject->CreateComponent<CColliderPixel>("ColliderPixel");
	}
	if (selectComponent&& newComponent)
	{
		std::string	parentName = selectComponent->GetName() + "(" + selectComponent->GetComponentTypeName() + ")";
		selectComponent->AddChild(newComponent);
		componentWindow->AddItem((CComponent*)newComponent, name, parentName);
	}
	else
	{
		std::string	objName = selectObject->GetName() + "(" + selectObject->GetObjectTypeName() + ")";
		componentWindow->AddItem((CComponent*)newComponent, name, objName);
	}
}

void CClassWindow::LoadGameObjectName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	int	length = (int)strlen(path)-3;
	for (size_t i = length; i > 0; --i)		// Root Path에서 Bin\을 지워준다.
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i-1));
			break;
		}
	}
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	strcat_s(directory, "Include/GameObject/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ObjectList->AddItem(name);
	}
	length = (int)strlen(path) - 3;
	for (size_t i = length; i > 0; --i)	
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	memset(directory, 0, MAX_PATH);
	strcpy_s(directory, path);
	strcat_s(directory, "Engine/Include/GameObject/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ObjectList->AddItem(name);
	}
}

void CClassWindow::LoadComponentName()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	int	length = (int)strlen(path)-3;
	for (size_t i = length; i > 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	strcat_s(directory, "Include/Component/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		m_ComponentList->AddItem(name);
	}
	length = (int)strlen(path) - 3;
	for (size_t i = length; i > 0; --i)
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	memset(directory, 0, MAX_PATH);
	strcpy_s(directory, path);
	strcat_s(directory, "Engine/Include/Component/");
	for (const auto& file : std::filesystem::directory_iterator(directory))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		if (strcmp(ext, ".cpp") == 0)
		{
			continue;
		}
		else if (strcmp(name, "Transform") == 0 ||
				 strcmp(name, "Transform2D") == 0 ||
				 strcmp(name, "Collider") == 0 ||
				 strcmp(name, "Collider2D") == 0 ||
				 strcmp(name, "Collider3D") == 0 ||
				 strcmp(name, "Component") == 0 ||
				 strcmp(name, "PrimitiveComponent") == 0 ||
				 strcmp(name, "ObjectComponent") == 0||
				 strcmp(name, "Tile")==0)
		{
			continue;
		}
		m_ComponentList->AddItem(name);
	}
}
