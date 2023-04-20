#include "MainSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Monster.h"

CMainSceneInfo::CMainSceneInfo()
{
}

CMainSceneInfo::~CMainSceneInfo()
{
}

bool CMainSceneInfo::Init()
{
	CSceneInfo::Init();
	CPlayer2D* player = m_Owner->CreateObject<CPlayer2D>("Player2D");
	SetPlayerObject(player);
	CMonster* Monster = m_Owner->CreateObject<CMonster>("Monster");
	return true;
}
