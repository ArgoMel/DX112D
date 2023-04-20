#include "CollisionSection3D.h"
#include "../Component/Collider3D.h"

CCollisionSection3D::CCollisionSection3D()
	: m_IndexX(0)
	, m_IndexY(0)
	, m_IndexZ(0)
	, m_Index(0)
{
}

CCollisionSection3D::~CCollisionSection3D()
{
}

void CCollisionSection3D::AddCollider(CCollider3D* collider)
{
	m_ColliderList.push_back(collider);
	collider->AddSectionIndex(m_Index);
}

void CCollisionSection3D::Collision(float deltaTime)
{
}
