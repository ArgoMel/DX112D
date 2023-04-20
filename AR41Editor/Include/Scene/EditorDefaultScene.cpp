#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/LoadingUI.h"
#include "../UI/MainMenuUI.h"
#include "../UI/UpperUI.h"
#include "../UI/PartyUI.h"
#include "../UI/EditPartyUI.h"
#include "../UI/TitleUI.h"
#include "../UI/GameClearUI.h"
#include "../GameObject/BackGround.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Boss.h"
#include "../GameObject/Bullet.h"

CEditorDefaultScene::CEditorDefaultScene()
{
	m_ClassTypeName = "EditorDefaultScene";
}

CEditorDefaultScene::~CEditorDefaultScene()
{
}

bool CEditorDefaultScene::Init()
{
	//m_Owner->CreateObject<CBackGround>("Background");
	//m_Owner->CreateObject<CBullet>("spike");
	//SetBossObject(m_Owner->CreateObject<CBoss>("Boss"));
	//SetPlayerObject(m_Owner->CreateObject<CPlayer2D>("Player2D"));
	//m_Owner->GetViewport()->CreateUIWindow<CGameClearUI>("GameClearUI");	//865 1080
	//m_Owner->GetViewport()->CreateUIWindow<CEditPartyUI>("EditPartyUI");	//배경 크기 세로 1450
	//m_Owner->GetViewport()->CreateUIWindow<CPartyUI>("PartyUI");
	//m_Owner->GetViewport()->CreateUIWindow<CMainMenuUI>("MainMenuUI");
	//m_Owner->GetViewport()->CreateUIWindow<CUpperUI>("UpperUI");
	//m_Owner->GetViewport()->CreateUIWindow<CTitleUI>("TitleUI");
	//m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");
	return true;
}