#include "PartyUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../GameObject/BackGround.h"

CPartyUI::CPartyUI()
    : m_Index(0)
{
    m_WindowTypeName = "PartyUI";
    m_Scene = CSceneManager::GetInst()->GetScene();
}

CPartyUI::CPartyUI(const CPartyUI& window)
    : CUIWindow(window)
    , m_Index(window.m_Index)
{   
	m_LeftButton = FindWidget<CUIButton>("ChangeLButton");
	m_RightButton = FindWidget<CUIButton>("ChangeRButton");
	m_PartyDetail = FindWidget<CUIButton>("PartyDetailButton");
	m_SetEdit = FindWidget<CUIButton>("SetEditButton");
	m_PlayButton = FindWidget<CUIButton>("PlayButton");
	m_BackButton = FindWidget<CUIButton>("BackButton");
    size_t size = std::size(m_IndexImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_IndexImg[i] = FindWidget<CUIImage>("IndexImg_" + std::to_string(i));
    }
    size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_FrameCharImg[i] = FindWidget<CUIImage>("FrameCharImg_" + std::to_string(i));
        m_CharImg[i] = FindWidget<CUIImage>("FrameImg_" + std::to_string(i));
        m_UnisonImg[i] = FindWidget<CUIImage>("UnisonCharImg" + std::to_string(i));
        m_FrameUnisonImg[i] = FindWidget<CUIImage>("FrameUnisonImg_" + std::to_string(i));
    }
}

CPartyUI::~CPartyUI()
{
	m_Scene->GetResource()->SoundStop("Party");
}

void CPartyUI::Start()
{
    CUIWindow::Start();
	m_Scene->GetResource()->SoundPlay("Party");
    m_LeftButton->SetSound(EButtonEventState::Click, "ButtonClick1");
	m_RightButton->SetSound(EButtonEventState::Click, "ButtonClick1");
	m_PartyDetail->SetSound(EButtonEventState::Click, "ButtonClick1");
	m_SetEdit->SetSound(EButtonEventState::Click, "ButtonClick1");
	m_PlayButton->SetSound(EButtonEventState::Click, "UI", "GameStart", false, "System\\se_battle_start.mp3");
    m_BackButton->SetSound(EButtonEventState::Click, "ButtonClick3");

    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        m_Index = bg->GetUserData().partyIndex;
        size_t size = std::size(m_IndexImg);
        for (size_t i = 0; i < size; ++i)
        {
            if(i== m_Index)
            {
                m_IndexImg[i]->SetImageTint(0,255,255,255);
                m_IndexImg[i]->SetSize(30.f, 30.f);
                continue;
            }
            m_IndexImg[i]->SetImageTint(255, 255, 255, 255);
            m_IndexImg[i]->SetSize(27.f, 27.f);
        }
        for (int i = (m_Index)*6; i < (m_Index) * 6+3; ++i)
        {
            std::string name= bg->GetUserData().charName[i];
            if(name=="none")
            {
                m_CharImg[i%3]->SetOpacity(0.f);
                m_FrameCharImg[i%3]->SetOpacity(0.f);
            }
            else
            {
                m_CharImg[i % 3]->SetOpacity(1.f);
                m_FrameCharImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//"+ name +"//ui//thumb_party_main_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_FrameCharImg[i%3]->SetTexture("FrameCharImg_" + std::to_string(i)+name,t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
        for (int i = (m_Index) * 6+3; i < (m_Index) * 6 + 6; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_UnisonImg[i % 3]->SetOpacity(0.f);
            }
            else
            {
                m_UnisonImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//ui//thumb_party_unison_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_UnisonImg[i % 3]->SetTexture("UnisonCharImg" + std::to_string(i) + name, t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
    }

	m_LeftButton->SetCallback<CPartyUI>(EButtonEventState::Click, this, &CPartyUI::Left);
	m_RightButton->SetCallback<CPartyUI>(EButtonEventState::Click, this, &CPartyUI::RIght);
	m_PlayButton->SetCallback<CPartyUI>(EButtonEventState::Click, this, &CPartyUI::Play);
	m_BackButton->SetCallback<CPartyUI>(EButtonEventState::Click, this, &CPartyUI::Back);
    m_PartyDetail->SetCallback<CPartyUI>(EButtonEventState::Click, this, &CPartyUI::EditParty);
}

bool CPartyUI::Init()
{
    CUIWindow::Init();
    CUIImage* bossImg = CreateWidget<CUIImage>("bossImg");
    bossImg->SetSize(850.f, 320.f);
    bossImg->SetPos(960.f, 870.f);
    bossImg->SetPivot(0.5f, 0.5f);
    bossImg->SetTexture("bossImg", TEXT("BG//challenge_dungeon_event_background_01.png"));

    CUIImage* partyWindowImg = CreateWidget<CUIImage>("party_window");
    partyWindowImg->SetSize(1114.f*0.6f, 733.f*0.6f);
    partyWindowImg->SetPos(960.f, 500.f);
    partyWindowImg->SetPivot(0.5f, 0.5f);
    partyWindowImg->SetTexture("party_window", TEXT("UI//party_window.png"));

    m_LeftButton = CreateWidget<CUIButton>("ChangeLButton");
    m_LeftButton->SetSize(100.f, 100.f);
    m_LeftButton->SetPos(540.f, 450.f);
    m_LeftButton->SetTexture(EButtonState::Normal, "ChangeLButton", TEXT("Button//arrow_left.png"));
    m_LeftButton->SetTexture(EButtonState::Hovered, "ChangeLButton", TEXT("Button//arrow_left.png"));
    m_LeftButton->SetTexture(EButtonState::Click, "ChangeLButton", TEXT("Button//arrow_left.png"));
    m_LeftButton->SetTexture(EButtonState::Disable, "ChangeLButton", TEXT("Button//arrow_left.png"));

    m_RightButton = CreateWidget<CUIButton>("ChangeRButton");
    m_RightButton->SetSize(100.f, 100.f);
    m_RightButton->SetPos(1280.f, 450.f);
    m_RightButton->SetTexture(EButtonState::Normal, "ChangeRButton", TEXT("Button//arrow.png"));
    m_RightButton->SetTexture(EButtonState::Hovered, "ChangeRButton", TEXT("Button//arrow.png"));
    m_RightButton->SetTexture(EButtonState::Click, "ChangeRButton", TEXT("Button//arrow.png"));
    m_RightButton->SetTexture(EButtonState::Disable, "ChangeRButton", TEXT("Button//arrow.png"));

    m_PartyDetail = CreateWidget<CUIButton>("PartyDetailButton");
    m_PartyDetail->SetSize(124.f*0.8f, 129.f * 0.8f);
    m_PartyDetail->SetPos(1080.f, 230.f);
    m_PartyDetail->SetTexture(EButtonState::Normal, "PartyDetailButton", TEXT("Button//go_party_detail.png"));
    m_PartyDetail->SetTexture(EButtonState::Hovered, "PartyDetailButton", TEXT("Button//go_party_detail.png"));
    m_PartyDetail->SetTexture(EButtonState::Click, "PartyDetailButton", TEXT("Button//go_party_detail.png"));
    m_PartyDetail->SetTexture(EButtonState::Disable, "PartyDetailButton", TEXT("Button//go_party_detail.png"));

    m_SetEdit = CreateWidget<CUIButton>("SetEditButton");
    m_SetEdit->SetSize(124.f * 0.8f, 129.f * 0.8f);
    m_SetEdit->SetPos(1180.f, 230.f);
    m_SetEdit->SetTexture(EButtonState::Normal, "SetEditButton", TEXT("Button//set_edit.png"));
    m_SetEdit->SetTexture(EButtonState::Hovered, "SetEditButton", TEXT("Button//set_edit.png"));
    m_SetEdit->SetTexture(EButtonState::Click, "SetEditButton", TEXT("Button//set_edit.png"));
    m_SetEdit->SetTexture(EButtonState::Disable, "SetEditButton", TEXT("Button//set_edit.png"));

    m_PlayButton = CreateWidget<CUIButton>("PlayButton");
    m_PlayButton->SetSize(400.f, 130.f);
    m_PlayButton->SetPos(750.f, 100.f);
    m_PlayButton->SetTexture(EButtonState::Normal, "PlayButton", TEXT("Button//play_button.png"));
    m_PlayButton->SetTexture(EButtonState::Hovered, "PlayButton", TEXT("Button//play_button.png"));
    m_PlayButton->SetTexture(EButtonState::Click, "PlayButton", TEXT("Button//play_button.png"));
    m_PlayButton->SetTexture(EButtonState::Disable, "PlayButton", TEXT("Button//play_button.png"));

    m_BackButton = CreateWidget<CUIButton>("BackButton");
    m_BackButton->SetSize(147.f, 137.f);
    m_BackButton->SetPos(536.f, 0.f);
    m_BackButton->SetTexture(EButtonState::Normal, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Hovered, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Click, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Disable, "BackButton", TEXT("Button//back.png"));

    size_t size = std::size(m_IndexImg);
    for(size_t i=0;i< size;++i)
    {
        m_IndexImg[i] = CreateWidget<CUIImage>("IndexImg_"+ std::to_string(i));
        m_IndexImg[i]->SetSize(27.f, 27.f);
        m_IndexImg[i]->SetPos(860.f+i*35.f, 280.f);
        m_IndexImg[i]->SetPivot(0.5f, 0.5f);
        m_IndexImg[i]->SetTexture("IndexImg", TEXT("UI//point.png"));
    }

    size = std::size(m_CharImg);
    CUIImage* weaponBG[3];
    CUIImage* unisonBG[3];
    for (size_t i = 0; i < size; ++i)
    {
        m_FrameCharImg[i] = CreateWidget<CUIImage>("FrameCharImg_" + std::to_string(i));
        m_FrameCharImg[i]->SetSize(186.f*0.66f,392.f * 0.66f);
        m_FrameCharImg[i]->SetPos(710.f + i * 220.f, 527.f);
        m_FrameCharImg[i]->SetPivot(0.5f, 0.5f);
        m_CharImg[i] = CreateWidget<CUIImage>("FrameImg_" + std::to_string(i));
        m_CharImg[i]->SetSize(227.f*0.6f, 451.f * 0.6f);
        m_CharImg[i]->SetPos(710.f + i * 220.f, 527.f);
        m_CharImg[i]->SetPivot(0.5f, 0.5f);
        m_CharImg[i]->SetTexture("FrameImg", TEXT("Cover//char_portrait_cover.png"));
        weaponBG[i] = CreateWidget<CUIImage>("weaponBG" + std::to_string(i));
        weaponBG[i]->SetSize(173.f * 0.6f, 173.f * 0.6f);
        weaponBG[i]->SetPos(790.f + i * 220.f, 550.f);
        weaponBG[i]->SetPivot(0.5f, 0.5f);
        weaponBG[i]->SetTexture("weaponBG", TEXT("Cover//weaponslot.png"));
        unisonBG[i] = CreateWidget<CUIImage>("unisonBG" + std::to_string(i));
        unisonBG[i]->SetSize(171.f * 0.6f, 221.f * 0.6f);
        unisonBG[i]->SetPos(789.f + i * 220.f, 424.f);
        unisonBG[i]->SetPivot(0.5f, 0.5f);
        unisonBG[i]->SetTexture("unisonBG", TEXT("Cover//subchar_portrait2.png"));
        m_UnisonImg[i] = CreateWidget<CUIImage>("UnisonCharImg" + std::to_string(i));
        m_UnisonImg[i]->SetSize(144.f * 0.66f, 188.f * 0.66f);
        m_UnisonImg[i]->SetPos(789.f + i * 220.f, 424.f);
        m_UnisonImg[i]->SetPivot(0.5f, 0.5f);
        m_FrameUnisonImg[i] = CreateWidget<CUIImage>("FrameUnisonImg_" + std::to_string(i));
        m_FrameUnisonImg[i]->SetSize(173.f * 0.6f, 221.f * 0.60f);
        m_FrameUnisonImg[i]->SetPos(789.f + i * 220.f, 424.f);
        m_FrameUnisonImg[i]->SetPivot(0.5f, 0.5f);
        m_FrameUnisonImg[i]->SetTexture("FrameUnisonImg", TEXT("Cover//unison_portrait_cover.png"));
    }

    CUIImage* readerMark = CreateWidget<CUIImage>("readerMark");
    readerMark->SetSize(123.f*0.6f, 43.f * 0.6f);
    readerMark->SetPos(673.f, 630.f);
    readerMark->SetPivot(0.5f, 0.5f);
    readerMark->SetTexture("readerMark", TEXT("Cover//leader.png"));
    return true;
}

void CPartyUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
}

void CPartyUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CPartyUI::Render()
{
    CUIWindow::Render();
}

CPartyUI* CPartyUI::Clone()
{
    return new CPartyUI(*this);
}

void CPartyUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CPartyUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_LeftButton = FindWidget<CUIButton>("ChangeLButton");
    m_RightButton = FindWidget<CUIButton>("ChangeRButton");
    m_PartyDetail = FindWidget<CUIButton>("PartyDetailButton");
    m_SetEdit = FindWidget<CUIButton>("SetEditButton");
    m_PlayButton = FindWidget<CUIButton>("PlayButton");
    m_BackButton = FindWidget<CUIButton>("BackButton");
    size_t size = std::size(m_IndexImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_IndexImg[i] = FindWidget<CUIImage>("IndexImg_" + std::to_string(i));
    }
    size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_FrameCharImg[i] = FindWidget<CUIImage>("FrameCharImg_" + std::to_string(i));
        m_CharImg[i] = FindWidget<CUIImage>("FrameImg_" + std::to_string(i));
        m_UnisonImg[i] = FindWidget<CUIImage>("UnisonCharImg" + std::to_string(i));
        m_FrameUnisonImg[i] = FindWidget<CUIImage>("FrameUnisonImg_" + std::to_string(i));
    }
}

void CPartyUI::Play()
{
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        if (bg->GetUserData().charName[0] == "none")
        {
            return;
        }
        bg->SetScore(0);
        bg->SetTime(0.f);
    }
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "ChallengeDungeon.scn"
        , m_Scene->GetSceneInfo()->GetFileName());
}

void CPartyUI::Back()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Lobby.scn"
        , m_Scene->GetSceneInfo()->GetFileName());
}

void CPartyUI::Left()
{
    if(--m_Index<0)
    {
        m_Index = 5;
    }
    size_t size = std::size(m_IndexImg);
    for (size_t i = 0; i < size; ++i)
    {
        if (i == m_Index)
        {
            m_IndexImg[i]->SetImageTint(0, 255, 255, 255);
            m_IndexImg[i]->SetSize(30.f, 30.f);
            continue;
        }
        m_IndexImg[i]->SetImageTint(255, 255, 255, 255);
        m_IndexImg[i]->SetSize(27.f, 27.f);
    }
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        bg->SetPartyIndex(m_Index);
        for (int i = (m_Index) * 6; i < (m_Index) * 6 + 3; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_CharImg[i % 3]->SetOpacity(0.f);
                m_FrameCharImg[i % 3]->SetOpacity(0.f);
            }
            else
            {
                m_CharImg[i % 3]->SetOpacity(1.f);
                m_FrameCharImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//ui//thumb_party_main_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_FrameCharImg[i % 3]->SetTexture("FrameCharImg_" + std::to_string(i) + name, t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
        for (int i = (m_Index) * 6 + 3; i < (m_Index) * 6 + 6; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_UnisonImg[i % 3]->SetOpacity(0.f);
            }
            else
            {
                m_UnisonImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//ui//thumb_party_unison_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_UnisonImg[i % 3]->SetTexture("UnisonCharImg" + std::to_string(i) + name, t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
    }
}

void CPartyUI::RIght()
{
    if (++m_Index > 5)
    {
        m_Index = 0;
    }
    size_t size = std::size(m_IndexImg);
    for (size_t i = 0; i < size; ++i)
    {
        if (i == m_Index)
        {
            m_IndexImg[i]->SetImageTint(0, 255, 255, 255);
            m_IndexImg[i]->SetSize(30.f, 30.f);
            continue;
        }
        m_IndexImg[i]->SetImageTint(255, 255, 255, 255);
        m_IndexImg[i]->SetSize(27.f, 27.f);
    }
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        bg->SetPartyIndex(m_Index);
        for (int i = (m_Index) * 6; i < (m_Index) * 6 + 3; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_CharImg[i % 3]->SetOpacity(0.f);
                m_FrameCharImg[i % 3]->SetOpacity(0.f);
            }
            else
            {
                m_CharImg[i % 3]->SetOpacity(1.f);
                m_FrameCharImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//ui//thumb_party_main_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_FrameCharImg[i % 3]->SetTexture("FrameCharImg_" + std::to_string(i) + name, t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
        for (int i = (m_Index) * 6 + 3; i < (m_Index) * 6 + 6; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_UnisonImg[i % 3]->SetOpacity(0.f);
            }
            else
            {
                m_UnisonImg[i % 3]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//ui//thumb_party_unison_1.png";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_UnisonImg[i % 3]->SetTexture("UnisonCharImg" + std::to_string(i) + name, t_filename);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
    }
}

void CPartyUI::EditParty()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "EditParty.scn"
        , m_Scene->GetSceneInfo()->GetFileName());
}
