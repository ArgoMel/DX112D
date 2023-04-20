#include "Scene.h"
#include "../Input.h"
#include "../PathManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SpriteComponent.h"
#include "../Component/CameraComponent.h"
#include "../Component/TargetArm.h"
#include "../Component/SceneComponent.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/TileMapComponent.h"
#include "../Component/NavigationAgent.h"
#include "../Animation/Animation2D.h"
#include "../UI/UIButton.h"
#include "../UI/UIImage.h"
#include "../UI/UIWindow.h"
std::unordered_map<std::string, CSceneInfo*> CScene::m_mapSceneInfoCDO;

CScene::CScene()
	: m_Change(false)
	, m_Start(false)
{
	m_Name = "Scene";
	m_SceneInfo = new CSceneInfo;
	m_SceneInfo->m_Owner = this;
	m_SceneInfo->Init();
	m_Resource = new CSceneResource;
	m_Resource->m_Owner = this;
	m_Resource->Init();
	m_CameraManager = new CCameraManager;
	m_CameraManager->m_Owner = this;
	m_CameraManager->Init();
	m_CollisionManager = new CSceneCollision;
	m_CollisionManager->m_Owner = this;
	m_CollisionManager->Init();
	m_Viewport = new CSceneViewport;
	m_Viewport->m_Owner = this;
	m_Viewport->Init();
	m_NavManager = new CNavigationManager;
	m_NavManager->m_Owner = this;
	m_NavManager->Init();
}

CScene::~CScene()
{
	CInput::GetInst()->ClearCallback(this);
	SAFE_DELETE(m_NavManager);
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CollisionManager);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_SceneInfo);
}

void CScene::CreateCDO()
{
// ==================== SceneInfo ====================
	CSceneInfo* info = new CSceneInfo;
	info->Init();
	CScene::AddSceneInfoCDO("SceneInfo", info);
// ==================== GameObject ====================
	CGameObject* objCDO = new CGameObject;
	objCDO->Init();
	CGameObject::AddObjectCDO("GameObject", objCDO);
// ==================== Component ====================
	CComponent* comCDO = new CSceneComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("SceneComponent", comCDO);
	comCDO = new CSpriteComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("SpriteComponent", comCDO);
	comCDO = new CCameraComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("CameraComponent", comCDO);
	comCDO = new CTargetArm;
	comCDO->Init();
	CComponent::AddComponentCDO("TargetArm", comCDO);
	comCDO = new CColliderBox2D;
	comCDO->Init();
	CComponent::AddComponentCDO("Box2D", comCDO);
	comCDO = new CTileMapComponent;
	comCDO->Init();
	CComponent::AddComponentCDO("TileMapComponent", comCDO);
	comCDO = new CNavigationAgent;
	comCDO->Init();
	CComponent::AddComponentCDO("NavigationAgent", comCDO);
// ==================== Animation ====================
	CAnimation2D* animCDO = new CAnimation2D;
	animCDO->Init();
	CAnimation2D::AddAnimationCDO("Animation2D", animCDO);
// ==================== UI ====================
	CUIWindow* UIWindowCDO = new CUIWindow;
	UIWindowCDO->Init();
	CUIWindow::AddUIWindowCDO("UIWindow", UIWindowCDO);
	CUIWidget* UIWidgetCDO = new CUIButton;
	UIWidgetCDO->Init();
	CUIWidget::AddUIWidgetCDO("UIButton", UIWidgetCDO);
	UIWidgetCDO = new CUIImage;
	UIWidgetCDO->Init();
	CUIWidget::AddUIWidgetCDO("UIImage", UIWidgetCDO);
}

void CScene::Start()
{
	m_Start = true;
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
	m_CameraManager->Start();
	m_Viewport->Start();
}

bool CScene::Init()
{
	return true;
}

void CScene::Update(float deltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->Update(deltaTime);
	}
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->Update(deltaTime);
		++iter;
	}
	m_CameraManager->Update(deltaTime);
	m_Viewport->Update(deltaTime);
}

void CScene::PostUpdate(float deltaTime)
{
	if (m_SceneInfo)
	{
		m_SceneInfo->PostUpdate(deltaTime);
	}
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		else if (!(*iter)->GetEnable())
		{
			++iter;
			continue;
		}
		(*iter)->PostUpdate(deltaTime);
		++iter;
	}
	m_CameraManager->PostUpdate(deltaTime);
	m_Viewport->PostUpdate(deltaTime);
}

void CScene::Collision(float deltaTime)
{
	m_CollisionManager->Update(deltaTime);
}

void CScene::Save(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return;
	}
	int	length = (int)m_Name.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Name.c_str(), 1, length, file);
	m_SceneInfo->Save(file);
	m_CameraManager->Save(file);
	m_CollisionManager->Save(file);
	m_Viewport->Save(file);
	int	objCount = (int)m_ObjList.size();
	fwrite(&objCount, 4, 1, file);
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		std::string	classTypeName = (*iter)->GetObjectTypeName();
		length = (int)classTypeName.length();
		fwrite(&length, 4, 1, file);
		fwrite(classTypeName.c_str(), 1, length, file);
		(*iter)->Save(file);
	}
	fclose(file);
}

void CScene::Load(const char* fullPath)
{
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return;
	}
//파일 크기 가져오기
	fseek(file, 0, SEEK_END);
	int	fileSize = (int)ftell(file);
	fseek(file, 0, SEEK_SET);
	int	loadSize = 0;
//불러오기 시작
	int	length = 0;
	char	name[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(name, 1, length, file);
	loadSize += 4 + length;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_Name = name;
	length = 0;
	char	sceneInfoName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(sceneInfoName, 1, length, file);
	loadSize += 4 + length;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	SAFE_DELETE(m_SceneInfo);
	CSceneInfo* CDO = FindSceneInfoCDO(sceneInfoName);
	if (!CDO)
	{
		CDO = FindSceneInfoCDO("SceneInfo");
	}
	m_SceneInfo = CDO->Clone();
	m_SceneInfo->m_Owner = this;

	m_SceneInfo->Load(file);
	int	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_CameraManager->m_Owner = this;
	m_CameraManager->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_CollisionManager->m_Owner = this;
	m_CollisionManager->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	m_Viewport->m_Owner = this;
	m_Viewport->Load(file);
	nextPos = (int)ftell(file);
	loadSize += (nextPos - loadSize);
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	int	objCount = 0;
	fread(&objCount, 4, 1, file);
	loadSize += 4;
	if (m_LoadingCallback)
	{
		m_LoadingCallback(loadSize / (float)fileSize);
	}

	for (int i = 0; i < objCount; ++i)
	{
		length = 0;
		char	objClassTypeName[256] = {};
		fread(&length, 4, 1, file);
		assert(length >= 0);
		fread(objClassTypeName, 1, length, file);
		loadSize += 4 + length;
		if (m_LoadingCallback)
		{
			m_LoadingCallback(loadSize / (float)fileSize);
		}

		CGameObject* objCDO = CGameObject::FindCDO(objClassTypeName);
		CGameObject* newObj = objCDO->Clone();
		newObj->SetScene(this);

		newObj->Load(file);
		nextPos = (int)ftell(file);
		loadSize += (nextPos - loadSize);
		{
			if (m_LoadingCallback)
			{
				m_LoadingCallback(loadSize / (float)fileSize);
			}
		}

		m_ObjList.push_back(newObj);
	}
	m_SceneInfo->LoadComplete();
	fclose(file);
}

void CScene::Save(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	Save(fullPath);
}

void CScene::Load(const char* fileName, const std::string& pathName)
{
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	char	fullPath[MAX_PATH] = {};
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, fileName);
	Load(fullPath);
}

void CScene::GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName)
{
	auto    iter = m_ObjList.begin();
	auto    iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		HierarchyObjectName	names;
		names.name = (*iter)->GetName();
		names.className = (*iter)->GetObjectTypeName();
		names.obj = *iter;
		vecName.push_back(names);
	}
}

CGameObject* CScene::FindObject(const std::string& name)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return nullptr;
}
