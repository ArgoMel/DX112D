#include "SceneWindow.h"
#include "ObjectWindow.h"
#include "PathManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInputText.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorGUIManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneInfo.h"
#include "../Scene/EditorDefaultScene.h"
#include "../Scene/LoadingSceneInfo.h"

CSceneWindow::CSceneWindow()
	: m_SceneList(nullptr)
	, m_SceneInfoList(nullptr)
	, m_SceneSaveName(nullptr)
	, m_SceneSelectName(nullptr)
{
}

CSceneWindow::~CSceneWindow()
{
}

bool CSceneWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("장면 정보");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSizeY(30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_SceneSaveName = CreateWidget<CEditorInputText>("SceneSaveName");
	m_SceneSaveName->SetHideName("SceneSaveName");
	m_SceneSaveName->SetSize(150.f, 30.f);
	CEditorButton* button = CreateWidget<CEditorButton>("Scene 변경", 100.f, 30.f);
	button->SetColor(29, 47, 73, 255);
	button->SetClickCallback<CSceneWindow>(this, &CSceneWindow::SceneChange);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("Scene 저장", 100.f, 30.f);
	button->SetClickCallback<CSceneWindow>(this, &CSceneWindow::SceneSave);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("Scene 생성", 100.f, 30.f);
	button->SetClickCallback<CSceneWindow>(this, &CSceneWindow::SceneCreate);
	m_SceneSelectName = CreateWidget<CEditorInputText>("SceneName");
	m_SceneSelectName->SetHideName("SceneName");
	m_SceneSelectName->SetSize(150.f, 30.f);
	m_SceneSelectName->ReadOnly(true);
	m_SceneList = CreateWidget<CEditorListBox>("SceneListBox");
	m_SceneList->SetHideName("SceneListBox");
	m_SceneList->SetSize(150.f, 300.f);
	m_SceneList->SetPageItemCount(6);
	m_SceneList->SetSelectCallback<CSceneWindow>(this, &CSceneWindow::SceneClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_SceneInfoList = CreateWidget<CEditorListBox>("SceneInfoListBox");
	m_SceneInfoList->SetHideName("SceneInfoListBox");
	m_SceneInfoList->SetSize(150.f, 300.f);
	m_SceneInfoList->SetPageItemCount(6);
	m_SceneInfoList->SetSelectCallback<CSceneWindow>(this, &CSceneWindow::SceneInfoClickCallback);
	LoadSceneDirectory();
	LoadSceneInfoDirectory();
	return true;
}

void CSceneWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CSceneWindow::SceneChange()
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	if (scene->GetName() == m_SelectSceneItem && !m_SelectSceneItem.empty())
	{
		return;
	}
	CSceneManager::GetInst()->CreateNextScene();
	CScene* nextScene = CSceneManager::GetInst()->GetNextScene();
	const PathInfo* info = CPathManager::GetInst()->FindPath(SCENE_PATH);
	char	fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, m_SelectSceneItem.c_str());
	strcat_s(fullPath, ".scn");
	nextScene->Load(fullPath);
	CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
	if(objectWindow)
	{
		objectWindow->Clear();
		objectWindow->ClearSelect();
		std::vector<HierarchyObjectName> vecName;
		nextScene->GetAllGameObjectHierarchyName(vecName);
		size_t	size = vecName.size();
		for (size_t i = 0; i < size; ++i)
		{
			objectWindow->AddItem(vecName[i].obj, vecName[i].name);
		}
		std::vector<HierarchyWindowName> vecWindowName;
		nextScene->GetViewport()->GetAllWindowHierarchyName(vecWindowName);
		size = vecWindowName.size();
		for (size_t i = 0; i < size; ++i)
		{
			objectWindow->AddItem(vecWindowName[i].window, vecWindowName[i].name);
		}
	}
}

void CSceneWindow::SceneSave()
{
	std::string name = m_SceneSaveName->GetText();
	if (name.empty())
	{
		return;
	}
	const PathInfo* info = CPathManager::GetInst()->FindPath(SCENE_PATH);
	char	fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->pathMultibyte);
	}
	strcat_s(fullPath, name.c_str());
	strcat_s(fullPath, ".scn");
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->SetName(name);
	scene->Save(fullPath);
	if (!m_SceneList->CheckItem(name))
	{
		m_SceneList->AddItem(name);
	}
}

void CSceneWindow::SceneCreate()
{
	if (m_SelectSceneInfoItem.empty())
	{
		return;
	}
	if(m_SelectSceneInfoItem=="EditorDefaultScene")
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneInfo<CEditorDefaultScene>(false);
	}
	else if (m_SelectSceneInfoItem == "LoadingSceneInfo")
	{
		CSceneManager::GetInst()->CreateNextScene();
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false);
	}
}

void CSceneWindow::SceneClickCallback(int index, const std::string& item)
{
	m_SelectSceneItem = item;
	m_SceneSelectName->SetText(item.c_str());
}

void CSceneWindow::SceneInfoClickCallback(int index, const std::string& item)
{
	m_SelectSceneInfoItem = item;
}

void CSceneWindow::LoadSceneDirectory()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	strcat_s(path, "Scene/");
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		char	name[64] = {};
		char	fullPath[MAX_PATH] = {};
		char	ext[_MAX_EXT] = {};
		strcpy_s(fullPath, file.path().generic_string().c_str());
		_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, 64, ext, _MAX_EXT);
		m_SceneList->AddItem(name);
	}
}

void CSceneWindow::LoadSceneInfoDirectory()
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(ROOT_PATH);
	char	path[MAX_PATH] = {};
	strcpy_s(path, info->pathMultibyte);
	int	length = (int)strlen(path) - 3;
	for (size_t i = length; i > 0; --i)		// Root Path에서 Bin\을 지워준다.
	{
		if (path[i] == '/' || path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}
	char	directory[MAX_PATH] = {};
	strcpy_s(directory, path);
	strcat_s(directory, "Include/Scene/");
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
		m_SceneInfoList->AddItem(name);
	}
}