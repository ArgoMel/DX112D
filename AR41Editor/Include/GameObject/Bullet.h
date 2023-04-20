#pragma once
#include "GameObject/GameObject.h"

class CBullet : public CGameObject
{
private:
	friend class CScene;
	friend class CSkill;
	CSharedPtr<class CSpriteComponent>	m_Sprite;
	CSharedPtr<class CColliderSphere2D>	m_Body;
	CSharedPtr<class CGameObject>	m_ColisionObj;
	std::string m_BulletName;
	std::string m_SkillName;
	float	m_PatternDelay;
	int		m_SummonCount;
	int		m_HitCount;
	EBulletType	m_BulletType;
	bool	m_IsLoop;
	bool	m_IsAnim;
	float	m_SPatternDelay;
	float	m_SDamageDelay;
	int		m_SHitCount;
protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet();
	void CollisionBullet(const CollisionResult& result);
public:
	CSpriteComponent* GetSprite()	const
	{
		return m_Sprite;
	}
	CColliderSphere2D* GetBody()	const
	{
		return m_Body;
	}
	EBulletType GetBulletType()    const
	{
		return m_BulletType;
	}
	void SetBulletType(EBulletType bulletType)
	{
		m_BulletType = bulletType;
	}
	void SetHitCount(int count)
	{
		m_HitCount = count;
	}
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBullet* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual void Move(EMoveType moveType, float deltaTime);
	virtual bool Save(const std::string& pathName = SAVE_PATH);
	virtual bool Load(const std::string& pathName = SAVE_PATH);
};

