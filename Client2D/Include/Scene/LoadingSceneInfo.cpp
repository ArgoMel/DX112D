#include "LoadingSceneInfo.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "Thread/ThreadManager.h"
#include "Thread/LoadingThread.h"
#include "Thread/ThreadQueue.h"
#include "Thread/DataStream.h"
#include "Editor/EditorGUIManager.h"
#include "../UI/LoadingUI.h"

#ifdef __has_include
#	if __has_include("../Window/ObjectWindow.h")
#		include "../Window/ObjectWindow.h"
//#		define have_ObjectWindow 1
//#	else
//#		define have_ObjectWindow 0
#	endif
#endif

CLoadingSceneInfo::CLoadingSceneInfo()
	: m_LoadingThread(nullptr)
	, m_LoadingQueue(nullptr)
	, m_LoadingUI(nullptr)
{
	m_ClassTypeName = "LoadingSceneInfo";
}

CLoadingSceneInfo::~CLoadingSceneInfo()
{
}

bool CLoadingSceneInfo::Init()
{
	m_LoadingUI=m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");
	return true;
}

void CLoadingSceneInfo::Update(float deltaTime)
{
	CSceneInfo::Update(deltaTime);
	if (!m_LoadingQueue->empty())
	{
		int	header;
		int size;
		unsigned char	data[1024] = {};
		m_LoadingQueue->pop(&header, &size, data);
		CDataStream	stream;
		stream.SetBuffer(data);
		float	rate = 0.f;
		stream.GetData<float>(&rate, 4);
		m_LoadingUI->SetLoadingPercent(rate);
	}
	if (m_LoadingThread->IsLoadComplete())
	{
		CScene* nextScene = CSceneManager::GetInst()->GetNextScene();
		nextScene->GetSceneInfo()->SetFileName(m_FileName);
		nextScene->GetSceneInfo()->SetPrevFileName(m_PrevFileName);
		CSceneManager::GetInst()->ChangeNextScene();
		CThreadManager::GetInst()->Delete("Loading");
#if __has_include("../Window/ObjectWindow.h")
		CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
		if (objectWindow)
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
				objectWindow->AddItem(vecWindowName[i].window, vecWindowName[i].className);
			}
		}
#endif
	}
}

void CLoadingSceneInfo::SceneChangeComplete()
{
	m_LoadingThread = CThreadManager::GetInst()->Create<CLoadingThread>("Loading");
	m_LoadingThread->SetLoadingSceneFileName(m_FileName);
	m_LoadingQueue = m_LoadingThread->GetQueue();
	m_LoadingThread->Start();
}
