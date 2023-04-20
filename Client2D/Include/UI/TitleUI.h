#pragma once
#include "UI\UIWindow.h"

class CTitleUI : public CUIWindow
{
protected:
	friend class CSceneViewport;
	friend class CScene;
	CSharedPtr<class CUIImage>	m_Title;
	CSharedPtr<class CUIImage>	m_TitleImg;
	CSharedPtr<class CUIImage>	m_TitleImgText;
	float m_Opacity;
	int m_Dir;
	CTitleUI();
	CTitleUI(const CTitleUI& window);
	virtual ~CTitleUI();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
	virtual CTitleUI* Clone();
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

