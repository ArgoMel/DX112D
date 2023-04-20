#pragma once
#include "UI\UIWindow.h"

class CUpperUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIButton>	m_CashPlusButton;
	CSharedPtr<class CUIText>	m_MoneyText;
	CSharedPtr<class CUIText>	m_CashText;
	CUpperUI();
	CUpperUI(const CUpperUI& window);
	virtual ~CUpperUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CUpperUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	virtual void AddCash();
};

