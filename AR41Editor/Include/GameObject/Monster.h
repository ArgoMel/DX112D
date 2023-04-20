#pragma once
#include "GameObject/GameObject.h"

class CMonster : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;
	std::string m_MonsterName;
	std::string m_SkillName;
	EMonsterType	m_MonsterType;
	int m_SummonCount;
	float m_PatternDelay;
	float	m_SPatternDelay;
	bool m_IsLoop;
protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CMonster* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual int InflictDamage(float damage);
	virtual bool Save(const std::string& pathName = SAVE_PATH);
	virtual bool Load(const std::string& pathName = SAVE_PATH);
	void PatternEnd();
};

