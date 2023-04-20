#pragma once
#include "UI\UIWindow.h"

class CPartyUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIButton>	m_LeftButton;
	CSharedPtr<class CUIButton>	m_RightButton;
	CSharedPtr<class CUIButton>	m_PartyDetail;
	CSharedPtr<class CUIButton>	m_SetEdit;
	CSharedPtr<class CUIButton>	m_PlayButton;
	CSharedPtr<class CUIButton>	m_BackButton;
	CSharedPtr<class CUIImage>	m_IndexImg[6];
	CSharedPtr<class CUIImage>	m_FrameCharImg[3];
	CSharedPtr<class CUIImage>	m_CharImg[3];
	CSharedPtr<class CUIImage>	m_FrameUnisonImg[3];
	CSharedPtr<class CUIImage>	m_UnisonImg[3];
	int m_Index;
	CPartyUI();
	CPartyUI(const CPartyUI& window);
	virtual ~CPartyUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CPartyUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void Play();
	void Back();
	void Left();
	void RIght();
	void EditParty();
};

