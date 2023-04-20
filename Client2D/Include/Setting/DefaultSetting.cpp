#include "DefaultSetting.h"
#include "Input.h"
#include "CollisionManager.h"
#include "Scene/Scene.h"
#include "../GameObject/Player2D.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Bullet.h"
#include "../GameObject/Boss.h"
#include "../GameObject/BackGround.h"
#include "../GameObject/Marker.h"
#include "../UI/MainmenuUI.h"
#include "../UI/UpperUI.h"
#include "../UI/PartyUI.h"
#include "../UI/EditPartyUI.h"
#include "../UI/StageUI.h"
#include "../UI/BossUI.h"
#include "../UI/TitleUI.h"
#include "../UI/GameClearUI.h"
#include "../Skill/SkillManager.h"

CDefaultSetting::CDefaultSetting()
{
}

CDefaultSetting::~CDefaultSetting()
{
    CSkillManager::DestroyInst();
}

void CDefaultSetting::Init()
{
    LoadResource();
	CreateCDO();
	SetInput();
	SetCollision();
    CSkillManager::GetInst()->Init();
}

void CDefaultSetting::CreateCDO()
{
	CScene::CreateObjectCDO<CPlayer2D>("Player2D");
	CScene::CreateObjectCDO<CMonster>("Monster");
	CScene::CreateObjectCDO<CBullet>("Bullet");
	CScene::CreateObjectCDO<CBoss>("Boss");
	CScene::CreateObjectCDO<CBackGround>("BackGround");
	CScene::CreateObjectCDO<CMarker>("Marker");

	CScene::CreateUIWindowCDO<CMainMenuUI>("MainMenuUI");
	CScene::CreateUIWindowCDO<CUpperUI>("UpperUI");
	CScene::CreateUIWindowCDO<CPartyUI>("PartyUI");
	CScene::CreateUIWindowCDO<CEditPartyUI>("EditPartyUI");
	CScene::CreateUIWindowCDO<CStageUI>("StageUI");
	CScene::CreateUIWindowCDO<CBossUI>("BossUI");
	CScene::CreateUIWindowCDO<CTitleUI>("TitleUI");
	CScene::CreateUIWindowCDO<CGameClearUI>("GameClearUI");
}

void CDefaultSetting::LoadResource()
{
    CResourceManager* resourceManager= CResourceManager::GetInst();
//mouse

    //std::vector<const TCHAR*>   vecFileName;
    //for (int i = 0; i <= 12; ++i)
    //{
    //    TCHAR	fileName[MAX_PATH] = {};
    //    wsprintf(fileName, TEXT("Mouse\\Default\\%d.png"), i);
    //    vecFileName.push_back(fileName);
    //}
    //resourceManager->CreateAnimationSequence2D("MouseDefault", "MouseDefault", vecFileName);
    //for (int i = 0; i < 11; ++i)
    //{
    //    resourceManager->AddAnimationSequence2DFrameAll("MouseDefault",13, 0.f, 0.f, 32.f, 31.f);
    //}
    //vecFileName.clear();
    
//anime
    for(int i=0;i<3;++i)
    {
        resourceManager->CreateAnimationSequence2D("Damage" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("Win" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("Skill" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("Back" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("Idle" + std::to_string(i), "Temp", TEXT("neutral.png"));
    }
    resourceManager->CreateAnimationSequence2D("BossIdle", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("BossDie", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("BossStun", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("BossCharge", "Temp", TEXT("neutral.png"));
    for (int i = 0; i < 10; ++i)
    {
        resourceManager->CreateAnimationSequence2D("BossSkill" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("MonsterIdle" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("MonsterDie" + std::to_string(i), "Temp", TEXT("neutral.png"));
        resourceManager->CreateAnimationSequence2D("MonsterSkill" + std::to_string(i), "Temp", TEXT("neutral.png"));
    }
    resourceManager->CreateAnimationSequence2D("BulletIdle", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("BulletDie", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("MarkerIdle", "Temp", TEXT("neutral.png"));
    resourceManager->CreateAnimationSequence2D("MarkerBreak", "Temp", TEXT("neutral.png"));
//sound
    resourceManager->LoadSound("Effect", "PopWindow", false, "click.mp3");
    resourceManager->LoadSound("Effect", "CharGrap", false, "System\\se_flick.mp3");
    resourceManager->LoadSound("Effect", "CharChange", false, "System\\se_orb_get_light.mp3");
    resourceManager->LoadSound("Effect", "CharRelease", false, "System\\se_go_back_click_2.mp3");
    resourceManager->LoadSound("Effect", "ActivateSkill", false, "System\\se_uncap_01.mp3");
    resourceManager->LoadSound("Effect", "GameReady", false, "System\\se_manaboard_extend_05.mp3");
    resourceManager->LoadSound("Effect", "PlayerRelease", false, "System\\se_manaboard_extend_04.mp3");
    resourceManager->LoadSound("Effect", "PlayerTouch", false, "System\\se_stage_node_dotmark.mp3");
    resourceManager->LoadSound("Effect", "PlayerHit", false, "sound_effect\\blow\\se_ball_plock.mp3");
    resourceManager->LoadSound("Effect", "PlayerDie", false, "sound_effect\\situation\\se_horror.mp3");
    resourceManager->LoadSound("Effect", "MonsterDie", false, "sound_effect\\blow\\se_ball_plock_stroke.mp3");
    resourceManager->LoadSound("Effect", "BossDie", false, "sound_effect\\element\\se_break_object.mp3");
    resourceManager->LoadSound("Effect", "BossCharge", false, "sound_effect\\monster\\se_physics_charge.mp3");
    resourceManager->LoadSound("Effect", "BossSummon", false, "sound_effect\\monster\\se_spit_out.mp3");
    resourceManager->LoadSound("Effect", "BossStun", false, "System\\se_orb_get_light.mp3");
    resourceManager->LoadSound("Effect", "Flapping", false, "sound_effect\\monster\\se_flapping.mp3");
    resourceManager->LoadSound("Effect", "FlapOut", false, "sound_effect\\monster\\se_flap_out.mp3");
    resourceManager->LoadSound("Effect", "OwlSpecial", false, "sound_effect\\monster\\se_scream1.mp3");
    resourceManager->LoadSound("Effect", "ShotBullet", false, "sound_effect\\thunder\\se_thunder_shot.mp3");
    resourceManager->LoadSound("Effect", "ShotLazer", false, "sound_effect\\thunder\\se_electric_beam.mp3");
    resourceManager->LoadSound("Effect", "ShotGrowUp", false, "sound_effect\\thunder\\se_thunder_sonic_slash.mp3");
    resourceManager->LoadSound("Effect", "GolemSummon", false, "sound_effect\\blow\\se_heavy_stamp.mp3");
    resourceManager->LoadSound("Effect", "Explosion", false, "sound_effect\\fire\\se_medium_explosion.mp3");
    resourceManager->LoadSound("Effect", "Stella", false, "sound_effect\\light\\se_light_bright_shooting_star.mp3");
    resourceManager->LoadSound("UI", "ButtonClick1", false, "System\\se_character_exchange.mp3");
    resourceManager->LoadSound("UI", "ButtonClick2", false, "sound_effect\\yakumono\\se_mana_get.mp3");
    resourceManager->LoadSound("UI", "ButtonClick3", false, "System\\se_character_speech.mp3");
    resourceManager->LoadSound("BGM", "Title", true, "BGM\\worldflipper_town_quest.mp3");
    resourceManager->LoadSound("BGM", "Lobby", true, "BGM\\worldflipper_town_quest_anniversary_melody.mp3");
    resourceManager->LoadSound("BGM", "Party", true, "BGM\\general_purpose_quest_select.mp3");
    resourceManager->LoadSound("BGM", "Clear", true, "BGM\\character_story_victory.mp3");
    resourceManager->LoadSound("BGM", "owl", true, "BGM\\challenge_dungeon\\battle_challenge_dungeon_1.mp3");
    resourceManager->LoadSound("BGM", "administrator_light", true, "BGM\\challenge_dungeon\\battle_challenge_dungeon_2.mp3");
    resourceManager->LoadSound("BGM", "guardian_golem", true, "BGM\\challenge_dungeon\\battle_challenge_dungeon_3.mp3");
    resourceManager->SetVolume("Effect", 20);
    resourceManager->SetVolume("Voice", 30);
    resourceManager->SetVolume("BGM",15);
    resourceManager->SetVolume("UI",40);
}

void CDefaultSetting::SetInput()
{
	CInput::GetInst()->AddBindKey("Touch", VK_LBUTTON);
	CInput::GetInst()->AddBindKey("Skill", VK_RBUTTON);
	CInput::GetInst()->AddBindKey("Pause", VK_MBUTTON);
	CInput::GetInst()->AddBindKey("RePosition", VK_F1);
	CInput::GetInst()->AddBindKey("ChangePos", VK_F2);
	CInput::GetInst()->AddBindKey("Pause2", VK_F9);
	CInput::GetInst()->AddBindKey("Del", VK_DELETE);
    CInput::GetInst()->AddBindKey("Del2", 'D');
    CInput::GetInst()->SetKeyCtrl("Del2");
    //CInput::GetInst()->AddBindKey("MoveClick", VK_F11);   //길찾기 테스트용
}

void CDefaultSetting::SetCollision()
{
    CCollisionManager::GetInst()->CreateChannel("Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->CreateChannel("PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Invisible", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateChannel("Marker", ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->CreateProfile("Player", "Player", true);
    CCollisionManager::GetInst()->CreateProfile("PlayerAttack", "PlayerAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Monster", "Monster", true);
    CCollisionManager::GetInst()->CreateProfile("MonsterAttack", "MonsterAttack", true);
    CCollisionManager::GetInst()->CreateProfile("Invisible", "Invisible", true, ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->CreateProfile("Marker", "Marker", true, ECollision_Interaction::Ignore);

    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "MonsterAttack", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Player", "Marker", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("PlayerAttack", "Marker", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Default", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
    CCollisionManager::GetInst()->SetCollisionInteraction("Marker", "Player", ECollision_Interaction::Collision);
    CCollisionManager::GetInst()->SetCollisionInteraction("Marker", "PlayerAttack", ECollision_Interaction::Collision);
}

