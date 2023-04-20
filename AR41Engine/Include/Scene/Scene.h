#pragma once
#include "SceneInfo.h"
#include "SceneResource.h"
#include "CameraManager.h"
#include "SceneCollision.h"
#include "SceneViewport.h"
#include "NavigationManager.h"

class CScene
{
private:
	friend class CSceneManager;
	static std::unordered_map<std::string, CSceneInfo*>	m_mapSceneInfoCDO;
	static void CreateCDO();
	CSceneInfo* m_SceneInfo;
	CSceneResource* m_Resource;
	CCameraManager* m_CameraManager;
	CSceneCollision* m_CollisionManager;
	CSceneViewport* m_Viewport;
	CNavigationManager* m_NavManager;
	std::string	m_Name;
	std::function<void(float)>	m_LoadingCallback;
	bool		m_Change;
	bool		m_Start;
	std::list<CSharedPtr<class CGameObject>>	m_ObjList;
	CScene();
	~CScene();
public:
	static void AddSceneInfoCDO(const std::string& name, CSceneInfo* info)
	{
		m_mapSceneInfoCDO.insert(std::make_pair(name, info));
	}
	static CSceneInfo* FindSceneInfoCDO(const std::string& name)
	{
		auto	iter = m_mapSceneInfoCDO.find(name);
		if (iter == m_mapSceneInfoCDO.end())
		{
			return nullptr;
		}
		return iter->second;
	}
	template <typename T>
	static T* CreateObjectCDO(const std::string& name)
	{
		T* CDO = new T;
		CDO->Init();
		CGameObject::AddObjectCDO(name, (CGameObject*)CDO);
		return CDO;
	}
	template <typename T>
	static T* CreateUIWindowCDO(const std::string& name)
	{
		T* CDO = new T;
		CDO->Init();
		CUIWindow::AddUIWindowCDO(name, (CUIWindow*)CDO);
		return CDO;
	}
	template <typename T>
	static T* CreateUIWidgetCDO(const std::string& name)
	{
		T* CDO = new T;
		CDO->Init();
		CUIWidget::AddUIWidgetCDO(name, (CUIWidget*)CDO);
		return CDO;
	}
	CSceneInfo* GetSceneInfo()	const
	{
		return m_SceneInfo;
	}
	CSceneResource* GetResource()	const
	{
		return m_Resource;
	}
	CCameraManager* GetCameraManager()	const
	{
		return m_CameraManager;
	}
	CSceneCollision* GetCollisionManager()	const
	{
		return m_CollisionManager;
	}
	CSceneViewport* GetViewport()	const
	{
		return m_Viewport;
	}
	CNavigationManager* GetNavigationManager()	const
	{
		return m_NavManager;
	}
	const std::string& GetName()	const
	{
		return m_Name;
	}
	void SetName(const std::string& name)
	{
		m_Name = name;
	}
	void Start();
	bool Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void Collision(float deltaTime);
	void Save(const char* fullPath);
	void Load(const char* fullPath);
	void Save(const char* fileName, const std::string& pathName);
	void Load(const char* fileName, const std::string& pathName);
	void GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName);
	class CGameObject* FindObject(const std::string& name);
	template <typename T>
	bool CreateSceneInfo(const std::string& name = "", const std::string& prevName = "")
	{
		SAFE_DELETE(m_SceneInfo);
		m_SceneInfo = new T;
		m_SceneInfo->m_Owner = this;
		if (name != "")
		{
			m_SceneInfo->SetFileName(name);
		}
		if (prevName != "")
		{
			m_SceneInfo->SetPrevFileName(prevName);
		}
		if (!m_SceneInfo->Init())
		{
			SAFE_DELETE(m_SceneInfo);
			return false;
		}
		return true;
	}
	template <typename T>
	T* CreateObject(const std::string& name)
	{
		T* obj = new T;
		obj->SetName(name);
		obj->SetScene(this);
		if (!obj->Init())
		{
			SAFE_RELEASE(obj);
			return nullptr;
		}
		m_ObjList.push_back(obj);
		if (m_Start)
		{
			obj->Start();
		}
		return obj;
	}
	template <typename T>
	void SetLoadingCallback(T* obj, void(T::* func)(float))
	{
		m_LoadingCallback = std::bind(func, obj, std::placeholders::_1);
	}
};

