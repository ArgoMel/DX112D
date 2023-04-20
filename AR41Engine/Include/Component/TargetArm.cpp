#include "TargetArm.h"

CTargetArm::CTargetArm()
	: m_TargetDistance(0.f)
	, m_TargetDistanceAxis(AXIS::AXIS_MAX)
{
	SetTypeID<CTargetArm>();
	m_ComponentTypeName = "TargetArm";
}

CTargetArm::CTargetArm(const CTargetArm& component)
	: CSceneComponent(component)
{
	m_TargetDistance = component.m_TargetDistance;
	m_TargetOffset = component.m_TargetOffset;
	m_TargetDistanceAxis = component.m_TargetDistanceAxis;
}

CTargetArm::~CTargetArm()
{
}

void CTargetArm::Destroy()
{
	CSceneComponent::Destroy();
}

void CTargetArm::Start()
{
	CSceneComponent::Start();
}

bool CTargetArm::Init()
{
	CSceneComponent::Init();
	return true;
}

void CTargetArm::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
	if (m_Parent)
	{
		Vector3	parentPos = m_Parent->GetWorldPos();
		Vector3 pos = parentPos - GetWorldAxis(m_TargetDistanceAxis) * m_TargetDistance;
		SetWorldPosition(pos + m_TargetOffset);
	}
}

void CTargetArm::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CTargetArm::Render()
{
	CSceneComponent::Render();
}

CTargetArm* CTargetArm::Clone() const
{
	return new CTargetArm(*this);
}

void CTargetArm::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_TargetOffset, sizeof(Vector3), 1, file);
	fwrite(&m_TargetDistance, sizeof(float), 1, file);
	fwrite(&m_TargetDistanceAxis, sizeof(AXIS), 1, file);
}

void CTargetArm::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_TargetOffset, sizeof(Vector3), 1, file);
	fread(&m_TargetDistance, sizeof(float), 1, file);
	fread(&m_TargetDistanceAxis, sizeof(AXIS), 1, file);
}
