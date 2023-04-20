#include "TitleUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CTitleUI::CTitleUI()
	: m_Opacity(0.f)
	, m_Dir(1)
{
	m_WindowTypeName = "TitleUI";
	m_Scene = CSceneManager::GetInst()->GetScene();
}

CTitleUI::CTitleUI(const CTitleUI& window)
	: CUIWindow(window)
	, m_Opacity(0.f)
	, m_Dir(1)
{
	m_Title = FindWidget<CUIImage>("Title");
	m_TitleImg = FindWidget<CUIImage>("TitleImg");
	m_TitleImgText = FindWidget<CUIImage>("TitleImgText");
}

CTitleUI::~CTitleUI()
{
	m_Scene->GetResource()->SoundStop("Title");
}

void CTitleUI::Start()
{
	CUIWindow::Start();
	m_Scene->GetResource()->SoundPlay("Title");
}

bool CTitleUI::Init()
{
	CUIWindow::Init();
	m_Title = CreateWidget<CUIImage>("Title");
	m_Title->SetSize(2000.f * 0.25f, 1258.f * 0.25f);
	m_Title->SetPos(960.f, 800.f);
	m_Title->SetPivot(0.5f, 0.5f);
	m_Title->SetTexture("Title", TEXT("Cover//Title_name.png"));

	m_TitleImg = CreateWidget<CUIImage>("TitleImg");
	m_TitleImg->SetSize(1007.f * 0.6f, 100.f * 0.6f);
	m_TitleImg->SetPos(960.f, 250.f);
	m_TitleImg->SetPivot(0.5f, 0.5f);
	m_TitleImg->SetTexture("TitleImg", TEXT("Cover//tap_screen_bg.png"));

	m_TitleImgText = CreateWidget<CUIImage>("TitleImgText");
	m_TitleImgText->SetSize(347.f * 0.6f, 41.f * 0.6f);
	m_TitleImgText->SetPos(960.f, 250.f);
	m_TitleImgText->SetPivot(0.5f, 0.5f);
	m_TitleImgText->SetTexture("TitleImgText", TEXT("Cover//tap_screen.png"));
	return true;
}

void CTitleUI::Update(float deltaTime)
{
	CUIWindow::Update(deltaTime);
	if(m_Opacity>1.5f)
	{
		m_Dir = -1;
	}
	if (m_Opacity < 0.3f)
	{
		m_Dir= 1;
	}
	m_Opacity +=deltaTime* m_Dir;
	m_TitleImg->SetOpacity(m_Opacity);
	m_TitleImgText->SetOpacity(m_Opacity);
}

void CTitleUI::PostUpdate(float deltaTime)
{
	CUIWindow::PostUpdate(deltaTime);
}

void CTitleUI::Render()
{
	CUIWindow::Render();
}

CTitleUI* CTitleUI::Clone()
{
	return new CTitleUI(*this);
}

void CTitleUI::Save(FILE* file)
{
	CUIWindow::Save(file);
}

void CTitleUI::Load(FILE* file)
{
	CUIWindow::Load(file);
	m_Title = FindWidget<CUIImage>("Title");
	m_TitleImg = FindWidget<CUIImage>("TitleImg");
	m_TitleImgText = FindWidget<CUIImage>("TitleImgText");
}
