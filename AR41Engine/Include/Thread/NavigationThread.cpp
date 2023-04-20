#include "NavigationThread.h"
#include "DataStream.h"
#include "Navigation.h"
#include "../Component/NavigationAgent.h"
#include "../Component/SceneComponent.h"

CNavigationThread::CNavigationThread()
	: m_TileMap(nullptr)
	, m_Navigation(nullptr)
{
}

CNavigationThread::~CNavigationThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::SetTileMapComponent(CTileMapComponent* tileMap)
{
	m_TileMap = tileMap;
	SAFE_DELETE(m_Navigation);
	m_Navigation = new CNavigation;
	m_Navigation->CreateNavigation(tileMap);
}

void CNavigationThread::AddInputData(CNavigationAgent* agent, const Vector2& end)
{
	unsigned char	buffer[1024] = {};
	CDataStream	stream;
	stream.SetBuffer(buffer);
	unsigned __int64	address = (unsigned __int64)agent;
	stream.AddData<unsigned __int64>(&address, 8);
	Vector3	ownerPos = agent->GetUpdateComponent()->GetWorldPos();
	Vector2 start = Vector2(ownerPos.x, ownerPos.y);
	stream.AddData<Vector2>(&start, 8);
	stream.AddData<Vector2>(&end, 8);
	m_InputQueue.push(0, stream.GetSize(), buffer);
	if (IsSuspend())
	{
		ReStart();
	}
}

void CNavigationThread::Run()
{
	if (!m_InputQueue.empty())
	{
		int	header = 0;
		int size = 0;
		unsigned char	buffer[1024] = {};
		m_InputQueue.pop(&header, &size, buffer);
		CDataStream	stream;
		stream.SetBuffer(buffer);
		unsigned __int64	address = 0;
		stream.GetData<unsigned __int64>(&address, 8);
		CNavigationAgent* agent = (CNavigationAgent*)address;
		if (!agent)
		{
			if (m_InputQueue.empty())
			{
				Suspend();
			}
			return;
		}
		Vector2	start;
		Vector2 end;
		stream.GetData<Vector2>(&start, 8);
		stream.GetData<Vector2>(&end, 8);
		std::list<Vector2>	pathList;
		m_Navigation->FindPath(start, end, pathList);		// 길을 찾아준다.
		// 길찾기가 끝나면 결과를 반환해준다.
		agent->AddPathList(pathList);
	}
}