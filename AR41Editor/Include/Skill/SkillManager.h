#pragma once
#include "Skill.h"

class CSkillManager
{
	DECLARE_SINGLE(CSkillManager)
private:
	friend class CBoss;
	friend class CPlayer2D;
	friend class CMonster;
	friend class CBullet;
	std::unordered_map<std::string, CSharedPtr<CSkill>>	m_mapSkill;
public:
	bool Init();
	CSkill* CreateSkill(const std::string& name);
	CSkill* FindSkill(const std::string& name);
	void ActivateSkill(const std::string& skillName, CComponent* obj, const std::string& objName, int createCount);
};