#pragma once
#include "../EngineInfo.h"

class CNavigationManager
{
private:
	friend class CScene;
	friend class CSceneManager;
	class CScene* m_Owner;
	std::vector<class CNavigationThread*>	m_vecNavThread;
protected:
	CNavigationManager();
	~CNavigationManager();
public:
	void AddNavigationThread(class CNavigationThread* thread)
	{
		m_vecNavThread.push_back(thread);
	}
	void Start();
	bool Init();
	void Move(class CNavigationAgent* agent, const Vector2& end);
};

