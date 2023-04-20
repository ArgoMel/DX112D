#pragma once
#include "UI\UIWindow.h"

class CMainMenuUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIButton>	m_StageButton;
	CSharedPtr<class CUIButton>	m_OptionButton[6];
	CMainMenuUI();
	CMainMenuUI(const CMainMenuUI& window);
	virtual ~CMainMenuUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CMainMenuUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
	void ChallengeDungeon();
	void RePosition();
	void Home();
	void Character();
	void Shop();
	void Gacha();
	void Menu();
};

