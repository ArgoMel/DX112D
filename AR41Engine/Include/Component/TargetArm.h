#pragma once
#include "SceneComponent.h"
class CTargetArm : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	Vector3		m_TargetOffset;
	AXIS		m_TargetDistanceAxis;
	float		m_TargetDistance;
	CTargetArm();
	CTargetArm(const CTargetArm& component);
	virtual ~CTargetArm();
public:
	Vector3 GetTargetOffset()	const
	{
		return m_TargetOffset;
	}
	AXIS GetTargetDistanceAxis()	const
	{
		return m_TargetDistanceAxis;
	}
	float GetTargetDistance()	const
	{
		return m_TargetDistance;
	}
	void SetTargetOffset(const Vector3& offset)
	{
		m_TargetOffset = offset;
	}
	void SetTargetOffset(float x, float y, float z)
	{
		m_TargetOffset = Vector3(x, y, z);
	}
	void SetTargetDistanceAxis(AXIS axis)
	{
		m_TargetDistanceAxis = axis;
	}
	void SetTargetDistance(float distance)
	{
		m_TargetDistance = distance;
	}
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CTargetArm* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

