#pragma once
#include "Thread.h"
#include "ThreadQueue.h"

class CNavigationThread : public CThread
{
private:
	friend class CThreadManager;
	class CTileMapComponent* m_TileMap;
	class CNavigation* m_Navigation;
	CThreadQueue	m_InputQueue;
protected:
	CNavigationThread();
	virtual ~CNavigationThread();
public:
	int GetWorkCount()
	{
		return m_InputQueue.size();
	}
	void SetTileMapComponent(class CTileMapComponent* tileMap);
	void AddInputData(class CNavigationAgent* agent, const Vector2& end);
	virtual void Run();
};

