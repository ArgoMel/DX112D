#include "UpperUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../GameObject/BackGround.h"

CUpperUI::CUpperUI()
{
	m_WindowTypeName = "UpperUI";
	m_Scene = CSceneManager::GetInst()->GetScene();
}

CUpperUI::CUpperUI(const CUpperUI& window)
	: CUIWindow(window)
{
    m_CashPlusButton = FindWidget<CUIButton>("cashPlusButton");
    m_MoneyText = FindWidget<CUIText>("moneyText");
    m_CashText = FindWidget<CUIText>("cashText");
}

CUpperUI::~CUpperUI()
{
}

void CUpperUI::Start()
{
    CUIWindow::Start();
    m_CashPlusButton->SetSound(EButtonEventState::Click, "UI", "ButtonClick2", false, "System\\se_character_select.mp3");

    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        int money = bg->GetUserData().money;
        TCHAR	fileName[MAX_PATH] = {};
        wsprintf(fileName, TEXT("%d"), money);
        m_MoneyText->SetText(fileName);
        money = bg->GetUserData().cash;
        wsprintf(fileName, TEXT("%d"), money);
        m_CashText->SetText(fileName);
    }

    m_CashPlusButton->SetCallback<CUpperUI>(EButtonEventState::Click, this, &CUpperUI::AddCash);
}

bool CUpperUI::Init()
{
    CUIWindow::Init();
    CUIImage* upperBar = CreateWidget<CUIImage>("UpperBar");
    upperBar->SetPos(960.f, 1040.f);
    upperBar->SetSize(850.f, 90.f);
    upperBar->SetPivot(0.5f, 0.5f);
    upperBar->SetTexture("UpperBar", TEXT("UI//UpperBar.png"));

    m_MoneyText = CreateWidget<CUIText>("moneyText");
    m_MoneyText->SetPos(720.f, 960.f);
    m_MoneyText->SetSize(300.f, 100.f);
    m_MoneyText->SetPivot(0.5f, 0.5f);
    m_MoneyText->SetFontSize(30.f);
    m_MoneyText->SetColor(0, 0, 0);
    m_MoneyText->SetAlignH(Text_Align_H::Right);
    m_MoneyText->SetText(TEXT("0"));

    m_CashText = CreateWidget<CUIText>("cashText");
    m_CashText->SetPos(900.f, 960.f);
    m_CashText->SetSize(300.f, 100.f);
    m_CashText->SetPivot(0.5f, 0.5f);
    m_CashText->SetFontSize(30.f);
    m_CashText->SetColor(0, 0, 0);
    m_CashText->SetAlignH(Text_Align_H::Right);
    m_CashText->SetText(TEXT("0"));

    m_CashPlusButton = CreateWidget<CUIButton>("cashPlusButton");
    m_CashPlusButton->SetSize(230.f, 80.f);
    m_CashPlusButton->SetPos(1030.f, 1000.f);
    m_CashPlusButton->SetOpacity(0.2f);
    return true;
}

void CUpperUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
}

void CUpperUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CUpperUI::Render()
{
    CUIWindow::Render();
}

CUpperUI* CUpperUI::Clone()
{
    return new CUpperUI(*this);
}

void CUpperUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CUpperUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_CashPlusButton = FindWidget<CUIButton>("cashPlusButton");
    m_MoneyText = FindWidget<CUIText>("moneyText");
    m_CashText = FindWidget<CUIText>("cashText");
}

void CUpperUI::AddCash()
{
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        int cash = bg->GetUserData().cash + 1500;
        if (cash > 999999)
        {
            cash = 999999;
            bg->SetCash(cash);
        }
        else
        {
            bg->AddCash(1500);
        }
        TCHAR	fileName[MAX_PATH] = {};
        wsprintf(fileName, TEXT("%d"), cash);
        m_CashText->SetText(fileName);
    }
}
