#include "Monster.h"
#include "Boss.h"
#include "PathManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/Material/Material.h"
#include "../Skill/SkillManager.h"

CMonster::CMonster()
	: m_SummonCount(0)
	, m_PatternDelay(0.f)
	, m_SPatternDelay(0.f)
	, m_IsLoop(false)
{
	SetTypeID<CMonster>();
	m_ObjectTypeName = "Monster";
}

CMonster::CMonster(const CMonster& obj)
	: CGameObject(obj)
{
	m_Body = (CColliderSphere2D*)FindComponent("Body");
	m_Sprite = (CSpriteComponent*)FindComponent(m_MonsterName);
}

CMonster::~CMonster()
{
}

void CMonster::Destroy()
{
	CGameObject::Destroy();
	CBoss* boss = (CBoss*)m_Scene->GetSceneInfo()->GetBossObject();
	if(boss)
	{
		boss->AddSMaxSummonCount(-1);
	}
	if(m_MonsterType == EMonsterType::Normal)
	{
		m_Scene->GetResource()->SoundPlay("MonsterDie");
	}
}

void CMonster::Start()
{
	CGameObject::Start();
	m_Sprite->SetName(m_MonsterName);
	std::string imgPath = "Character\\" + m_MonsterName + "\\pixelart\\sprite_sheet\\pixelart0002.png";
	TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
	t_filename[imgPath.size()] = 0;
	std::copy(imgPath.begin(), imgPath.end(), t_filename);
	m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_MonsterName, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	if (!Load(SAVE_PATH))
	{
		return;
	}
//로드성공후 이미지가 기본이미지인 경우 애니메이션을 만들지 않는다(이미지로드 실패이므로)
	if (m_Sprite->GetTexture()->GetName() == "alk")
	{
		return;
	}

	std::string animeName = m_MonsterName + "_die";
	int animNum = -1;
//애니메이션이 인게임에서 이미 만들어진게 있는지 체크
	do
	{
		++animNum;
		if(animNum>9)
		{
			animNum = 0;
		}
		m_Anim = m_Sprite->SetAnimation<CAnimation2D>("MonsterAnim" + std::to_string(animNum));
		m_Anim->AddAnimation("Die", "MonsterDie" + std::to_string(animNum));
		std::string texName = m_Anim->GetCurrentAnimationSequence()->GetTexture()->GetName();
		if (texName == animeName|| texName=="Temp")
		{
			break;
		}
	} while (true);
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->SetCurrentEndFunction("Die", this, &CMonster::Destroy);
	m_Anim->AddAnimation("Skill", "MonsterSkill" + std::to_string(animNum), 1.f, 1.f, m_IsLoop);
	m_Anim->SetCurrentAnimation("Skill");
	animeName = m_MonsterName + "_skill";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	if (!m_IsLoop)
	{
		m_Anim->SetCurrentEndFunction("Skill", this, &CMonster::PatternEnd);
	}
	m_Anim->AddAnimation("Idle", "MonsterIdle" + std::to_string(animNum), 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Idle");
	animeName = m_MonsterName + "_idle";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
}

bool CMonster::Init()
{
	CGameObject::Init();
	m_MonsterName = GetName();

	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>(m_MonsterName);

	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionProfile("Monster");
	m_Body->SetWorldPosition(1200.f, 600.f);
	m_Body->SetRadius(22.5f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetRelativeScale(50.f, 50.f);
	return true;
}

void CMonster::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
//사망
	if (m_ObjectData.hp <= 0&& m_MonsterType != EMonsterType::Invisible)
	{
		if (m_Anim)
		{
			m_Anim->ChangeAnimation("Die");
		}
	}
	m_SPatternDelay += deltaTime;
	if (m_SPatternDelay > m_PatternDelay)
	{
		if (m_Anim)
		{
			m_Anim->ChangeAnimation("Skill");
		}
		CSkillManager::GetInst()->ActivateSkill(m_MonsterName, m_Body, m_SkillName, m_SummonCount);
		m_SPatternDelay = 0;
		if (m_MonsterType == EMonsterType::OneShot)
		{
			if (m_Anim)
			{
				m_Anim->ChangeAnimation("Die");
			}
			else
			{
				Destroy();
			}
		}
	}
}

void CMonster::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CMonster* CMonster::Clone() const
{
	return new CMonster(*this);
}

void CMonster::Save(FILE* file)
{
	CGameObject::Save(file);
}

void CMonster::Load(FILE* file)
{
	CGameObject::Load(file);
}

int CMonster::InflictDamage(float damage)
{
	CGameObject::InflictDamage(damage);
	return m_ObjectData.hp;
}
//에디터에서 사용
bool CMonster::Save(const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_MonsterName.c_str());
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	fprintf(file, "[name]\n");
	fprintf(file, m_OutputName.c_str());
	fprintf(file, "\n");
	fprintf(file, "[hp]\n");
	fprintf(file, "%d", m_ObjectData.hp);
	fprintf(file, "[attck]\n");
	fprintf(file, "%d", m_ObjectData.attack);
	fprintf(file, "[defense]\n");
	fprintf(file, "%d", m_ObjectData.defense);
	fprintf(file, "[element]\n");
	fprintf(file, "%d\n", m_ObjectData.element);
	fprintf(file, "[tribe]\n");
	fprintf(file, "%d\n", m_ObjectData.tribe);
	fclose(file);
	return true;
}

bool CMonster::Load(const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_MonsterName.c_str());
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	char szbuff[256] = {};
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_OutputName = szbuff;
	m_OutputName = m_OutputName.substr(0, m_OutputName.length() - 1);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_SkillName = szbuff;
	m_SkillName = m_SkillName.substr(0, m_SkillName.length() - 1);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.hp);
	m_ObjectData.maxHp = m_ObjectData.hp;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.attack);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.defense);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.element);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.tribe);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_SummonCount);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_PatternDelay);
	if(m_PatternDelay==-1.f)
	{
		float min;
		float max;
		Fscanf(szbuff, file);
		fscanf_s(file, "%f", &min);
		Fscanf(szbuff, file);
		fscanf_s(file, "%f", &max);
		m_PatternDelay = rand()% (int)max + min;
	}
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	int isLoop;
	fscanf_s(file, "%d", &isLoop);
	m_IsLoop = isLoop;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_MonsterType);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_LifeTime);
	fclose(file);
	return true;
}

void CMonster::PatternEnd()
{
	m_Anim->ChangeAnimation("Idle");
}