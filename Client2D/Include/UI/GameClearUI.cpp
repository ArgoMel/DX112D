#include "GameClearUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../GameObject/BackGround.h"

CGameClearUI::CGameClearUI()
{
	m_WindowTypeName = "GameClearUI";
	m_Scene = CSceneManager::GetInst()->GetScene();
}

CGameClearUI::CGameClearUI(const CGameClearUI& window)
	: CUIWindow(window)
{
    m_MinuteText = FindWidget<CUIText>("MinuteText");
    m_SecondText = FindWidget<CUIText>("SecondText");
    m_MiliSecondText = FindWidget<CUIText>("MiliSecondText");
    m_FMinuteText = FindWidget<CUIText>("FMinuteText");
    m_FSecondText = FindWidget<CUIText>("FSecondText");
    m_FMiliSecondText = FindWidget<CUIText>("FMiliSecondText");
    m_ScoreText = FindWidget<CUIText>("ScoreText");
    m_HighScoreText = FindWidget<CUIText>("HighScoreText");
    size_t size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_UpperCharImg[i] = FindWidget<CUIImage>("UpperCharImg" + std::to_string(i));
        m_CharImg[i] = FindWidget<CUIImage>("CharImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    m_NextBtnText = FindWidget<CUIText>("NextBtnText");
    m_NextBtn = FindWidget<CUIButton>("NextBtn");
}

CGameClearUI::~CGameClearUI()
{
    m_Scene->GetResource()->SoundStop("Clear");
}

void CGameClearUI::Start()
{
    CUIWindow::Start();
    m_Scene->GetResource()->SoundPlay("Clear");
    m_NextBtn->SetSound(EButtonEventState::Click, "ButtonClick");
    m_NextBtn->SetCallback<CGameClearUI>(EButtonEventState::Click, this, &CGameClearUI::Lobby);

    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
//점수
        int score=bg->GetUserData().score;
        int highScore=bg->GetUserData().highScore;
        m_ScoreText->SetIntText(score);
        if(score> highScore)
        {
            highScore = score;
            bg->SetHighScore(highScore);
        }
        m_HighScoreText->SetIntText(highScore);
//시간
        float time = bg->GetUserData().time;
        float fastTime = bg->GetUserData().fastTime;
        int minute = (int)time / 60;
        int second = (int)time % 60;
        int mili = (int)((time - (int)time)*100);
        m_MinuteText->SetIntText(minute, true);
        m_SecondText->SetIntText(second, true);
        m_MiliSecondText->SetIntText(mili, true);
        if (time < fastTime&& time!=0.f)
        {
            fastTime = time;
            bg->SetFastTime(fastTime);
        }
        minute = (int)fastTime / 60;
        second = (int)fastTime % 60;
        mili = (int)((fastTime - (int)fastTime) * 100);
        m_FMinuteText->SetIntText(minute, true);
        m_FSecondText->SetIntText(second,true);
        m_FMiliSecondText->SetIntText(mili, true);
//파티 이미지
        int index = bg->GetUserData().partyIndex;
        size_t size = std::size(m_CharImg);
        for (int i =0; i < 6; ++i)
        {
            std::string name = bg->GetUserData().charName[i+index*6];
            if (name == "none")
            {
                m_CharImg[i]->SetOpacity(0.f);
                m_UpperCharImg[i]->SetOpacity(0.f);
            }
            else
            {
                m_CharImg[i]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//pixelart//animated//neutral.gif";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_CharImg[i]->SetTexture(name, t_filename);
                m_CharImg[i]->SetSize(m_CharImg[i]->GetTextureInfo().texture->GetWidth() * 2.f,
                    m_CharImg[i]->GetTextureInfo().texture->GetHeight() * 2.f);
                SAFE_DELETE_ARRAY(t_filename);
//애니메이션
                m_UpperCharImg[i]->SetOpacity(1.f);
                m_UpperCharImg[i]->SetSize(m_CharImg[i]->GetSize());
                m_UpperCharImg[i]->ClearFrame();
                std::vector<const TCHAR*>   vecFileName;
                for (int j = 0; j < 4; ++j)
                {
                    std::string path = "Character//" + name 
                        + "//pixelart//sprite_sheet//kachidoki"+ std::to_string(j) +".png";
                    TCHAR* filename = new TCHAR[path.size() + 1];
                    filename[path.size()] = 0;
                    std::copy(path.begin(), path.end(), filename);
                    vecFileName.push_back(filename);
                    m_UpperCharImg[i]->AddFrameData(Vector2(0.f, 0.f), Vector2(26.f, 32.f));
                }
                m_UpperCharImg[i]->SetTexture("Win_" + name, vecFileName);
                m_UpperCharImg[i]->SetPlayTime(10.f);
                for (int j = 0; j < 4; ++j)
                {
                    SAFE_DELETE_ARRAY(vecFileName[j]);
                }
                vecFileName.clear();
            }
        }
    }
}

bool CGameClearUI::Init()
{
    CUIWindow::Init();
    m_MinuteText = CreateWidget<CUIText>("MinuteText");
    m_MinuteText->SetPos(885.f, 775.f);
    m_MinuteText->SetSize(50.f, 100.f);
    m_MinuteText->SetPivot(0.5f, 0.5f);
    m_MinuteText->SetFontSize(40.f);
    m_MinuteText->SetText(TEXT("00"));
    m_MinuteText->SetColor(0,0,0);
    m_MinuteText->SetAlignH(Text_Align_H::Right);

    m_SecondText = CreateWidget<CUIText>("SecondText");
    m_SecondText->SetPos(950.f, 775.f);
    m_SecondText->SetSize(50.f, 100.f);
    m_SecondText->SetPivot(0.5f, 0.5f);
    m_SecondText->SetFontSize(40.f);
    m_SecondText->SetText(TEXT("00"));
    m_SecondText->SetColor(0, 0, 0);
    m_SecondText->SetAlignH(Text_Align_H::Right);

    m_MiliSecondText = CreateWidget<CUIText>("MiliSecondText");
    m_MiliSecondText->SetPos(1005.f, 775.f);
    m_MiliSecondText->SetSize(50.f, 100.f);
    m_MiliSecondText->SetPivot(0.5f, 0.5f);
    m_MiliSecondText->SetFontSize(40.f);
    m_MiliSecondText->SetText(TEXT("00"));
    m_MiliSecondText->SetColor(0, 0, 0);
    m_MiliSecondText->SetAlignH(Text_Align_H::Right);

    m_FMinuteText = CreateWidget<CUIText>("FMinuteText");
    m_FMinuteText->SetPos(885.f, 515.f);
    m_FMinuteText->SetSize(50.f, 100.f);
    m_FMinuteText->SetPivot(0.5f, 0.5f);
    m_FMinuteText->SetFontSize(40.f);
    m_FMinuteText->SetText(TEXT("00"));
    m_FMinuteText->SetColor(250, 100, 0);
    m_FMinuteText->SetAlignH(Text_Align_H::Right);

    m_FSecondText = CreateWidget<CUIText>("FSecondText");
    m_FSecondText->SetPos(950.f, 515.f);
    m_FSecondText->SetSize(50.f, 100.f);
    m_FSecondText->SetPivot(0.5f, 0.5f);
    m_FSecondText->SetFontSize(40.f);
    m_FSecondText->SetText(TEXT("00"));
    m_FSecondText->SetColor(250, 100, 0);
    m_FSecondText->SetAlignH(Text_Align_H::Right);

    m_FMiliSecondText = CreateWidget<CUIText>("FMiliSecondText");
    m_FMiliSecondText->SetPos(1005.f, 515.f);
    m_FMiliSecondText->SetSize(50.f, 100.f);
    m_FMiliSecondText->SetPivot(0.5f, 0.5f);
    m_FMiliSecondText->SetFontSize(40.f);
    m_FMiliSecondText->SetText(TEXT("00"));
    m_FMiliSecondText->SetColor(250, 100, 0);
    m_FMiliSecondText->SetAlignH(Text_Align_H::Right);

    m_ScoreText = CreateWidget<CUIText>("ScoreText");
    m_ScoreText->SetPos(900.f, 726.f);
    m_ScoreText->SetSize(150.f, 100.f);
    m_ScoreText->SetPivot(0.5f, 0.5f);
    m_ScoreText->SetFontSize(25.f);
    m_ScoreText->SetText(TEXT("0"));
    m_ScoreText->SetColor(0, 0, 0);
    m_ScoreText->SetAlignH(Text_Align_H::Right);

    m_HighScoreText = CreateWidget<CUIText>("HighScoreText");
    m_HighScoreText->SetPos(900.f, 464.f);
    m_HighScoreText->SetSize(150.f, 100.f);
    m_HighScoreText->SetPivot(0.5f, 0.5f);
    m_HighScoreText->SetFontSize(25.f);
    m_HighScoreText->SetText(TEXT("0"));
    m_HighScoreText->SetColor(0, 0, 0);
    m_HighScoreText->SetAlignH(Text_Align_H::Right);

    size_t size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = CreateWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_MainStatueImg[i]->SetSize(165.f * 0.5f, 155.f * 0.5f);
        m_MainStatueImg[i]->SetPos(750.f + i * 180.f, 270.f);
        m_MainStatueImg[i]->SetPivot(0.5f, 0.5f);
        m_MainStatueImg[i]->SetTexture("MainStatueImg", TEXT("UI//mainStatue_sprite.png"));
        m_UnisonStatueImg[i] = CreateWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i]->SetSize(169.f * 0.5f, 117.f * 0.5f);
        m_UnisonStatueImg[i]->SetPos(808.f + i * 180.f, 253.f);
        m_UnisonStatueImg[i]->SetPivot(0.5f, 0.5f);
        m_UnisonStatueImg[i]->SetTexture("UnisonStatueImg", TEXT("UI//unisonStatue_sprite.png"));
    }
    size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_CharImg[i] = CreateWidget<CUIImage>("CharImg" + std::to_string(i));
        if (i < 3)
        {
            m_CharImg[i]->SetPos(752.f + i * 180.f, 315.f);
        }
        else
        {
            m_CharImg[i]->SetPos(270.f + i * 180.f, 285.f);
        }
        m_CharImg[i]->SetPivot(0.5f, 0.5f);
    }
    for (int i = (int)size - 1; i >= 0; --i)
    {
        m_UpperCharImg[i] = CreateWidget<CUIImage>("UpperCharImg" + std::to_string(i));
        float sign = i % 2 == 0 ? -1.f : 1.f;
        if (i % 3 == 0)
        {
            m_UpperCharImg[i]->SetPos(1145.f, 550.f + i * 20.f);
        }
        else
        {
            m_UpperCharImg[i]->SetPos(1145.f + sign * 40.f, 550.f + i * 20.f);
        }
        m_UpperCharImg[i]->SetPivot(0.5f, 0.5f);
    }

    m_NextBtn = CreateWidget<CUIButton>("NextBtn");
    m_NextBtn->SetPos(843.f, 19.f);
    m_NextBtn->SetSize(237.f, 79.f);
    m_NextBtn->SetTexture(EButtonState::Normal, "NextBtn", TEXT("Button//btn_blue.png"));
    m_NextBtn->SetTexture(EButtonState::Hovered, "NextBtn", TEXT("Button//btn_blue.png"));
    m_NextBtn->SetTexture(EButtonState::Click, "NextBtn", TEXT("Button//btn_blue.png"));
    m_NextBtn->SetTexture(EButtonState::Disable, "NextBtn", TEXT("Button//btn_blue.png"));

    m_NextBtnText = CreateWidget<CUIText>("NextBtnText");
    m_NextBtnText->SetPos(910.f, -20.f);
    m_NextBtnText->SetSize(100.f, 100.f);
    m_NextBtnText->SetPivot(0.5f, 0.5f);
    m_NextBtnText->SetFontSize(25.f);
    m_NextBtnText->SetText(TEXT("다음"));
    m_NextBtnText->SetAlignH(Text_Align_H::Center);
	return true;
}

void CGameClearUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
}

void CGameClearUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CGameClearUI::Render()
{
    CUIWindow::Render();
}

CGameClearUI* CGameClearUI::Clone()
{
    return new CGameClearUI(*this);
}

void CGameClearUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CGameClearUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_MinuteText = FindWidget<CUIText>("MinuteText");
    m_SecondText = FindWidget<CUIText>("SecondText");
    m_MiliSecondText = FindWidget<CUIText>("MiliSecondText");
    m_FMinuteText = FindWidget<CUIText>("FMinuteText");
    m_FSecondText = FindWidget<CUIText>("FSecondText");
    m_FMiliSecondText = FindWidget<CUIText>("FMiliSecondText");
    m_ScoreText = FindWidget<CUIText>("ScoreText");
    m_HighScoreText = FindWidget<CUIText>("HighScoreText");
    size_t size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_UpperCharImg[i] = FindWidget<CUIImage>("UpperCharImg" + std::to_string(i));
        m_CharImg[i] = FindWidget<CUIImage>("CharImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    m_NextBtnText = FindWidget<CUIText>("NextBtnText");
    m_NextBtn = FindWidget<CUIButton>("NextBtn");
}

void CGameClearUI::Lobby()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Lobby.scn");
}
