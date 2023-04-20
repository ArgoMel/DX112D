#include "ThreadManager.h"
#include "NavigationThread.h"
#include "../Component/TileMapComponent.h"
#include "../Scene/Scene.h"
DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{
}

CThreadManager::~CThreadManager()
{
	{
		auto	iter = m_mapThread.begin();
		auto	iterEnd = m_mapThread.end();
		for (; iter != iterEnd; ++iter)
		{
			iter->second->Stop();
			SAFE_DELETE(iter->second);
		}
		m_mapThread.clear();
	}
	{
		auto	iter = m_mapCriticalSection.begin();
		auto	iterEnd = m_mapCriticalSection.end();
		for (; iter != iterEnd; ++iter)
		{
			::DeleteCriticalSection(iter->second);	// 전역함수를 호출
			SAFE_DELETE(iter->second);
		}
		m_mapCriticalSection.clear();
	}
}

bool CThreadManager::Init()
{
	CreateCriticalSection("Loading");
	return true;
}

bool CThreadManager::Suspend(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Suspend();
	return true;
}

bool CThreadManager::Resume(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Resume();
	return true;
}

bool CThreadManager::ReStart(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->ReStart();
	return true;
}

bool CThreadManager::Delete(const std::string& name)
{
	auto	iter = m_mapThread.find(name);
	if (iter == m_mapThread.end())
	{
		return false;
	}
	iter->second->Stop();
	SAFE_DELETE(iter->second);
	m_mapThread.erase(iter);
	return true;
}

bool CThreadManager::Start(const std::string& name)
{
	CThread* thread = FindThread(name);
	if (!thread)
	{
		return false;
	}
	thread->Start();
	return true;
}

void CThreadManager::CreateNavigationThread(CTileMapComponent* tileMap)
{
	CScene* scene = tileMap->GetScene();
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = scene->GetName()+ "_" + tileMap->GetName() + "_"+ sceneAddress;

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CNavigationThread* thread = Create<CNavigationThread>(threadName);
		scene->GetNavigationManager()->AddNavigationThread(thread);
		thread->SetTileMapComponent(tileMap);
		thread->SetLoop(true);
		thread->Start();
		thread->Suspend();
	}
}

void CThreadManager::DeleteNavigationThread(CTileMapComponent* tileMap)
{
	CScene* scene = tileMap->GetScene();
	if (!scene)
	{
		return;
	}
	unsigned __int64	address = (unsigned __int64)scene;
	char	sceneAddress[32] = {};
	sprintf_s(sceneAddress, "%llu", address);
	std::string	name = tileMap->GetSceneName() + "_" + tileMap->GetName() + "_" + sceneAddress;

	SYSTEM_INFO	sysInfo = {};
	GetSystemInfo(&sysInfo);
	for (DWORD i = 0; i < sysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char	threadName[256] = {};
		sprintf_s(threadName, "%s_%d", name.c_str(), (int)i);
		CThread* thread = FindThread(threadName);
		thread->ReStart();
		Delete(threadName);
	}
}

CThread* CThreadManager::FindThread(const std::string& name)
{
	auto	iter = m_mapThread.find(name);
	if (iter == m_mapThread.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool CThreadManager::CreateCriticalSection(const std::string& name)
{
	CRITICAL_SECTION* crt = FindCriticalSection(name);
	if (crt)
	{
		return false;
	}
	crt = new CRITICAL_SECTION;
	InitializeCriticalSection(crt);
	m_mapCriticalSection.insert(std::make_pair(name, crt));
	return true;
}

bool CThreadManager::DeleteCriticalSection(const std::string& name)
{
	auto	iter = m_mapCriticalSection.find(name);
	if (iter == m_mapCriticalSection.end())
	{
		return false;
	}
	::DeleteCriticalSection(iter->second);
	SAFE_DELETE(iter->second);
	m_mapCriticalSection.erase(iter);
	return true;
}

CRITICAL_SECTION* CThreadManager::FindCriticalSection(const std::string& name)
{
	auto	iter = m_mapCriticalSection.find(name);
	if (iter == m_mapCriticalSection.end())
	{
		return nullptr;
	}
	return iter->second;
}
