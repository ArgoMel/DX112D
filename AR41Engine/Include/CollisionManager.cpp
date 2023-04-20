#include "CollisionManager.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"
DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	{
		auto	iter = m_mapProfile.begin();
		auto	iterEnd = m_mapProfile.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}
	{
		auto	iter = m_vecChannel.begin();
		auto	iterEnd = m_vecChannel.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
	}
}

bool CCollisionManager::Init()
{
	CreateChannel("Default", ECollision_Interaction::Collision);
	CreateChannel("Mouse", ECollision_Interaction::Collision);
	CreateChannel("Player", ECollision_Interaction::Collision);
	CreateChannel("PlayerAttack", ECollision_Interaction::Collision);
	CreateChannel("Monster", ECollision_Interaction::Collision);
	CreateChannel("MonsterAttack", ECollision_Interaction::Collision);
	CreateProfile("Default", "Default", true);
	CreateProfile("Mouse", "Mouse", true);
	CreateProfile("Player", "Player", true);
	CreateProfile("PlayerAttack", "PlayerAttack", true);
	CreateProfile("Monster", "Monster", true);
	CreateProfile("MonsterAttack", "MonsterAttack", true);

	SetCollisionInteraction("Player", "PlayerAttack", ECollision_Interaction::Ignore);
	SetCollisionInteraction("Player", "Player", ECollision_Interaction::Ignore);
	SetCollisionInteraction("PlayerAttack", "Player", ECollision_Interaction::Ignore);
	SetCollisionInteraction("PlayerAttack", "MonsterAttack", ECollision_Interaction::Ignore);
	SetCollisionInteraction("PlayerAttack", "PlayerAttack", ECollision_Interaction::Ignore);
	SetCollisionInteraction("Monster", "MonsterAttack", ECollision_Interaction::Ignore);
	SetCollisionInteraction("Monster", "Monster", ECollision_Interaction::Ignore);
	SetCollisionInteraction("MonsterAttack", "Monster", ECollision_Interaction::Ignore);
	SetCollisionInteraction("MonsterAttack", "MonsterAttack", ECollision_Interaction::Ignore);
	SetCollisionInteraction("MonsterAttack", "PlayerAttack", ECollision_Interaction::Ignore);
	return true;
}

bool CCollisionManager::CreateProfile(const std::string& name, const std::string& channelName, bool enable
	, ECollision_Interaction baseInteraction)
{
	CollisionProfile* profile = FindProfile(name);
	if (profile)
	{
		return false;
	}
	CollisionChannel* channel = nullptr;
	size_t	count = m_vecChannel.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_vecChannel[i]->name == channelName)
		{
			channel = m_vecChannel[i];
			break;
		}
	}
	if (!channel)
	{
		return false;
	}
	profile = new CollisionProfile;
	profile->name = name;
	profile->channel = channel;
	profile->enable = enable;
	profile->vecCollisionInteraction.resize(m_vecChannel.size());
	count = profile->vecCollisionInteraction.size();
	for (size_t i = 0; i < count; ++i)
	{
		profile->vecCollisionInteraction[i] = baseInteraction;
	}
	m_mapProfile.insert(std::make_pair(name, profile));
	return true;
}

bool CCollisionManager::SetCollisionInteraction(const std::string& name, const std::string& channelName
	, ECollision_Interaction interaction)
{
	CollisionProfile* profile = FindProfile(name);
	if (!profile)
	{
		return false;
	}
	CollisionChannel* channel = nullptr;
	size_t	count = m_vecChannel.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_vecChannel[i]->name == channelName)
		{
			channel = m_vecChannel[i];
			break;
		}
	}
	if (!channel)
	{
		return false;
	}
	profile->vecCollisionInteraction[(int)channel->channel] = interaction;
	return true;
}

bool CCollisionManager::CheckCollisionInteraction(const std::string& name, const std::string& channelName
	, ECollision_Interaction interaction)
{
	CollisionProfile* profile = FindProfile(name);
	if (!profile)
	{
		return false;
	}
	CollisionChannel* channel = nullptr;
	size_t	count = m_vecChannel.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_vecChannel[i]->name == channelName)
		{
			channel = m_vecChannel[i];
			break;
		}
	}
	if (!channel)
	{
		return false;
	}
	return profile->vecCollisionInteraction[(int)channel->channel] == interaction;
}

bool CCollisionManager::CreateChannel(const std::string& name, ECollision_Interaction interaction)
{
	size_t	count = m_vecChannel.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_vecChannel[i]->name == name)
		{
			return false;
		}
	}
	CollisionChannel* newChannel = new CollisionChannel;
	newChannel->name = name;
	newChannel->channel = (ECollision_Channel)m_vecChannel.size();
	newChannel->interaction = interaction;
	m_vecChannel.push_back(newChannel);
	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();
	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecCollisionInteraction.push_back(interaction);
	}
	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& name)
{
	auto	iter = m_mapProfile.find(name);
	if (iter == m_mapProfile.end())
	{
		return nullptr;
	}
	return iter->second;
}

bool CCollisionManager::CollisionBox2DToBox2D(Vector2& hitPoint, CColliderBox2D* src, CColliderBox2D* dest)
{
	if (CollisionBox2DToBox2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionBox2DToOBB2D(Vector2& hitPoint, CColliderBox2D* src, CColliderOBB2D* dest)
{
	if (CollisionBox2DToOBB2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionBox2DToSphere2D(Vector2& hitPoint, CColliderBox2D* src, CColliderSphere2D* dest)
{
	if (CollisionBox2DToSphere2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionBox2DToPixel(Vector2& hitPoint, CColliderBox2D* src, CColliderPixel* dest)
{
	if (CollisionBox2DToPixel(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionSphere2DToOBB2D(Vector2& hitPoint, CColliderSphere2D* src, CColliderOBB2D* dest)
{
	if (CollisionSphere2DToOBB2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionSphere2DToSphere2D(Vector2& hitPoint, CColliderSphere2D* src, CColliderSphere2D* dest)
{
	if (CollisionSphere2DToSphere2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionSphere2DToPixel(Vector2& hitPoint, CColliderSphere2D* src, CColliderPixel* dest)
{
	if (CollisionSphere2DToPixel(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionOBB2DToOBB2D(Vector2& hitPoint, CColliderOBB2D* src, CColliderOBB2D* dest)
{
	if (CollisionOBB2DToOBB2D(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionOBB2DToPixel(Vector2& hitPoint, CColliderOBB2D* src, CColliderPixel* dest)
{
	if (CollisionOBB2DToPixel(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionPixelToPixel(Vector2& hitPoint, CColliderPixel* src, CColliderPixel* dest)
{
	if (CollisionPixelToPixel(hitPoint, src->GetInfo(), dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionPointToBox2D(Vector2& hitPoint, const Vector2& src, CColliderBox2D* dest)
{
	if (CollisionPointToBox2D(hitPoint, src, dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionPointToOBB2D(Vector2& hitPoint, const Vector2& src, CColliderOBB2D* dest)
{
	if (CollisionPointToOBB2D(hitPoint, src, dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionPointToSphere2D(Vector2& hitPoint, const Vector2& src, CColliderSphere2D* dest)
{
	if (CollisionPointToSphere2D(hitPoint, src, dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionPointToPixel(Vector2& hitPoint, const Vector2& src, CColliderPixel* dest)
{
	if (CollisionPointToPixel(hitPoint, src, dest->GetInfo()))
	{
		dest->m_HitPoint = Vector3(hitPoint.x, hitPoint.y, 0.f);
		return true;
	}
	return false;
}

bool CCollisionManager::CollisionBox2DToBox2D(Vector2& hitPoint, const Box2DInfo& src, const Box2DInfo& dest)
{
	if (src.left > dest.right)
	{
		return false;
	}
	else if (src.right < dest.left)
	{
		return false;
	}
	else if (src.bottom > dest.top)
	{
		return false;
	}
	else if (src.top < dest.bottom)
	{
		return false;
	}
	ComputeHitPoint(hitPoint, src, dest);
	return true;
}

bool CCollisionManager::CollisionBox2DToOBB2D(Vector2& hitPoint, const Box2DInfo& src, const OBB2DInfo& dest)
{
	OBB2DInfo	srcInfo;
	srcInfo.center = Vector2((src.left + src.right) / 2.f, (src.top + src.bottom) / 2.f);
	srcInfo.axis[(int)AXIS2D::AXIS2D_X] = Vector2(1.f, 0.f);
	srcInfo.axis[(int)AXIS2D::AXIS2D_Y] = Vector2(0.f, 1.f);
	srcInfo.length[(int)AXIS2D::AXIS2D_X] = (src.right - src.left) / 2.f;
	srcInfo.length[(int)AXIS2D::AXIS2D_Y] = (src.top - src.bottom) / 2.f;
	return CollisionOBB2DToOBB2D(hitPoint, srcInfo, dest);
}

bool CCollisionManager::CollisionBox2DToSphere2D(Vector2& hitPoint, const Box2DInfo& src, const Sphere2DInfo& dest)
{
	if ((src.left <= dest.center.x && dest.center.x <= src.right) || (src.bottom <= dest.center.y && dest.center.y <= src.top))
	{
		Box2DInfo	info = src;
		info.left -= dest.radius;
		info.bottom -= dest.radius;
		info.right += dest.radius;
		info.top += dest.radius;
		if (info.left > dest.center.x)
		{
			return false;
		}
		else if (info.bottom > dest.center.y)
		{
			return false;
		}
		else if (info.right < dest.center.x)
		{
			return false;
		}
		else if (info.top < dest.center.y)
		{
			return false;
		}
		Box2DInfo	overlapBox = ConvertBox2DInfo(dest);
		ComputeHitPoint(hitPoint, src, overlapBox);
		return true;
	}
	Vector2	pos[4] =
	{
		Vector2(src.left, src.top),
		Vector2(src.right, src.top),
		Vector2(src.left, src.bottom),
		Vector2(src.right, src.bottom)
	};
	for (int i = 0; i < 4; ++i)
	{
		float dist = dest.center.Distance(pos[i]);
		if (dist <= dest.radius)
		{
			Box2DInfo	overlapBox = ConvertBox2DInfo(dest);
			ComputeHitPoint(hitPoint, src, overlapBox);
			return true;
		}
	}
	return false;
}

bool CCollisionManager::CollisionBox2DToPixel(Vector2& hitPoint, const Box2DInfo& src, const PixelInfo& dest)
{
	// �ȼ��浹ü�� �����ϴ� �簢���� �浹ó���� ���� �Ѵ�.
	if (!CollisionBox2DToBox2D(hitPoint, src, dest.box2D))
	{
		return false;
	}
	// �� �簢���� �������� ������ ���ϰ� �ش� ������ �ȼ������� Ȯ���Ͽ� �浹�� �Ǿ������� �Ǵ��Ѵ�.
	Box2DInfo overlapInfo = OverlapBox2D(src, dest.box2D);
	overlapInfo.left -= dest.box2D.left;
	overlapInfo.bottom -= dest.box2D.bottom;
	overlapInfo.right -= dest.box2D.left;
	overlapInfo.top -= dest.box2D.bottom;
	float width = (float)dest.width;
	float height = (float)dest.height;
	overlapInfo.left = overlapInfo.left < 0.f ? 0.f : overlapInfo.left;
	overlapInfo.bottom = overlapInfo.bottom < 0.f ? 0.f : overlapInfo.bottom;
	overlapInfo.right = overlapInfo.right > width ? width - 1.f : overlapInfo.right;
	overlapInfo.top = overlapInfo.top > height ? height - 1.f : overlapInfo.top;
	// Bottom�� Top�� ����� �̹����� �����ش�.
	overlapInfo.bottom = height - overlapInfo.bottom;
	overlapInfo.top = height - overlapInfo.top;
	bool	collision = false;
	for (int y = (int)overlapInfo.top; y < (int)overlapInfo.bottom; ++y)
	{
		for (int x = (int)overlapInfo.left; x < (int)overlapInfo.right; ++x)
		{
			int	index = y * (int)dest.width * 4 + x * 4;
			switch (dest.pixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			switch (dest.pixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			if (collision)
			{
				break;
			}
		}
		if (collision)
		{
			break;
		}
	}
	ComputeHitPoint(hitPoint, src, dest.box2D);
	return collision;
}

bool CCollisionManager::CollisionSphere2DToOBB2D(Vector2& hitPoint, const Sphere2DInfo& src, const OBB2DInfo& dest)
{
	Vector2	centerLine = src.center - dest.center;
	float centerProjDist = centerLine.Length();
	Vector2	axis = centerLine;
	axis.Normalize();
	float srcDist = src.radius;
	float destDist= abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_X]) * dest.length[(int)AXIS2D::AXIS2D_X]) +
		abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_Y]) * dest.length[(int)AXIS2D::AXIS2D_Y]);
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	axis = dest.axis[(int)AXIS2D::AXIS2D_X];
	centerProjDist = abs(axis.Dot(centerLine));
	destDist = dest.length[(int)AXIS2D::AXIS2D_X];
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	axis = dest.axis[(int)AXIS2D::AXIS2D_Y];
	centerProjDist = abs(axis.Dot(centerLine));
	destDist = dest.length[(int)AXIS2D::AXIS2D_Y];
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	Box2DInfo	srcInfo = ConvertBox2DInfo(src);
	Box2DInfo	destInfo=ConvertBox2DInfo(dest);
	ComputeHitPoint(hitPoint, srcInfo, destInfo);
	return true;
}

bool CCollisionManager::CollisionSphere2DToSphere2D(Vector2& hitPoint, const Sphere2DInfo& src, const Sphere2DInfo& dest)
{
	float	dist = src.center.Distance(dest.center);
	bool result = dist <= src.radius + dest.radius;
	hitPoint = (src.center + dest.center) / 2.f;
	return result;
}

bool CCollisionManager::CollisionSphere2DToPixel(Vector2& hitPoint, const Sphere2DInfo& src, const PixelInfo& dest)
{
	if (!CollisionBox2DToSphere2D(hitPoint, dest.box2D, src))
	{
		return false;
	}
	Box2DInfo overlapInfo = OverlapBox2D(dest.box2D, src);
	overlapInfo.left -= dest.box2D.left;
	overlapInfo.bottom -= dest.box2D.bottom;
	overlapInfo.right -= dest.box2D.left;
	overlapInfo.top -=dest.box2D.bottom;
	float width = (float)dest.width;
	float height = (float)dest.height;
	overlapInfo.left = overlapInfo.left < 0.f ? 0.f : overlapInfo.left;
	overlapInfo.bottom = overlapInfo.bottom < 0.f ? 0.f : overlapInfo.bottom;
	overlapInfo.right = overlapInfo.right > width ? width - 1.f : overlapInfo.right;
	overlapInfo.top = overlapInfo.top > height ? height - 1.f : overlapInfo.top;
	overlapInfo.bottom = height - overlapInfo.bottom;
	overlapInfo.top = height - overlapInfo.top;
	bool	collision = false;
	for (int y = (int)overlapInfo.top; y < (int)overlapInfo.bottom; ++y)
	{
		for (int x = (int)overlapInfo.left; x < (int)overlapInfo.right; ++x)
		{
			int	index = y * (int)dest.width * 4 + x * 4;
			Vector2	pixelWorldPos = Vector2(dest.box2D.left, dest.box2D.bottom) + Vector2((float)x, (float)height - (float)y);
			if (!CollisionPointToSphere2D(hitPoint, pixelWorldPos, src))
			{
				continue;
			}
			switch (dest.pixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			switch (dest.pixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			if (collision)
			{
				break;
			}
		}
		if (collision)
		{
			break;
		}
	}
	//ComputeHitPoint(hitPoint, overlapInfo, dest.box2D);	//��������
	return collision;
}

bool CCollisionManager::CollisionOBB2DToOBB2D(Vector2& hitPoint, const OBB2DInfo& src, const OBB2DInfo& dest)
{
	Vector2	centerLine = src.center - dest.center;
	Vector2	axis = src.axis[(int)AXIS2D::AXIS2D_X];
	float centerProjDist = abs(centerLine.Dot(axis));
	float srcDist = src.length[(int)AXIS2D::AXIS2D_X];
	float destDist = abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_X]) * dest.length[(int)AXIS2D::AXIS2D_X]) +
		abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_Y]) * dest.length[(int)AXIS2D::AXIS2D_Y]);
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	axis = src.axis[(int)AXIS2D::AXIS2D_Y];
	centerProjDist = abs(centerLine.Dot(axis));
	srcDist = src.length[(int)AXIS2D::AXIS2D_Y];
	destDist = abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_X]) * dest.length[(int)AXIS2D::AXIS2D_X]) +
		abs(axis.Dot(dest.axis[(int)AXIS2D::AXIS2D_Y]) * dest.length[(int)AXIS2D::AXIS2D_Y]);
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	axis = dest.axis[(int)AXIS2D::AXIS2D_X];
	centerProjDist = abs(centerLine.Dot(axis));
	srcDist = abs(axis.Dot(src.axis[(int)AXIS2D::AXIS2D_X]) * src.length[(int)AXIS2D::AXIS2D_X]) +
		abs(axis.Dot(src.axis[(int)AXIS2D::AXIS2D_Y]) * src.length[(int)AXIS2D::AXIS2D_Y]);
	destDist = dest.length[(int)AXIS2D::AXIS2D_X];
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	axis = dest.axis[(int)AXIS2D::AXIS2D_Y];
	centerProjDist = abs(centerLine.Dot(axis));
	srcDist = abs(axis.Dot(src.axis[(int)AXIS2D::AXIS2D_X]) * src.length[(int)AXIS2D::AXIS2D_X]) +
		abs(axis.Dot(src.axis[(int)AXIS2D::AXIS2D_Y]) * src.length[(int)AXIS2D::AXIS2D_Y]);
	destDist = dest.length[(int)AXIS2D::AXIS2D_Y];
	if (centerProjDist > srcDist + destDist)
	{
		return false;
	}
	Box2DInfo	srcInfo = ConvertBox2DInfo(src);
	Box2DInfo	destInfo = ConvertBox2DInfo(dest);
	ComputeHitPoint(hitPoint, srcInfo, destInfo);
	return true;
}

bool CCollisionManager::CollisionOBB2DToPixel(Vector2& hitPoint, const OBB2DInfo& src, const PixelInfo& dest)
{
	if (!CollisionBox2DToOBB2D(hitPoint, dest.box2D, src))
	{
		return false;
	}
	Box2DInfo overlapInfo = OverlapBox2D(dest.box2D, src);
	overlapInfo.left -= dest.box2D.left;
	overlapInfo.bottom -= dest.box2D.bottom;
	overlapInfo.right -= dest.box2D.left;
	overlapInfo.top -= dest.box2D.bottom;
	float width = (float)dest.width;
	float height = (float)dest.height;
	overlapInfo.left = overlapInfo.left < 0.f ? 0.f : overlapInfo.left;
	overlapInfo.bottom = overlapInfo.bottom < 0.f ? 0.f : overlapInfo.bottom;
	overlapInfo.right = overlapInfo.right > width ? width - 1.f : overlapInfo.right;
	overlapInfo.top = overlapInfo.top > height ? height - 1.f : overlapInfo.top;
	overlapInfo.bottom = height - overlapInfo.bottom;
	overlapInfo.top = height - overlapInfo.top;
	bool	collision = false;
	for (int y = (int)overlapInfo.top; y < (int)overlapInfo.bottom; ++y)
	{
		for (int x = (int)overlapInfo.left; x < (int)overlapInfo.right; ++x)
		{
			int	index = y * (int)dest.width * 4 + x * 4;
			Vector2	pixelWorldPos = Vector2(dest.box2D.left, dest.box2D.bottom) + Vector2((float)x, (float)height - (float)y);
			if (!CollisionPointToOBB2D(hitPoint, pixelWorldPos, src))
			{
				continue;
			}
			switch (dest.pixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (dest.pixel[index] == dest.typeColor[0] &&
					dest.pixel[index + 1] == dest.typeColor[1] &&
					dest.pixel[index + 2] == dest.typeColor[2])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			switch (dest.pixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					continue;
				}
				collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (dest.pixel[index + 3] == dest.typeColor[3])
				{
					collision = true;
				}
				else
				{
					continue;
				}
				break;
			}
			if (collision)
			{
				break;
			}
		}
		if (collision)
		{
			break;
		}
	}
	ComputeHitPoint(hitPoint, overlapInfo, dest.box2D);
	return collision;
}

bool CCollisionManager::CollisionPixelToPixel(Vector2& hitPoint, const PixelInfo& src, const PixelInfo& dest)
{
	return false;
}

bool CCollisionManager::CollisionPointToBox2D(Vector2& hitPoint, const Vector2& src, const Box2DInfo& dest)
{
	if (src.x < dest.left)
	{
		return false;
	}
	else if (src.x > dest.right)
	{
		return false;
	}
	else if (src.y < dest.bottom)
	{
		return false;
	}
	else if (src.y > dest.top)
	{
		return false;
	}
	hitPoint = src;
	return true;
}

bool CCollisionManager::CollisionPointToOBB2D(Vector2& hitPoint, const Vector2& src, const OBB2DInfo& dest)
{
	Vector2	centerLine = src - dest.center;
	Vector2	axis = dest.axis[(int)AXIS2D::AXIS2D_X];
	float centerProjDist = abs(axis.Dot(centerLine));
	if (centerProjDist > dest.length[(int)AXIS2D::AXIS2D_X])
	{
		return false;
	}
	axis = dest.axis[(int)AXIS2D::AXIS2D_Y];
	centerProjDist = abs(axis.Dot(centerLine));
	if (centerProjDist > dest.length[(int)AXIS2D::AXIS2D_Y])
	{
		return false;
	}
	hitPoint = src;
	return true;
}

bool CCollisionManager::CollisionPointToSphere2D(Vector2& hitPoint, const Vector2& src, const Sphere2DInfo& dest)
{
	bool result = src.Distance(dest.center) <= dest.radius;
	if (result)
	{
		hitPoint = src;
	}
	return result;
}

bool CCollisionManager::CollisionPointToPixel(Vector2& hitPoint, const Vector2& src, const PixelInfo& dest)
{
	if (!CollisionPointToBox2D(hitPoint, src, dest.box2D))
	{
		return false;
	}
	Vector2	convertSrc = src;
	convertSrc.x -= dest.box2D.left;
	convertSrc.y -= dest.box2D.bottom;
	convertSrc.y = dest.height - convertSrc.y;
	bool	collision = false;
	int	index = (int)convertSrc.y * (int)dest.width * 4 + (int)convertSrc.x * 4;
	switch (dest.pixelColorCollisionType)
	{
	case EPixelCollision_Type::Color_Ignore:
		if (dest.pixel[index] ==dest.typeColor[0] &&
			dest.pixel[index + 1] == dest.typeColor[1] &&
			dest.pixel[index + 2] == dest.typeColor[2])
		{
			collision = false;
		}
		else
		{
			collision = true;
		}
		break;
	case EPixelCollision_Type::Color_Confirm:
		if (dest.pixel[index] == dest.typeColor[0] &&
			dest.pixel[index + 1] == dest.typeColor[1] &&
			dest.pixel[index + 2] == dest.typeColor[2])
		{
			collision = true;
		}
		else
		{
			collision = false;
		}
		break;
	}
	switch (dest.pixelAlphaCollisionType)
	{
	case EPixelCollision_Type::Alpha_Ignore:
		if (dest.pixel[index + 3] == dest.typeColor[3])
		{
			collision = false;
		}
		else
		{
			collision = true;
		}
		break;
	case EPixelCollision_Type::Alpha_Confirm:
		if (dest.pixel[index + 3] == dest.typeColor[3])
		{
			collision = true;
		}
		else
		{
			collision = false;
		}
		break;
	}
	hitPoint = src;
	return collision;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const Sphere2DInfo& info)
{
	Box2DInfo	result;
	result.left = info.center.x - info.radius;
	result.bottom = info.center.y - info.radius;
	result.right = info.center.x + info.radius;
	result.top = info.center.y + info.radius;
	return result;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const OBB2DInfo& info)
{
	Box2DInfo	result;
	Vector2	pos[4];
	pos[0] = info.center - info.axis[(int)AXIS2D::AXIS2D_X] * info.length[(int)AXIS2D::AXIS2D_X] 
		+ info.axis[(int)AXIS2D::AXIS2D_Y] * info.length[(int)AXIS2D::AXIS2D_Y];
	pos[1] = info.center + info.axis[(int)AXIS2D::AXIS2D_X] * info.length[(int)AXIS2D::AXIS2D_X] 
		+ info.axis[(int)AXIS2D::AXIS2D_Y] * info.length[(int)AXIS2D::AXIS2D_Y];
	pos[2] = info.center - info.axis[(int)AXIS2D::AXIS2D_X] * info.length[(int)AXIS2D::AXIS2D_X] 
		- info.axis[(int)AXIS2D::AXIS2D_Y] * info.length[(int)AXIS2D::AXIS2D_Y];
	pos[3] = info.center + info.axis[(int)AXIS2D::AXIS2D_X] * info.length[(int)AXIS2D::AXIS2D_X]
		- info.axis[(int)AXIS2D::AXIS2D_Y] * info.length[(int)AXIS2D::AXIS2D_Y];
	result.left = pos[0].x;
	result.bottom = pos[0].y;
	result.right = pos[0].x;
	result.top = pos[0].y;
	for (int i = 1; i < 4; ++i)
	{
		result.left = result.left > pos[i].x ? pos[i].x : result.left;
		result.bottom = result.bottom > pos[i].y ? pos[i].y : result.bottom;
		result.right = result.right < pos[i].x ? pos[i].x : result.right;
		result.top = result.top < pos[i].y ? pos[i].y : result.top;
	}
	return result;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& src, const Box2DInfo& dest)
{
	Box2DInfo	info;
	info.left = src.left > dest.left ? src.left : dest.left;
	info.bottom = src.bottom > dest.bottom ? src.bottom : dest.bottom;
	info.right = src.right < dest.right ? src.right : dest.right;
	info.top = src.top < dest.top ? src.top : dest.top;
	return info;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& src, const Sphere2DInfo& dest)
{
	Box2DInfo	info;
	Box2DInfo	destInfo;
	destInfo = ConvertBox2DInfo(dest);
	info.left = src.left > destInfo.left ? src.left : destInfo.left;
	info.bottom = src.bottom > destInfo.bottom ? src.bottom : destInfo.bottom;
	info.right = src.right < destInfo.right ? src.right : destInfo.right;
	info.top = src.top < destInfo.top ? src.top : destInfo.top;
	return info;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& src, const OBB2DInfo& dest)
{
	Box2DInfo	info;
	Box2DInfo	destInfo;
	destInfo = ConvertBox2DInfo(dest);
	info.left = src.left > destInfo.left ? src.left : destInfo.left;
	info.bottom = src.bottom > destInfo.bottom ? src.bottom : destInfo.bottom;
	info.right = src.right < destInfo.right ? src.right : destInfo.right;
	info.top = src.top < destInfo.top ? src.top : destInfo.top;
	return info;
}

void CCollisionManager::ComputeHitPoint(Vector2& hitPoint, const Box2DInfo& src, const Box2DInfo& dest)
{
	float left = src.left > dest.left ? src.left : dest.left;
	float bottom = src.bottom > dest.bottom ? src.bottom : dest.bottom;
	float right = src.right < dest.right ? src.right : dest.right;
	float top = src.top < dest.top ? src.top : dest.top;
	hitPoint.x = (left + right) / 2.f;
	hitPoint.y = (top + bottom) / 2.f;
}