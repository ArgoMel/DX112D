#include "Boss.h"
#include "Player2D.h"
#include "Marker.h"
#include "Device.h"
#include "PathManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderSphere2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Resource/Material/Material.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../Skill/SkillManager.h"
#include "../UI/BossUI.h"

CBoss::CBoss()
	: m_SkillCount(0)
	, m_dir(-1)
	, m_RandomNum(0)
	, m_LoopIndex(-1)
	, m_SChargeCount(0)
	, m_SMaxSummonCount(0)
	, m_SLoopCount(0)
	, m_SChargeTime(0.f)
	, m_SDownTime(0.f)
	, m_SPatternDelay(0.f)
	, m_DeadTime(-1.f)
{
	SetTypeID<CBoss>();
	m_ObjectTypeName = "Boss";
}

CBoss::CBoss(const CBoss& obj)
	: CGameObject(obj)
	, m_dir(-1)
	, m_SkillCount(obj.m_SkillCount)
	, m_RandomNum(0)
	, m_LoopIndex(-1)
	, m_SChargeCount(0)
	, m_SMaxSummonCount(0)
	, m_SChargeTime(0.f)
	, m_SDownTime(0.f)
	, m_SPatternDelay(0.f)
	, m_DeadTime(-1.f)
{
	m_Body = (CColliderSphere2D*)FindComponent("Body");
	m_Sprite = (CSpriteComponent*)FindComponent(m_BossName);
}

CBoss::~CBoss()
{
	if(m_Scene)
	{
		m_Scene->GetResource()->SoundStop(m_BossName);
		m_Scene->GetResource()->FindChannelGroup("Effect")->stop();
	}
}

void CBoss::Destroy()
{
	CGameObject::Destroy();
	CSceneManager::GetInst()->CreateNextScene();
	if (m_Scene->GetName() == "ChallengeDungeon")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "ChallengeDungeon2.scn");
	}
	else if (m_Scene->GetName() == "ChallengeDungeon2")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "ChallengeDungeon3.scn");
	}
	else if (m_Scene->GetName() == "ChallengeDungeon3")
	{
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "DungeonClear.scn");
	}
}

void CBoss::Start()
{
	CGameObject::Start();
	m_BossUI=m_Scene->GetViewport()->CreateUIWindow<CBossUI>("BossUI");
	m_BossName = m_Sprite->GetName();
	m_Scene->GetResource()->SoundPlay(m_BossName);
	std::string imgPath = "Boss\\" + m_BossName + "\\000a.png";
	TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
	t_filename[imgPath.size()] = 0;
	std::copy(imgPath.begin(), imgPath.end(), t_filename);
	m_Sprite->GetMaterial(0)->SetTexture(0, 0, (int)EShaderBufferType::Pixel, m_BossName, t_filename);
	SAFE_DELETE_ARRAY(t_filename);
	m_Sprite->SetRelativeScale((float)m_Sprite->GetMaterial(0)->GetTexture()->GetWidth() * 2.5f,
		(float)m_Sprite->GetMaterial(0)->GetTexture()->GetHeight() * 2.5f);
//랜덤 범위 설정
	Resolution RS = CDevice::GetInst()->GetResolution();
	m_RangeX[0] = (float)RS.width * 0.4f;
	m_RangeX[1] = (float)RS.width * 0.6f;
	m_RangeY[0] = (float)RS.height * 0.3f;
	m_RangeY[1] = (float)RS.height * 0.6f;
//로드
	if (!Load(SAVE_PATH))
	{
		return;
	}
	m_RandomNum = m_NormalPatternCount;
	m_StartPos = GetWorldPos();
//이름 설정
	t_filename = new TCHAR[m_OutputName.size() + 1];
	t_filename[m_OutputName.size()] = 0;
	std::copy(m_OutputName.begin(), m_OutputName.end(), t_filename);
	m_BossUI->SetText(t_filename);
	SAFE_DELETE_ARRAY(t_filename);
//애니메이션
	m_Anim = m_Sprite->SetAnimation<CAnimation2D>("BossAnim");
	m_Anim->AddAnimation("Die", "BossDie");
	std::string animeName = m_BossName + "_die";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->AddAnimation("Stun", "BossStun", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Stun");
	animeName = m_BossName + "_stun";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	m_Anim->AddAnimation("Charge", "BossCharge", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Charge");
	animeName = m_BossName + "_charge";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
	for (int i = 0; i < m_SkillCount;++i)
	{
		m_Anim->AddAnimation("Skill" + std::to_string(i), "BossSkill" + std::to_string(i), 1.f, 1.f, m_vecIsLoop[i]);
		m_Anim->SetCurrentAnimation("Skill" + std::to_string(i));
		animeName = m_BossName + "_skill"+ std::to_string(i);
		m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
		if(!m_vecIsLoop[i])
		{
			m_Anim->SetCurrentEndFunction("Skill" + std::to_string(i), this, &CBoss::PatternEnd);
		}
	}
	m_Anim->AddAnimation("Idle", "BossIdle", 1.f, 1.f, true);
	m_Anim->SetCurrentAnimation("Idle");
	animeName = m_BossName + "_idle";
	m_Anim->GetCurrentAnimationSequence()->Load(animeName.c_str(), ANIMATION_PATH);
}

bool CBoss::Init()
{
	CGameObject::Init();
	m_Body = CreateComponent<CColliderSphere2D>("Body");
	m_Sprite = CreateComponent<CSpriteComponent>(m_BossName);

	m_Body->AddChild(m_Sprite);

	m_Body->SetCollisionProfile("Monster");
	m_Body->SetWorldPosition(960.f, 700.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	return true;
}

void CBoss::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
	if (m_StatusData.isDeBuff[(int)EDeBuff::Paralysis])
	{
		return;
	}
	if(!m_Anim|| m_Anim->GetCurrentAnimationName() == "Die")
	{
		return;
	}
//차지상태
	if (m_Anim->GetCurrentAnimationName() == "Charge")
	{
		m_SChargeTime += deltaTime;
		int leftTime = (int)(m_ChargeTime - m_SChargeTime);
		m_BossUI->SetSkillName(std::to_string(leftTime),40.f,50.f);
		int markerCount =0;
		auto	iter = m_listMarker.begin();
		auto	iterEnd = m_listMarker.end();
		for (; iter != iterEnd; ++iter)
		{
			if (!(*iter)->GetActive())
			{
				++markerCount;
				continue;
			}
		}
//마커를 전부 깬 경우
		if (markerCount >= m_MarkerCount)
		{
			m_Scene->GetResource()->SoundStop("BossCharge");
			m_Scene->GetResource()->SoundPlay("BossStun");
			m_Anim->ChangeAnimation("Stun");
			m_SPatternDelay = 0;
			m_SChargeCount = 0;
			m_listMarker.clear();
			return;
		}
//시간초과
		if(m_SChargeTime> m_ChargeTime)
		{
			m_Scene->GetResource()->SoundStop("BossCharge");
			m_Anim->ChangeAnimation("Skill" + std::to_string(m_SkillCount - 1));
			m_RandomNum = rand() %(m_SkillCount - m_NormalPatternCount)+ m_NormalPatternCount;
			m_BossUI->SetSkillName(m_SkillName[m_RandomNum]);
			CSkillManager::GetInst()->ActivateSkill(m_BossName + std::to_string(m_RandomNum), 
				m_Body, m_SkillName[m_RandomNum], m_vecSummonCount[m_RandomNum]);
			auto	iter = m_listMarker.begin();
			auto	iterEnd = m_listMarker.end();
			for (; iter != iterEnd; ++iter)
			{
				(*iter)->Destroy();
			}
			m_listMarker.clear();
			m_SPatternDelay = 0;
			m_SChargeCount = 0;
			if (m_vecIsLoop[m_RandomNum])
			{
				m_LoopIndex = m_RandomNum;
			}
		}
		return;
	}
//스턴상태
	if(m_Anim->GetCurrentAnimationName()=="Stun")
	{
		m_SDownTime += deltaTime;
		if(m_SDownTime> m_DownTime)
		{
			m_SDownTime = 0;
			m_Anim->ChangeAnimation("Idle");
		}
		return;
	}
//일반패턴
	m_SPatternDelay += deltaTime;
	if(m_SPatternDelay> m_vecPatternDelay[m_RandomNum])
	{
//m_ChargeCount가 -1이면 랜덤으로 스페셜스킬 발동
		if(m_ChargeCount==-1&& m_LoopIndex == -1)
		{
			if(m_RandomNum== m_NormalPatternCount)
			{
				m_SChargeCount = -2;
			}
			else
			{
				m_SChargeCount = rand() % 2 ? -2 : 0;
			}
		}
//일반패턴
		if (m_SChargeCount < m_ChargeCount)
		{
			if (m_LoopIndex == -1)
			{
				int randomNum = rand() % m_NormalPatternCount;
				while(m_RandomNum== randomNum)
				{
					randomNum = rand() % m_NormalPatternCount;
				}
				m_RandomNum = randomNum;
			}
//0번째 스킬을 소환스킬로 고정
			if(m_RandomNum ==0)	
			{
				m_SMaxSummonCount += m_vecSummonCount[m_RandomNum];
				if(m_SMaxSummonCount> m_MaxSummonCount)
				{
					m_SMaxSummonCount = m_MaxSummonCount;
					++m_RandomNum;
					m_LoopIndex = -1;
					m_SLoopCount = 0;
					m_MoveType = EMoveType::None;
				}
			}
			if (m_vecIsLoop[m_RandomNum])
			{
				m_LoopIndex = m_RandomNum;
				++m_SLoopCount;
				if (m_SLoopCount > m_vecLoopCount[m_LoopIndex])
				{
					m_LoopIndex = -1;
					m_SLoopCount = 0;
					m_MoveType = EMoveType::None;
					if (m_RandomNum < m_NormalPatternCount)
					{
						++m_SChargeCount;
					}
					return;
				}
			}
			m_Anim->ChangeAnimation("Skill" + std::to_string(m_RandomNum));
			m_BossUI->SetSkillName(m_SkillName[m_RandomNum]);
			CSkillManager::GetInst()->ActivateSkill(m_BossName + std::to_string(m_RandomNum),
				m_Body, m_SkillName[m_RandomNum], m_vecSummonCount[m_RandomNum]);
			m_SPatternDelay = 0;
			if (m_LoopIndex == -1)
			{
				++m_SChargeCount;
			}
		}
//차지상태로 변경
		else
		{
			m_Scene->GetResource()->SoundPlay("BossCharge");
			m_Anim->ChangeAnimation("Charge");
			m_MoveType = EMoveType::None;
			m_SChargeTime = 0;
			Vector2 pos = GetWorldPos();
			for (int i = 0; i < m_MarkerCount; ++i)
			{
				CMarker* marker = m_Scene->CreateObject<CMarker>("Marker");
				marker->SetWorldPosition(pos.x + m_vecMarkerPos[i].x, pos.y + m_vecMarkerPos[i].y);
				m_listMarker.push_back(marker);
			}
		}
	}
}

void CBoss::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
	if (m_StatusData.isDeBuff[(int)EDeBuff::Paralysis])
	{
		return;
	}
	if (m_DeadTime > 0.f)
	{
		m_DeadTime -= deltaTime;
		if (m_DeadTime <= 0.f)
		{
			Destroy();
			return;
		}
	}
	if (!m_Anim || m_Anim->GetCurrentAnimationName() == "Die")
	{
		return;
	}
	BossMove(m_MoveType, deltaTime);
	m_BossUI->SetPos(GetWorldPos());
}

CBoss* CBoss::Clone() const
{
	return new CBoss(*this);
}

void CBoss::Save(FILE* file)
{
	CGameObject::Save(file);
	int	length = (int)m_Sprite->GetName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Sprite->GetName().c_str(), 1, length, file);
}

void CBoss::Load(FILE* file)
{
	CGameObject::Load(file);
	size_t	length = 0;
	char	typeName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(typeName, 1, length, file);
	m_Sprite->SetName(typeName);
}
//에디터에서 사용
bool CBoss::Save(const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_BossName.c_str());
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
	fprintf(file, "[skill_count]\n");
	fprintf(file, "%d\n", m_SkillCount);
	fclose(file);
	return true;
}

bool CBoss::Load(const std::string& pathName)
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(pathName);
	if (path)
	{
		strcpy_s(fullPath, path->pathMultibyte);
	}
	strcat_s(fullPath, m_BossName.c_str());
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	char szbuff[256] = {};
	float f_Temp;
	std::string s_Temp;
	Vector2 v_Temp;
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_OutputName= szbuff;
	m_OutputName = m_OutputName.substr(0, m_OutputName.length() - 1);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_SkillCount);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	m_SkillName->resize(m_SkillCount);
	for (int i = 0; i < m_SkillCount; ++i)
	{
		Fscanf(szbuff, file);
		m_SkillName[i] = szbuff;
		m_SkillName[i] = m_SkillName[i].substr(0, m_SkillName[i].length() - 1);
	}
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
	fscanf_s(file, "%f", &m_ObjectData.speed);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.element);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ObjectData.tribe);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_MoveType);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_NormalPatternCount);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_ChargeCount);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_ChargeTime);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%f", &m_DownTime);
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_MarkerCount);
	Fscanf(szbuff, file);
	m_vecMarkerPos.resize(m_MarkerCount);
	for (int i = 0; i < m_MarkerCount; ++i)
	{
		Fscanf(szbuff, file);
		fscanf_s(file, "%f", &f_Temp);
		m_vecMarkerPos[i].x = f_Temp;
		Fscanf(szbuff, file);
		fscanf_s(file, "%f", &f_Temp);
		m_vecMarkerPos[i].y = f_Temp;
	}
	Fscanf(szbuff, file);
	m_vecSummonCount.resize(m_SkillCount);
	for (int i = 0; i < m_SkillCount; ++i)
	{
		Fscanf(szbuff, file);
		fscanf_s(file, "%d", &m_vecSummonCount[i]);
	}
	Fscanf(szbuff, file);
	Fscanf(szbuff, file);
	fscanf_s(file, "%d", &m_MaxSummonCount);
	Fscanf(szbuff, file);
	m_vecPatternDelay.resize(m_SkillCount);
	for (int i = 0; i < m_SkillCount; ++i)
	{
		Fscanf(szbuff, file);
		fscanf_s(file, "%f", &m_vecPatternDelay[i]);
	}
	Fscanf(szbuff, file);
	m_vecIsLoop.resize(m_SkillCount);
	for (int i = 0; i < m_SkillCount;++i)
	{
		int isLoop;
		Fscanf(szbuff, file);
		fscanf_s(file,"%d", &isLoop);
		m_vecIsLoop[i] = isLoop;
	}
	Fscanf(szbuff, file);
	m_vecLoopCount.resize(m_SkillCount);
	for (int i = 0; i < m_SkillCount; ++i)
	{
		Fscanf(szbuff, file);
		fscanf_s(file, "%d", &m_vecLoopCount[i]);
	}
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

int CBoss::InflictDamage(float damage)
{
	CGameObject::InflictDamage(damage);
	if (m_Anim&&m_Anim->GetCurrentAnimationName() == "Boss_Stun")
	{
		damage *= 1.5f;
	}
	m_BossUI->Damaged(m_ObjectData.hp / (float)m_ObjectData.maxHp*100.f);
//사망
	if (m_ObjectData.hp <= 0)
	{
		if (!m_Anim || m_Anim->GetCurrentAnimationName() == "Die")
		{
			return 0;
		}
		m_Scene->GetResource()->SoundPlay("BossDie");
		CPlayer2D* player = (CPlayer2D*)m_Scene->GetSceneInfo()->GetPlayerObject();
		if (!player->PlayerWin()) 
		{
			return 0;
		}
		m_Body->SetCollisionProfile("Invisible");
		m_BossUI->Destroy();
		if(m_Anim)
		{
			m_Anim->ChangeAnimation("Die");
			m_DeadTime = 3.f;
		}
	}
	return m_ObjectData.hp;
}

void CBoss::SetBuff(int index, float time, float quantity, bool b)
{
	CGameObject::SetBuff(index, time, quantity, b);
	m_BossUI->SetBuffImg(index, b);
}

void CBoss::SetDeBuff(int index, float time, float quantity, bool b)
{
	CGameObject::SetDeBuff(index,time, quantity,b);
	m_BossUI->SetDeBuffImg(index, b);
}

void CBoss::PatternEnd()
{
	m_Anim->ChangeAnimation("Idle");
}

void CBoss::SetRandomPos()
{
	m_RandomPos.x = rand() % (int)(m_RangeX[1] - m_RangeX[0]) + m_RangeX[0];
	m_RandomPos.y = rand() % (int)(m_RangeY[1] - m_RangeY[0]) + m_RangeY[0];
}

void CBoss::BossMove(EMoveType moveType, float deltaTime)
{
	if(moveType == EMoveType::None)
	{
		m_dir = -1;
	}
	else if (moveType == EMoveType::LeftRight)
	{
		if (m_dir == 1 && m_Body->GetWorldPos().x > m_RangeX[1])
		{
			m_dir = -1;
		}
		if (m_dir == -1 && m_Body->GetWorldPos().x < m_RangeX[0])
		{
			m_dir = 1;
		}
		m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS::AXIS_X) * m_ObjectData.speed * (float)m_dir * deltaTime);
	}
	else if (moveType == EMoveType::Random)
	{
		Vector2 pos = GetWorldPos();
		if(pos.Distance(m_RandomPos)<1.f)
		{
			return;
		}
		float angle = pos.Angle(m_RandomPos);
		AddWorldPositionX(cosf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
		AddWorldPositionY(sinf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
	}
	else if (moveType == EMoveType::RushDown)
	{
		Vector2 pos = GetWorldPos();
		Vector2 goalPos = Vector2(960.f, 150.f);
		if (pos.y > m_StartPos.y && m_dir == 1)
		{
			return;
		}
		if (pos.y < goalPos.y&& m_dir == -1)
		{
			m_dir = 1;
			m_Anim->ChangeAnimation("Idle");
			CSkillManager::GetInst()->ActivateSkill(m_BossName + std::to_string(5), m_Body, "explosion", 1);
		}
		if(m_dir == -1)
		{
			float angle = pos.Angle(goalPos);
			AddWorldPositionX(cosf(DegreeToRadian(angle)) * m_ObjectData.speed * 4.f * deltaTime);
			AddWorldPositionY(sinf(DegreeToRadian(angle)) * m_ObjectData.speed * 4.f * deltaTime);
			return;
		}
		float angle = pos.Angle(m_StartPos);
		AddWorldPositionX(cosf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
		AddWorldPositionY(sinf(DegreeToRadian(angle)) * m_ObjectData.speed * deltaTime);
	}
}
