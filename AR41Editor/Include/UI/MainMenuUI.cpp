#include "MainMenuUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingSceneInfo.h"

CMainMenuUI::CMainMenuUI()
{
	m_WindowTypeName = "MainMenuUI";
    m_Scene = CSceneManager::GetInst()->GetScene();
}

CMainMenuUI::CMainMenuUI(const CMainMenuUI& window)
	: CUIWindow(window)
{
    m_StageButton = FindWidget<CUIButton>("StageButton");
    size_t size = std::size(m_OptionButton);
    for (size_t i = 0; i < size; ++i)
    {
        m_OptionButton[i] = FindWidget<CUIButton>("OptionButton_" + std::to_string(i));
    }
}

CMainMenuUI::~CMainMenuUI()
{
    m_Scene->GetResource()->SoundStop("Lobby");
}

void CMainMenuUI::Start()
{
	CUIWindow::Start();
    m_StageButton->SetSound(EButtonEventState::Click, "UI", "ButtonClick", false, "System\\se_character_exchange.mp3");
    size_t size= std::size(m_OptionButton);
    for(size_t i=0; i<size;++i)
    {
        m_OptionButton[i]->SetSound(EButtonEventState::Click,  "ButtonClick");
    }

    m_Scene->GetResource()->SoundPlay("Lobby");

    m_StageButton->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::ChallengeDungeon);
    m_OptionButton[0]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::RePosition);
    m_OptionButton[1]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::Home);
    m_OptionButton[2]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::Character);
    m_OptionButton[3]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::Shop);
    m_OptionButton[4]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::Gacha);
    m_OptionButton[5]->SetCallback<CMainMenuUI>(EButtonEventState::Click, this, &CMainMenuUI::Menu);
}

bool CMainMenuUI::Init()
{
    CUIWindow::Init();
    m_StageButton = CreateWidget<CUIButton>("StageButton");
    m_StageButton->SetSize(300.5f, 210.f);
    m_StageButton->SetPos(450.f, 120.f);
    m_StageButton->SetTexture(EButtonState::Normal, "StageButton"
        , TEXT("Button//challenge_dungeon_event_home_banner.png"));
    m_StageButton->SetTexture(EButtonState::Hovered, "StageButton"
        , TEXT("Button//challenge_dungeon_event_home_banner.png"));
    m_StageButton->SetTexture(EButtonState::Click, "StageButton"
        , TEXT("Button//challenge_dungeon_event_home_banner.png"));
    m_StageButton->SetTexture(EButtonState::Disable, "StageButton"
        , TEXT("Button//challenge_dungeon_event_home_banner.png"));

    CUIImage* blackBG = CreateWidget<CUIImage>("blackBG");
    blackBG->SetPos(235.f, 540.f);
    blackBG->SetSize(600.f, 1080.f);
    blackBG->SetPivot(0.5f, 0.5f);
    blackBG->SetTexture("blackBG", TEXT("Cover//black_square.png"));

    size_t size = std::size(m_OptionButton);
    for (size_t i = 0; i < size; ++i)
    {
        m_OptionButton[i] = CreateWidget<CUIButton>("OptionButton_" + std::to_string(i));
        m_OptionButton[i]->SetSize(140.f, 130.f);
        m_OptionButton[i]->SetPos(545.f + 140.f * i, 0.f);
        if(i==0)
        {
            m_OptionButton[i]->AddPos(-10.f,0.f);
        }
        TCHAR	fileName[MAX_PATH] = {};
        wsprintf(fileName, TEXT("Button\\OptionButton_%d.png"), (int)i);
        m_OptionButton[i]->SetTexture(EButtonState::Normal, "OptionButton_" + std::to_string(i), fileName);
        m_OptionButton[i]->SetTexture(EButtonState::Hovered, "OptionButton_" + std::to_string(i), fileName);
        m_OptionButton[i]->SetTexture(EButtonState::Click, "OptionButton_" + std::to_string(i), fileName);
        m_OptionButton[i]->SetTexture(EButtonState::Disable, "OptionButton_" + std::to_string(i), fileName);
    }
    return true;
}

void CMainMenuUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
}

void CMainMenuUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CMainMenuUI::Render()
{
    CUIWindow::Render();
}

CMainMenuUI* CMainMenuUI::Clone()
{
    return new CMainMenuUI(*this);
}

void CMainMenuUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CMainMenuUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_StageButton = FindWidget<CUIButton>("StageButton");
    size_t size = std::size(m_OptionButton);
    for (size_t i = 0; i < size; ++i)
    {
        m_OptionButton[i] = FindWidget<CUIButton>("OptionButton_"+ std::to_string(i));
    }
}

void CMainMenuUI::ChallengeDungeon()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Party.scn"
        , m_Scene->GetSceneInfo()->GetFileName());
}

void CMainMenuUI::RePosition()
{
}

void CMainMenuUI::Home()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Title.scn"
        , m_Scene->GetSceneInfo()->GetFileName());
}

void CMainMenuUI::Character()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "EditParty.scn", 
        m_Scene->GetSceneInfo()->GetFileName());
}

void CMainMenuUI::Shop()
{
}

void CMainMenuUI::Gacha()
{
}

void CMainMenuUI::Menu()
{
}
