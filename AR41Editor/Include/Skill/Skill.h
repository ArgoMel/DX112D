#pragma once
#include "Ref.h"

struct SkillFunction
{
	std::string	name; // ¿Ã∏ß
	std::function<void(CComponent* ,const std::string&, int)>	function;
};

class CSkill : public CRef
{
private:
	friend class CSkillManager;
	CSkill();
	virtual ~CSkill();
protected:
	std::vector<SkillFunction*>   m_vecSkillInfo;
public:
	void ActivateSkill(const std::string& skillName, CComponent* obj, const std::string& objName, int createCount);
	void AddSkill(const std::string& name, CSkill* obj, void(CSkill::* func)(CComponent*, const std::string&, int));
	void PowerFlipSword(CComponent* obj, const std::string& readerRole, int comboCount);
	void TrippleArrow(CComponent* obj, const std::string& objName, int createCount);
	void RandomSummon(CComponent* obj, const std::string& objName, int createCount);
	void CreateBinObject(CComponent* obj, const std::string& objName, int createCount);
	void PushLeftOrRight(CComponent* obj, const std::string& objName, int createCount);
	void OwlSpecialSkill(CComponent* obj, const std::string& objName, int createCount);
	void GuidedBullet(CComponent* obj, const std::string& objName, int createCount);
	void StaticSummon(CComponent* obj, const std::string& objName, int createCount);
	void StraightLazer(CComponent* obj, const std::string& objName, int createCount);
	void StraightSummon(CComponent* obj, const std::string& objName, int createCount);
	void BombRush(CComponent* obj, const std::string& objName, int createCount);
	void RandomMove(CComponent* obj, const std::string& objName, int createCount);
	void AddDefense(CComponent* obj, const std::string& objName, int createCount);
	void RushDown(CComponent* obj, const std::string& objName, int createCount);
	void GrowUpBullet(CComponent* obj, const std::string& objName, int createCount);
	void Explosion(CComponent* obj, const std::string& objName, int createCount);
	void MeteorBreak(CComponent* obj, const std::string& objName, int createCount);
	void RushToBoss(CComponent* obj, const std::string& objName, int createCount);
	void Stella(CComponent* obj, const std::string& objName, int createCount);
	void ShaSusu(CComponent* obj, const std::string& objName, int createCount);
	void Lazeret(CComponent* obj, const std::string& objName, int createCount);
};

