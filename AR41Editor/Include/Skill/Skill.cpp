#include "Skill.h"
#include "Device.h"
#include "Component/Component.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../GameObject/Player2D.h"
#include "../GameObject\Bullet.h"
#include "../GameObject\Monster.h"
#include "../GameObject\BackGround.h"
#include "../GameObject\Boss.h"

CSkill::CSkill()
{
	
}

CSkill:: ~CSkill()
{	
	auto	iter = m_vecSkillInfo.begin();
	auto	iterEnd = m_vecSkillInfo.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
}

void CSkill::ActivateSkill(const std::string& skillName, CComponent* obj, const std::string& objName, int createCount)
{
	size_t size = m_vecSkillInfo.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecSkillInfo[i]->name == skillName)
		{
			m_vecSkillInfo[i]->function(obj, objName, createCount);
		}
	}
}

void CSkill::AddSkill(const std::string& name, CSkill* obj, void(CSkill::* func)(CComponent*, const std::string&, int))
{
	SkillFunction* skill = new SkillFunction;
	skill->name = name;
	skill->function = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_vecSkillInfo.push_back(skill);
}

void CSkill::PowerFlipSword(CComponent* obj, const std::string& readerRole, int comboCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer2D* player = (CPlayer2D*)scene->GetSceneInfo()->GetPlayerObject();
	int attack = (int)(player->GetPlayerData(0).attack * (player->GetAbilityData(0).PF * 0.01f + 1.f));
	UserData userData = obj->GetOwner()->GetUserData();
	std::string name = userData.charName[userData.partyIndex * 6];
	int ranNum = rand() % 2;
	scene->GetResource()->SoundPlay("PowerFlip_" + name + std::to_string(ranNum));
	CBullet* bullet = scene->CreateObject<CBullet>("PowerFlip_Sword");
	bullet->GetBody()->SetCollisionProfile("PlayerAttack");
	if (comboCount > 40)
	{
		bullet->SetWorldScale(0.5f, 0.5f);
		bullet->GetSprite()->SetRelativeScale(500.f, 500.f);
		bullet->SetAtk(attack * 4);
	}
	else if (comboCount > 20)
	{
		bullet->SetWorldScale(0.15f, 0.15f);
		bullet->GetSprite()->SetRelativeScale(150.f, 150.f);
		bullet->SetAtk(attack * 2);
	}
	else
	{
		bullet->SetWorldScale(0.1f, 0.1f);
		bullet->GetSprite()->SetRelativeScale(100.f, 100.f);
		bullet->SetAtk(attack);
	}
}

void CSkill::TrippleArrow(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("FlapOut");
	obj->GetOwner()->SetMoveType(EMoveType::LeftRight);
	CBullet* bullet;
	for (int i = 0; i < createCount; ++i)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		bullet->AddWorldPositionY(-20.f);
		bullet->AddWorldRotation(0,0, (i - 1) * 30.f + 180.f);
	}
}
//일정범위안에 랜덤으로 생성
void CSkill::RandomSummon(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("BossSummon");
	Resolution RS = CDevice::GetInst()->GetResolution();
	CMonster* mon;
	for (int i = 0; i < createCount;++i)
	{
		float randomX = rand() % (int)((float)RS.width * 0.2f) + (float)RS.width * 0.4f;
		float randomY = rand() % (int)((float)RS.height * 0.2f) + (float)RS.height * 0.4f;
		mon = scene->CreateObject<CMonster>("enemy_"+objName);
		mon->SetWorldPosition(randomX,randomY);
	}
}

void CSkill::CreateBinObject(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CMonster* mon;
	for (int i = 0; i < createCount; ++i)
	{
		mon = scene->CreateObject<CMonster>("enemy_"+objName);
		mon->SetWorldPosition(0, 0);
	}
}

void CSkill::PushLeftOrRight(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("Flapping");
	Resolution RS = CDevice::GetInst()->GetResolution();
	float middleX = (float)RS.width * 0.5f;
	int dir= -1;
	if(scene->GetSceneInfo()->GetBossObject()->GetWorldPos().x> middleX)
	{
		dir =1;
	}
	CBullet* bullet;
	for (int i = 0; i < createCount; ++i)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(middleX +dir*i*40.f,1100.f- i*70.f);
		bullet->AddWorldRotation(0, 0,-225.f* dir);
	}
}

void CSkill::OwlSpecialSkill(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("OwlSpecial");
	obj->GetOwner()->SetMoveType(EMoveType::LeftRight);
	CBullet* bullet;
	for (int i = 0; i < createCount; ++i)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		bullet->AddWorldRotation(0, 0, i* 60.f);
		if(objName=="owl_blast")
		{
			bullet->SetWorldScale(0.33f, 0.33f);
			bullet->GetSprite()->SetRelativeScale(75.f, 75.f);
		}
		else
		{
			bullet->SetWorldScale(0.1f, 0.1f);
			bullet->GetSprite()->SetRelativeScale(25.f, 25.f);
			obj->Destroy();
		}
	}
}

void CSkill::GuidedBullet(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("ShotBullet");
	CBullet* bullet;
	for (int i = 0; i < createCount; ++i)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		bullet->AddWorldRotation(0, 0, i * 360.f/createCount);
	}
}
//좌우로 소환된다.
void CSkill::StaticSummon(CComponent* obj, const std::string& objName, int createCount)
{
	
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("BossSummon");
	CMonster* mon;
	for (int i = 2; i <= createCount+1; ++i)
	{
		int odd = i % 2 ? 1: -1;	//홀수판별
		mon = scene->CreateObject<CMonster>("enemy_" + objName);
		mon->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		mon->AddWorldPosition(odd*190.f*(int)(i/2),45.f);
	}
}

void CSkill::StraightLazer(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("ShotLazer");
	CBullet* bullet;
	for (int i = 0; i < createCount; ++i)
	{
		bullet = scene->CreateObject<CBullet>("straight_lazer");
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		bullet->GetBody()->SetCollisionProfile("Invisible");
		CColliderOBB2D* obb=bullet->CreateComponent<CColliderOBB2D>("OBB_"+objName);
		bullet->GetSprite()->AddChild(obb);
		if (objName == "rm-rf7")
		{
			bullet->GetSprite()->SetWorldScaleX(435.f);
			obb->SetWorldScale(2.f,9.f);
		}
		else
		{
			obb->SetBoxHalfSize(20.f, 400.f);
		}
		obb->SetRelativePosition(0.f, -500.f);
		obb->SetCollisionProfile("MonsterAttack");
		obb->SetCollisionCallback<CBullet>(ECollision_Result::Collision, bullet, &CBullet::CollisionBullet);
	}
}

void CSkill::StraightSummon(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("BossSummon");
	int ranNum = rand() % 2 ? 1: -1;
	CMonster* mon;
	for (int i = 0; i < createCount; ++i)
	{
		mon = scene->CreateObject<CMonster>("enemy_" + objName);
		mon->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		mon->AddWorldPosition(ranNum*(125.f+i * 60.f) , 0.f);
	}
}

void CSkill::BombRush(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("GolemSummon");
	Vector2	dir;
	CMonster* mon;
	int ranNum = rand() % 6;
	for (int i = 0; i < createCount; ++i)
	{
		dir.x = cosf(DegreeToRadian(60.f* (ranNum+i)));
		dir.y = sinf(DegreeToRadian(60.f * (ranNum + i)));
		mon = scene->CreateObject<CMonster>("enemy_" + objName);
		mon->SetWorldPosition(960.f,750.f);
		mon->AddWorldPosition(dir*200.f);
	}
}

void CSkill::RandomMove(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CBoss* boss = (CBoss*)scene->GetSceneInfo()->GetBossObject();
	boss->SetMoveType(EMoveType::Random);
	boss->SetRandomPos();
}

void CSkill::AddDefense(CComponent* obj, const std::string& objName, int createCount)
{
	obj->GetOwner()->SetBuff((int)EBuff::DefenceUp,10.f,10.f);
}

void CSkill::RushDown(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	obj->GetOwner()->SetMoveType(EMoveType::RushDown);
}

void CSkill::GrowUpBullet(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("ShotGrowUp");
	CBullet* bullet;
	if(1== createCount)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		return;
	}
	Vector2	dir;
	float degree = 360.f/ createCount;
	for (int i = 0; i < createCount; ++i)
	{
		dir.x = cosf(DegreeToRadian(degree *i));
		dir.y = sinf(DegreeToRadian(degree * i));
		bullet = scene->CreateObject<CBullet>("growup_bullet");
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		bullet->AddWorldPosition(dir * 50.f);
		bullet->SetSpeed(500.f);
	}
}

void CSkill::Explosion(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("Explosion");
	CBullet* bullet;
	if (1 == createCount)
	{
		bullet = scene->CreateObject<CBullet>(objName);
		bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
		return;
	}
}

void CSkill::MeteorBreak(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("Explosion");
	CPlayer2D* player = (CPlayer2D*)scene->GetSceneInfo()->GetPlayerObject();
	int attack = (int)(player->GetPlayerData(createCount).attack * player->GetSkillDamage(createCount)*
		((player->GetAbilityData(createCount).Skill + player->GetAbilityData(0).Party_Skill)* 0.01f + 1.f));
	CBullet* bullet = scene->CreateObject<CBullet>(objName);	//Meteor Break
	bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
	bullet->AddWorldPositionY(150.f);
	bullet->GetBody()->SetCollisionProfile("PlayerAttack");
	bullet->SetAtk(attack);
}

void CSkill::RushToBoss(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer2D* player = (CPlayer2D*)scene->GetSceneInfo()->GetPlayerObject();
	int attack = (int)(player->GetPlayerData(createCount).attack * player->GetSkillDamage(createCount) *
		((player->GetAbilityData(createCount).Skill + player->GetAbilityData(0).Party_Skill) * 0.01f + 1.f));
	player->GetRigidBody()->SetVelocity(0.f, 0.f);
	player->SetMoveType(EMoveType::RushToBoss);
	CBullet* bullet = scene->CreateObject<CBullet>(objName);
	bullet->SetWorldPosition(obj->GetOwner()->GetWorldPos());
	bullet->GetBody()->SetCollisionProfile("PlayerAttack");
	bullet->GetSprite()->GetMaterial(0)->SetOpacity(0.f);
	bullet->SetAtk(attack);
	bullet->SetHitCount(player->GetHitCount(createCount));
	bullet->SetMoveType(EMoveType::RushToBoss);
}

void CSkill::Stella(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	scene->GetResource()->SoundPlay("Stella");
	CBoss* boss = (CBoss*)scene->GetSceneInfo()->GetBossObject();
	boss->SetDeBuff((int)EDeBuff::Paralysis, 10.f, 0.f);
}

void CSkill::ShaSusu(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer2D* player = (CPlayer2D*)scene->GetSceneInfo()->GetPlayerObject();
	player->GainSkillGauge(0,30.f);
	player->SetBuff((int)EBuff::SkillUp, 15.f, 100.f);
}

void CSkill::Lazeret(CComponent* obj, const std::string& objName, int createCount)
{
	CScene* scene = CSceneManager::GetInst()->GetScene();
	CPlayer2D* player = (CPlayer2D*)scene->GetSceneInfo()->GetPlayerObject();
	player->SetBuff((int)EBuff::AttackUp, 15.f, 50.f);
	player->SetBuff((int)EBuff::DefenceUp, 15.f, 20.f);
	//재생 100
}