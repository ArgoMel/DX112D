#include "SkillManager.h"
#include "Component/Component.h"
DEFINITION_SINGLE(CSkillManager)

CSkillManager::CSkillManager()
{
}

CSkillManager::~CSkillManager()
{
}
// ~~오브젝트가 ~~스킬을 발동한다
bool CSkillManager::Init()
{
//owl
	CSkill* skill = CreateSkill("owl0");
	skill->AddSkill("owl0", skill, &CSkill::RandomSummon);
	skill = CreateSkill("owl1");
	skill->AddSkill("owl1", skill, &CSkill::TrippleArrow);
	skill = CreateSkill("owl2");
	skill->AddSkill("owl2", skill, &CSkill::CreateBinObject);
	skill = CreateSkill("owl3");
	skill->AddSkill("owl3", skill, &CSkill::OwlSpecialSkill);
	skill = CreateSkill("owl_blast");
	skill->AddSkill("owl_blast", skill, &CSkill::OwlSpecialSkill);
	skill = CreateSkill("enemy_galeforce");
	skill->AddSkill("enemy_galeforce", skill, &CSkill::PushLeftOrRight);
//administrator_light
	skill = CreateSkill("administrator_light0");
	skill->AddSkill("administrator_light0", skill, &CSkill::RandomSummon);
	skill = CreateSkill("administrator_light1");
	skill->AddSkill("administrator_light1", skill, &CSkill::StaticSummon);
	skill = CreateSkill("administrator_light2");
	skill->AddSkill("administrator_light2", skill, &CSkill::StraightSummon);
	skill = CreateSkill("administrator_light3");
	skill->AddSkill("administrator_light3", skill, &CSkill::StraightLazer);
	skill = CreateSkill("enemy_Protection");
	skill->AddSkill("enemy_Protection", skill, &CSkill::GuidedBullet);
	skill = CreateSkill("enemy_HiltShot");
	skill->AddSkill("enemy_HiltShot", skill, &CSkill::GuidedBullet);
	skill = CreateSkill("enemy_KillNine");
	skill->AddSkill("enemy_KillNine", skill, &CSkill::StraightLazer);
//golem
    skill = CreateSkill("guardian_golem0");
	skill->AddSkill("guardian_golem0", skill, &CSkill::BombRush);
	skill = CreateSkill("guardian_golem1");
	skill->AddSkill("guardian_golem1", skill, &CSkill::RandomMove);
	skill = CreateSkill("guardian_golem2");
	skill->AddSkill("guardian_golem2", skill, &CSkill::AddDefense);
	skill = CreateSkill("guardian_golem3");
	skill->AddSkill("guardian_golem3", skill, &CSkill::RushDown);
	skill = CreateSkill("guardian_golem4");
	skill->AddSkill("guardian_golem4", skill, &CSkill::GrowUpBullet);
	skill = CreateSkill("guardian_golem5");
	skill->AddSkill("guardian_golem5", skill, &CSkill::Explosion);
	skill = CreateSkill("enemy_Bomb Rush");
	skill->AddSkill("enemy_Bomb Rush", skill, &CSkill::GrowUpBullet);
//PowerFlip
	skill = CreateSkill("PowerFlip0");
	skill->AddSkill("PowerFlip0", skill, &CSkill::PowerFlipSword);
//skill
	skill = CreateSkill("alk");
	skill->AddSkill("alk", skill, &CSkill::MeteorBreak);
	skill = CreateSkill("stella_2anv");
	skill->AddSkill("stella_2anv", skill, &CSkill::Stella);
	skill = CreateSkill("white_tiger");
	skill->AddSkill("white_tiger", skill, &CSkill::RushToBoss);
	skill = CreateSkill("zeta");
	skill->AddSkill("zeta", skill, &CSkill::RushToBoss);
	skill = CreateSkill("student_gunsmith");
	skill->AddSkill("student_gunsmith", skill, &CSkill::ShaSusu);
	skill = CreateSkill("prince_zero");
	skill->AddSkill("prince_zero", skill, &CSkill::Lazeret);
	return true;
}

CSkill* CSkillManager::CreateSkill(const std::string& name)
{
	CSkill* skill = FindSkill(name);
	if (skill)
	{
		return nullptr;
	}
	skill = new CSkill;
	skill->SetName(name);
	m_mapSkill.insert(std::make_pair(name, skill));
	return skill;
}

CSkill* CSkillManager::FindSkill(const std::string& name)
{
	auto	iter = m_mapSkill.find(name);
	if (iter == m_mapSkill.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CSkillManager::ActivateSkill(const std::string& skillName, CComponent* obj, const std::string& objName, 
	int createCount)
{
	CSkill* skill = FindSkill(skillName);
	if (!skill)
	{
		return;
	}
	skill->ActivateSkill(skillName, obj, objName, createCount);
}
