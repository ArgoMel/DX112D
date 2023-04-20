#pragma once
#include "UI\UIWindow.h"

class CStageUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	friend class CPlayer2D;
	CSharedPtr<class CBackGround> m_BG;
	CSharedPtr<class CUIImage>	m_BlackImg;
	CSharedPtr<class CUIText>	m_SecondText;
	CSharedPtr<class CUIText>	m_MinuteText;
	CSharedPtr<class CUIText>	m_ScoreText;
	CSharedPtr<class CUIText>	m_ComboText;
	CSharedPtr<class CUIImage>			m_FeverImg;
	CSharedPtr<class CUIProgressBar>	m_FeverBar;
	CSharedPtr<class CUIImage>			m_CharImg[3];
	CSharedPtr<class CUIImage>			m_CharCover[3];
	CSharedPtr<class CUIProgressBar>	m_HpBar[3];
	CSharedPtr<class CUIText>	m_HpText[3];
	CSharedPtr<class CUIProgressBar>	m_SkillChargeBar[3];	//중앙에 노란색 차는거
	CSharedPtr<class CUIImage>	m_SkillCurtain;
	CSharedPtr<class CUIImage>	m_SkillCurtainDark;
	CSharedPtr<class CUIImage>	m_SkillBar[3];
	CSharedPtr<class CUIImage>	m_CharDirMarker;	//흰색줄
	CSharedPtr<class CUIImage>	m_MousePosImg;
	CSharedPtr<class CUIImage>	m_BlackBG[2];
//리트라이 UI
	CSharedPtr<class CUIImage>	m_GameOverImg;
	CSharedPtr<class CUIImage>	m_RetryBG;
	CSharedPtr<class CUIText>	m_RetryText;
	CSharedPtr<class CUIText>	m_RetrySemiText;
	CSharedPtr<class CUIButton>	m_ForgiveBtn;
	CSharedPtr<class CUIButton>	m_RetryBtn;
	CSharedPtr<class CUIText>	m_ForgiveBtnText;
	CSharedPtr<class CUIText>	m_RetryBtnText;
//
	Vector2		m_RClickPos;
	float		m_barDistance;
	float m_Time;
	float m_PlayTime;
	float m_DarkTime;
	float m_FadeTime;
	int m_TotalTime;
	int m_SkillBarIndex;
	bool m_IsGaming;
	bool m_IsLarger;
	bool m_IsRetryUI;
	bool m_IsSkillOn;
	CStageUI();
	CStageUI(const CStageUI& window);
	virtual ~CStageUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CStageUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void RetryUI();
	void Damaged(int index,float value);
	void SetSkillBar(int index, float gauge, int wait);
	void TouchWindow();
	void HoldWindow(float angle,float distance);
	void ReleaseWindow(float gauge[3], int wait[6]);
	bool ViewSkill(float gauge[3], int wait[6]);
	void DragSkill();
	int ActivateSkill(float gauge[3], int wait[6]);
	void Pause();
	void Back();
	void Retry();
	void AddScore(int score);
};

