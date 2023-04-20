#pragma once
#include "UI\UIWindow.h"

class CLoadingUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIImage>	m_LoadingTips;
	CSharedPtr<class CUIImage>	m_LoadingCircle;
	CSharedPtr<class CUIProgressBar>	m_LoadingBar;
	CSharedPtr<class CUIText>	m_LoadingText;
	CSharedPtr<class CUINumber>	m_LoadingNumber[3];
	CLoadingUI();
	CLoadingUI(const CLoadingUI& window);
	virtual ~CLoadingUI();
public:
	void SetLoadingPercent(float percent);
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CLoadingUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

