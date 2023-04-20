#pragma once
#include "UI\UIWindow.h"

class CGameClearUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	friend class CPlayer2D;
	CSharedPtr<class CBackGround> m_BG;
	CSharedPtr<class CUIText>	m_MinuteText;
	CSharedPtr<class CUIText>	m_SecondText;
	CSharedPtr<class CUIText>	m_MiliSecondText;
	CSharedPtr<class CUIText>	m_FMinuteText;
	CSharedPtr<class CUIText>	m_FSecondText;
	CSharedPtr<class CUIText>	m_FMiliSecondText;
	CSharedPtr<class CUIText>	m_ScoreText;
	CSharedPtr<class CUIText>	m_HighScoreText;
	CSharedPtr<class CUIImage>	m_UpperCharImg[6];
	CSharedPtr<class CUIImage>	m_CharImg[6];
	CSharedPtr<class CUIImage>	m_MainStatueImg[3];
	CSharedPtr<class CUIImage>	m_UnisonStatueImg[3];
	CSharedPtr<class CUIText>	m_NextBtnText;
	CSharedPtr<class CUIButton>	m_NextBtn;
	CGameClearUI();
	CGameClearUI(const CGameClearUI& window);
	virtual ~CGameClearUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CGameClearUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void Lobby();
};

