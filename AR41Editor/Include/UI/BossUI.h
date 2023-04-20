#pragma once
#include "UI\UIWindow.h"

class CBossUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIImage>			m_BossCover;
	CSharedPtr<class CUIText>			m_BossName;
	CSharedPtr<class CUIImage>			m_BossSkillCover;
	CSharedPtr<class CUIText>			m_BossSkillName;
	CSharedPtr<class CUIProgressBar>	m_BossHPBar;
	CSharedPtr<class CUIImage>			m_BossBuff[(int)EBuff::End];
	CSharedPtr<class CUIImage>			m_BossDeBuff[(int)EDeBuff::End];
	float m_TextTime;
	CBossUI();
	CBossUI(const CBossUI& window);
	virtual ~CBossUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CBossUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void Damaged(float value);
	void SetSkillName(const std::string& name,float sizeX=203.f,float sizeY=61.f);
	void SetText(const TCHAR* text);
	void SetBuffImg(int index, bool b);
	void SetDeBuffImg(int index, bool b);
};

