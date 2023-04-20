#pragma once
#include "UI\UIWindow.h"

class CEditPartyUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIButton>	m_LeftButton;
	CSharedPtr<class CUIButton>	m_RightButton;
	CSharedPtr<class CUIButton>	m_SortButton;
	CSharedPtr<class CUIButton>	m_BackButton;
	CSharedPtr<class CUIImage>	m_SpriteImg[6];
	CSharedPtr<class CUIImage>	m_MainStatueImg[3];
	CSharedPtr<class CUIImage>	m_UnisonStatueImg[3];
	CSharedPtr<class CUIImage>	m_AllCharSatue[16];
	CSharedPtr<class CUIImage>	m_AllCharImg[16];
	CSharedPtr<class CUIImage>	m_MouseImg;
	size_t m_Index;
	CEditPartyUI();
	CEditPartyUI(const CEditPartyUI& window);
	virtual ~CEditPartyUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CEditPartyUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void Sort();
	void Back();
	void Left();
	void Right();
	void IconClick();
	void IconRelease();
};

