#pragma once
#include "SceneComponent.h"

class CCameraComponent : public CSceneComponent
{
protected:
	friend class CCameraManager;
	friend class CGameObject;
	friend class CScene;
	ECameraType	m_CameraType;
	Matrix		m_matView;
	Matrix		m_matProj;
	float		m_CameraViewDistance;
	CCameraComponent();
	CCameraComponent(const CCameraComponent& component);
	virtual ~CCameraComponent();
public:
	const Matrix& GetViewMatrix()	const
	{
		return m_matView;
	}
	const Matrix& GetProjMatrix()	const
	{
		return m_matProj;
	}
	ECameraType GetCameraType()	const
	{
		return m_CameraType;
	}
	float GetCameraViewDistance()	const
	{
		return m_CameraViewDistance;
	}
	void SetCameraType(ECameraType type)
	{
		m_CameraType = type;
		ComputeProjectionMatrix();
	}
	void SetCameraViewDistance(float distance)
	{
		m_CameraViewDistance = distance;
		ComputeProjectionMatrix();
	}
	void ComputeProjectionMatrix();
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CCameraComponent* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

