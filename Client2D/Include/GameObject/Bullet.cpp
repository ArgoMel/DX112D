#include "Bullet.h"
#include "Player2D.h"
#include "PathManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Component/RigidBody.h"
#include "Resource/Material/Material.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../Skill/SkillManager.h"

CBullet::CBullet()
	: m_BulletType(EBulletType::Straight)
	, m_SPatternDelay(0.f)
	, m_SDamageDelay(0.f)
	, m_SHitCount(0)
{
	SetTypeID<CBullet>();
	m_ObjectTypeName = "Bullet";
}

CBullet::CBullet(const CBullet& obj)
	: CGameObject(obj)
	, m_BulletType(obj.m_BulletType)
	, m_SPatternDelay(0.f)
	, m_SDamageDelay(0.f)
	, m_SHitCount(0)
{
	m_Body = (CColliderSphere2D*)FindComponent("Body");
	m_Sprite = (CSpriteComponent*)FindComponent(m_BulletName);
}

CBullet::~CBullet()
{
}

void CBullet::Destroy()
{
	CGameObject::Destroy();
}

void CBullet::Start()
{
	CGameObject::Start();
	m_Body->SetCollisionCallback<CBullet>(ECollision_Result::Collision, this, &CBullet::CollisionBullet);

	m_Sprite->SetName(m_BulletName);
	std::string imgPath = "Character\\" + m_BulletName + "\\pixelart\\sprite_sheet\\000a.png";
	TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
	t_filename[imgPath.size()] = 0;
	std::copy(imgPath.begin(), imgPath.end(), t_filename);
	m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_BulletName, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	float width= (float)m_Sprite->GetMaterial(0)->GetTexture()->GetWidth() * 2.5f;
	float height= (float)m_Sprite->GetMaterial(0)->GetTexture()->GetHeight() * 2.5f;
	float radius = width > height ? width : height;
	m_Body->SetRadius(radius * 0.5f);
	m_Sprite->SetRelativeScale(width, height);
	if (!Load(SAVE_PATH))
	{
		return;
	}
	if(!m_IsAnim)
	{
		return;
	}
	m_Anim = m_Sprite->SetAnimation<CAnimation2D>("BulletAnim");
	m_Anim->AddAnimation("Die", "BulletDie");
	std::string animeName = m_BulletName + "_die";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->SetCurrentEndFunction("Die", this, &CBullet::Destroy);
	m_Anim->AddAnimation("Idle", "BulletIdle", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Idle");
	animeName = m_BulletName + "_idle";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
}

bool CBullet::Init()
{
	CGameObject::Init();
	m_BulletName = GetName();

	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>(m_BulletName);

	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionProfile("MonsterAttack");
	m_Body->SetWorldPosition(960.f,-50.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(40.f, 40.f);
	m_Sprite->SetInheritRotZ(true);
	return true;
}

void CBullet::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	if(m_BulletType==EBulletType::Straight)
	{
		AddWorldPosition(GetWorldAxis(AXIS::AXIS_Y) * m_ObjectData.speed * deltaTime);
	}
	else if (m_BulletType == EBulletType::Rotation)
	{
		SetWorldPosition(m_Scene->GetSceneInfo()->GetPlayerObject()->GetWorldPos());
		AddWorldRotationZ(m_ObjectData.speed * deltaTime);
	}
	else if (m_BulletType == EBulletType::GuidedBullet)
	{
		Vector2 playerPos=m_Scene->GetSceneInfo()->GetPlayerObject()->GetWorldPos();
		Vector2 pos= GetWorldPos();
		float angle = pos.Angle(playerPos);
		SetWorldRotationZ(angle-90.f);
		AddWorldPositionX(cosf(DegreeToRadian(angle))*m_ObjectData.speed * deltaTime);
		AddWorldPositionY(sinf(DegreeToRadian(angle))*m_ObjectData.speed * deltaTime);
	}
	else if (m_BulletType == EBulletType::GrowUp)
	{
		Vector2 scale= m_Sprite->GetWorldScale();
		m_Body->AddRadius(m_ObjectData.speed * deltaTime*0.5f);
		m_Sprite->SetWorldScale(scale+ m_ObjectData.speed * deltaTime);
	}
	m_SPatternDelay += deltaTime;
	if (m_SPatternDelay > m_PatternDelay)
	{
		CSkillManager::GetInst()->ActivateSkill(m_BulletName, m_Body, m_SkillName, m_SummonCount);
		m_SPatternDelay = 0;
	}
}

void CBullet::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
	Move(m_MoveType, deltaTime);
	m_SDamageDelay += deltaTime;
	if(m_SDamageDelay >= 0.1f&&m_ColisionObj&& m_HitCount> m_SHitCount)
	{
		m_SDamageDelay = 0.f;
		++m_SHitCount;
		m_ColisionObj->InflictDamage((float)m_ObjectData.attack);
	}
}

CBullet* CBullet::Clone() const
{
	return new CBullet(*this);
}

void CBullet::Save(FILE* file)
{
	CGameObject::Save(file);
	int	length = (int)m_BulletName.length();
	fwrite(&length, 4, 1, file);
	fwrite(m_BulletName.c_str(), 1, length, file);
}

void CBullet::Load(FILE* file)
{
	CGameObject::Load(file);
	int	length = 0;
	char	typeName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(typeName, 1, length, file);
	m_BulletName = typeName;
}

void CBullet::Move(EMoveType moveType, float deltaTime)
{
	if (moveType == EMoveType::None)
	{
		return;
	}
	else if (moveType == EMoveType::RushToBoss)
	{
		Vector2 pos = GetWorldPos();
		Vector2 bossPos = m_Scene->GetSceneInfo()->GetBossObject()->GetWorldPos();
		if (pos.Distance(bossPos) < 200.f)
		{
			return;
		}
		float angle = pos.Angle(bossPos);
		SetWorldRotationZ(angle + 90.f);
		AddWorldPositionX(cosf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
		AddWorldPositionY(sinf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
	}
}

bool CBullet::Save(const std::string& pathName)
{	
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_BulletName.c_str());
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	fprintf(file, "[name]\n");
	fprintf(file, m_OutputName.c_str());
	fprintf(file, "\n");
	fprintf(file, "[attck]\n");
	fprintf(file, "%d", m_ObjectData.attack);
	fprintf(file, "[defense]\n");
	fprintf(file, "%d", m_ObjectData.defense);
	fprintf(file, "[element]\n");
	fprintf(file, "%d\n", m_ObjectData.element);
	fclose(file);
	return true;
}

bool CBullet::Load(const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_BulletName.c_str());
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	char szbuff[256] = {};
	int temp;
	float f_Temp;
	std::string s_Temp;
	Vector2 v_Temp;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_OutputName = szbuff;
	m_OutputName = m_OutputName.substr(0, m_OutputName.length() - 1);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_SkillName = szbuff;
	m_SkillName = m_SkillName.substr(0, m_SkillName.length() - 1);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.attack);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_ObjectData.speed);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.element);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_SummonCount);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_PatternDelay);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &temp);
	m_IsAnim = temp;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &temp);
	m_IsLoop = temp;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_BulletType);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_LifeTime);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	s_Temp = szbuff;
	if (s_Temp == "[no_option]\r")
	{
		return true;
	}
	fscanf_s(file, "%f", &f_Temp);
	m_Body->SetRadius(f_Temp);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Body->SetWorldPositionX(f_Temp);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Body->SetWorldPositionY(f_Temp);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Sprite->SetRelativeScaleX(f_Temp);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Sprite->SetRelativeScaleY(f_Temp);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Sprite->SetRelativePositionX(f_Temp);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &f_Temp);
	m_Sprite->SetRelativePositionY(f_Temp);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &v_Temp.x);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &v_Temp.y);
	m_Sprite->SetPivot(v_Temp);
	fclose(file);
	return true;
}

void CBullet::CollisionBullet(const CollisionResult& result)
{
	std::string bulletname = result.src->GetCollisionProfile()->name;
	std::string name = result.dest->GetCollisionProfile()->name;
	if(bulletname=="PlayerAttack")
	{
		if (name == "Monster")
		{
			m_ColisionObj = result.dest->GetOwner();
			result.dest->GetOwner()->InflictDamage((float)m_ObjectData.attack);
			++m_SHitCount;
			if (m_Anim)
			{
				m_Anim->ChangeAnimation("Die");
			}
			else
			{
				if (m_BulletType == EBulletType::Straight)
				{
					Destroy();
				}
			}
		}
	}
	if (bulletname == "MonsterAttack")
	{
		if (name == "Player")
		{
			int index= stoi(result.dest->GetName().substr(4));			//Body1
			CPlayer2D* player = (CPlayer2D*)m_Scene->GetSceneInfo()->GetPlayerObject();
			if (m_BulletType == EBulletType::Straight|| m_BulletType == EBulletType::GuidedBullet)
			{
				player->InflictDamage((float)m_ObjectData.attack, index);
				if (GetName() == "push_LR")
				{
					float angle = GetWorldRot().z + 90.f;
					Vector2	dir;
					dir.x = cosf(DegreeToRadian(angle)) * m_ObjectData.speed * 0.5f;
					dir.y = sinf(DegreeToRadian(angle)) * m_ObjectData.speed * 0.5f;
					player->GetRigidBody()->AddForce(dir.x * 2.f, dir.y * 2.f);
					player->GetRigidBody()->SetVelocity(dir.x, dir.y);
				}
				if (m_Anim)
				{
					m_Anim->ChangeAnimation("Die");
					return;
				}
				Destroy();
			}
			else if (m_BulletType == EBulletType::Spike)
			{
				player->InflictDamage((float)player->GetPlayerData(index).maxHp * 0.05f, index);
			}
			else if (m_BulletType == EBulletType::Laser)
			{
				player->InflictDamage((float)m_ObjectData.attack, index);
			}
			else if (m_BulletType == EBulletType::GrowUp)
			{
				player->InflictDamage((float)m_ObjectData.attack, index);
				m_Body->SetCollisionProfile("Invisible");
			}
		}
		else if(name == "Default")
		{
			if (GetName() == "push_LR")
			{
				return;
			}
			if (m_Anim)
			{
				m_Anim->ChangeAnimation("Die");
			}
			else
			{
				if (m_BulletType == EBulletType::Straight)
				{
					Destroy();
				}
			}
		}
	}
}
