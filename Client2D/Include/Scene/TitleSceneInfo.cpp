#include "TitleSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/BackGround.h"

CTitleSceneInfo::CTitleSceneInfo()
{
}

CTitleSceneInfo::~CTitleSceneInfo()
{
}

bool CTitleSceneInfo::Init()
{
	CSceneInfo::Init();
	CBackGround* player = m_Owner->CreateObject<CBackGround>("Background");
	return true;
}
