#include "StageUI.h"
#include "Input.h"
#include "Engine.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../GameObject/BackGround.h"

CStageUI::CStageUI()
    : m_Time(0.f)
    , m_PlayTime(0.f)
    , m_DarkTime(0.f)
    , m_FadeTime(-1.f)
    , m_SkillBarIndex(-1)
    , m_TotalTime(1800)
    , m_IsGaming(false)
    , m_IsLarger(false)
    , m_IsRetryUI(false)
    , m_IsSkillOn(false)
{
	m_WindowTypeName = "StageUI";
	m_Scene = CSceneManager::GetInst()->GetScene();
}

CStageUI::CStageUI(const CStageUI& window)
	: CUIWindow(window)
	, m_Time(0.f)
    , m_PlayTime(0.f)
	, m_DarkTime(0.f)
    , m_FadeTime(-1.f)
    , m_SkillBarIndex(-1)
	, m_TotalTime(window.m_TotalTime)
	, m_IsGaming(window.m_IsGaming)
	, m_IsLarger(window.m_IsLarger)
    , m_IsRetryUI(false)
{
    m_BlackImg = FindWidget<CUIImage>("BlackImg");
    m_SecondText = FindWidget<CUIText>("SecondText");
    m_MinuteText = FindWidget<CUIText>("MinuteText");
    m_ScoreText = FindWidget<CUIText>("ScoreText");
    m_ComboText = FindWidget<CUIText>("ComboText");
    m_FeverImg = FindWidget<CUIImage>("FeverImg");
    m_FeverBar = FindWidget<CUIProgressBar>("FeverBar");
    size_t size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_CharImg[i] = FindWidget<CUIImage>("CharImg" + std::to_string(i));
        m_CharCover[i] = FindWidget<CUIImage>("CharCover" + std::to_string(i));
        m_HpBar[i] = FindWidget<CUIProgressBar>("HpBar" + std::to_string(i));
        m_HpText[i] = FindWidget<CUIText>("HpText" + std::to_string(i));
        m_SkillBar[i] = FindWidget<CUIImage>("SkillBar" + std::to_string(i));
        m_SkillChargeBar[i] = FindWidget<CUIProgressBar>("SkillChargeBar" + std::to_string(i));
    }
    m_SkillCurtainDark = FindWidget<CUIImage>("SkillCurtainDark");
    m_SkillCurtain = FindWidget<CUIImage>("SkillCurtain");
    m_BlackBG[0] = FindWidget<CUIImage>("blackBGL");
    m_BlackBG[1] = FindWidget<CUIImage>("blackBGR");
    m_MousePosImg = FindWidget<CUIImage>("MousePosImg");
    m_CharDirMarker = FindWidget<CUIImage>("CharDirMarker");
    m_GameOverImg = FindWidget<CUIImage>("GameOverImg");
    m_RetryBG = FindWidget<CUIImage>("RetryBG");
    m_RetryText = FindWidget<CUIText>("RetryText");
    m_RetrySemiText = FindWidget<CUIText>("RetrySemiText");
    m_ForgiveBtn = FindWidget<CUIButton>("ForgiveBtn");
    m_RetryBtn = FindWidget<CUIButton>("RetryBtn");
    m_ForgiveBtnText = FindWidget<CUIText>("ForgiveBtnText");
    m_RetryBtnText = FindWidget<CUIText>("RetryBtnText");
}

CStageUI::~CStageUI()
{
    int score = _wtoi(m_ScoreText->GetText());
    if (m_BG)
    {
        m_BG->AddScore(score);
        m_BG->AddTime(m_PlayTime);
    }
}

void CStageUI::Start()
{
    CUIWindow::Start();
    m_ForgiveBtn->SetSound(EButtonEventState::Click, "ButtonClick");
    m_RetryBtn->SetSound(EButtonEventState::Click, "ButtonClick2");

    m_BG = (CBackGround*)m_Scene->FindObject("Background");
    if (m_BG)
    {
        int index = m_BG->GetUserData().partyIndex;
        int size = (int)std::size(m_CharImg);
        for (int i = index * 6; i < index* 6+size; ++i)
        {
            std::string name = m_BG->GetUserData().charName[i];
            if(name=="none")
            {
                m_CharImg[i % 3]->SetEnable(false);
                m_CharCover[i % 3]->SetEnable(false);
                m_HpBar[i % 3]->SetEnable(false);
                m_HpText[i % 3]->SetEnable(false);
                m_SkillBar[i%3]->SetEnable(false);
                continue;
            }
            std::string imgPath = "Character//" + name + "//ui//battle_member_status_1.png";
            TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
            t_filename[imgPath.size()] = 0;
            std::copy(imgPath.begin(), imgPath.end(), t_filename);
            m_CharImg[i % 3]->SetTexture("CharImg_"+m_CharImg[i % 3]->GetName(), t_filename);
            imgPath = "Character//" + name + "//ui//battle_control_board_1.png";
            t_filename[imgPath.size()] = 0;
            std::copy(imgPath.begin(), imgPath.end(), t_filename);
            m_SkillBar[i]->SetTexture("SkillBar_" + name, t_filename);
            SAFE_DELETE_ARRAY(t_filename);
            imgPath = "Character//" + name + "//voice//battle//skill_0.mp3";
            m_Scene->GetResource()->LoadSound("Voice", "Skill_" + name + "0", false, imgPath.c_str(), TEXTURE_PATH);
            imgPath = "Character//" + name + "//voice//battle//skill_1.mp3";
            m_Scene->GetResource()->LoadSound("Voice", "Skill_" + name + "1", false, imgPath.c_str(), TEXTURE_PATH);
        }
        AddScore(m_BG->GetUserData().score);
    }
    m_ForgiveBtn->SetCallback<CStageUI>(EButtonEventState::Click, this, &CStageUI::Back);
    m_RetryBtn->SetCallback<CStageUI>(EButtonEventState::Click, this, &CStageUI::Retry);

    CInput::GetInst()->AddBindFunction<CStageUI>("Pause", Input_Type::Down, this, &CStageUI::Pause, m_Scene);
    CInput::GetInst()->AddBindFunction<CStageUI>("Skill", Input_Type::Stay, this, &CStageUI::DragSkill, m_Scene);
}

bool CStageUI::Init()
{
    CUIWindow::Init();
    m_BlackImg = CreateWidget<CUIImage>("BlackImg");
    m_BlackImg->SetSize(670.f*0.6f, 59.f * 0.6f);
    m_BlackImg->SetPos(750.f , 40.f);
    m_BlackImg->SetPivot(0.5f, 0.5f);
    m_BlackImg->SetTexture("BlackImg", TEXT("UI//Time_Score.png"));
    //1800초 시작
    m_MinuteText = CreateWidget<CUIText>("MinuteText");
    m_MinuteText->SetPos(595.f, -45.f);
    m_MinuteText->SetSize(50.f, 100.f);
    m_MinuteText->SetPivot(0.5f, 0.5f);
    m_MinuteText->SetFontSize(25.f);
    m_MinuteText->SetText(TEXT("30"));
    m_MinuteText->SetAlignH(Text_Align_H::Right);

    m_SecondText = CreateWidget<CUIText>("SecondText");
    m_SecondText->SetPos(640.f, -45.f);
    m_SecondText->SetSize(50.f, 100.f);
    m_SecondText->SetPivot(0.5f, 0.5f);
    m_SecondText->SetFontSize(25.f);
    m_SecondText->SetText(TEXT("00"));
    m_SecondText->SetAlignH(Text_Align_H::Right);

    m_ScoreText = CreateWidget<CUIText>("ScoreText");
    m_ScoreText->SetPos(770.f, -45.f);
    m_ScoreText->SetSize(150.f, 100.f);
    m_ScoreText->SetPivot(0.5f, 0.5f);
    m_ScoreText->SetFontSize(25.f);
    m_ScoreText->SetText(TEXT("0"));
    m_ScoreText->SetAlignH(Text_Align_H::Right);

    m_ComboText = CreateWidget<CUIText>("ComboText");
    m_ComboText->SetPos(840.f, 80.f);
    m_ComboText->SetSize(250.f, 100.f);
    m_ComboText->SetPivot(0.5f, 0.5f);
    m_ComboText->SetFontSize(45.f);
    m_ComboText->SetText(TEXT("0 Combo"));
    m_ComboText->SetColor(255,222,0);
    m_ComboText->SetAlignH(Text_Align_H::Center);
    m_ComboText->SetEnable(false);

    m_FeverBar = CreateWidget<CUIProgressBar>("FeverBar");
    m_FeverBar->SetPos(960.f, 375.f);
    m_FeverBar->SetSize(240.f, 18.f);
    m_FeverBar->SetImageTint(EProgressBarTextureType::Back, 255, 255, 255, 0);
    m_FeverBar->SetTexture(EProgressBarTextureType::Bar, "FeverBar", TEXT("Map\\fevergauge.png"));
    m_FeverBar->SetProgressBarMin(0.f);
    m_FeverBar->SetProgressBarMax(100.f);
    m_FeverBar->SetValue(0.f);
    m_FeverBar->SetBarDir(EProgressBarDir::RightToLeft);
    m_FeverBar->SetPivot(0.5f, 0.5f);

    m_FeverImg = CreateWidget<CUIImage>("FeverImg");
    m_FeverImg->SetPos(960.f, 380.f);
    m_FeverImg->SetSize(276.f, 87.f);
    m_FeverImg->SetPivot(0.5f, 0.5f);
    m_FeverImg->SetTexture("FeverImg", TEXT("Map\\feverbar.png"));

    int size = (int)std::size(m_CharImg);
    for (int i = 0; i < size; ++i)
    {
        m_HpBar[i] = CreateWidget<CUIProgressBar>("HpBar" + std::to_string(i));
        m_HpBar[i]->SetPos(711.f + i * 250.f, 1020.f);
        m_HpBar[i]->SetSize(150.f, 25.f);
        m_HpBar[i]->SetImageTint(EProgressBarTextureType::Back, 255, 255, 255, 0);
        m_HpBar[i]->SetImageTint(EProgressBarTextureType::Bar, 0, 255, 50, 255);
        m_HpBar[i]->SetTexture(EProgressBarTextureType::Bar, "LoagindBar", TEXT("Frame\\loadingGauge.png"));
        m_HpBar[i]->SetProgressBarMin(0.f);
        m_HpBar[i]->SetProgressBarMax(100.f);
        m_HpBar[i]->SetValue(100.f);
        m_HpBar[i]->SetBarDir(EProgressBarDir::RightToLeft);
        m_HpBar[i]->SetPivot(0.5f, 0.5f);
        m_CharImg[i] = CreateWidget<CUIImage>("CharImg" + std::to_string(i));
        m_CharImg[i]->SetSize(58.f*0.9f,58.f * 0.9f);
        m_CharImg[i]->SetPos(610.f + i * 250.f, 1020.f);
        m_CharImg[i]->SetPivot(0.5f, 0.5f);
        m_CharCover[i] = CreateWidget<CUIImage>("CharCover" + std::to_string(i));
        m_CharCover[i]->SetSize(258.f * 0.9f, 60.f * 0.9f);
        m_CharCover[i]->SetPos(700.f + i * 250.f, 1020.f);
        m_CharCover[i]->SetPivot(0.5f, 0.5f);
        m_CharCover[i]->SetTexture("CharCover", TEXT("Cover\\ingame_cover.png"));
        m_HpText[i] = CreateWidget<CUIText>("HpText" + std::to_string(i));
        m_HpText[i]->SetPos(695.f + i * 250.f, 971.f);
        m_HpText[i]->SetSize(100.f, 100.f);
        m_HpText[i]->SetPivot(0.5f, 0.5f);
        m_HpText[i]->SetFontSize(25.f);
        m_HpText[i]->SetText(TEXT("100.0%"));
        m_HpText[i]->SetAlignH(Text_Align_H::Center);
        m_HpText[i]->SetAlignV(Text_Align_V::Middle);
        m_HpText[i]->SetShadowEnable(true);
        m_HpText[i]->SetShadowColor(0, 0, 0);
        m_SkillBar[i] = CreateWidget<CUIImage>("SkillBar" + std::to_string(i));
		m_SkillBar[i]->SetSize(104.f, 268.f);
		m_SkillBar[i]->SetAngle(60.f * (1 - i));
		m_SkillBar[i]->SetPivot(0.5f, 0.f);
        m_SkillBar[i]->SetOpacity(0.f);
        m_SkillChargeBar[i] = CreateWidget<CUIProgressBar>("SkillChargeBar" + std::to_string(i));
        m_SkillChargeBar[i]->SetPos(933.f + i * 27.f, 222.f);
        if(i!=1)
        {
            m_SkillChargeBar[i]->AddPos(0.f,-20.f);
        }
        m_SkillChargeBar[i]->SetSize(50.f, 88.f);
        m_SkillChargeBar[i]->SetImageTint(EProgressBarTextureType::Back, 255, 255, 255, 0);
        m_SkillChargeBar[i]->SetTexture(EProgressBarTextureType::Bar, "SkillChargeBar", TEXT("UI\\skill_bar.png"));
        m_SkillChargeBar[i]->SetProgressBarMin(0.f);
        m_SkillChargeBar[i]->SetProgressBarMax(100.f);
        m_SkillChargeBar[i]->SetBarDir(EProgressBarDir::TopToBottom);
        m_SkillChargeBar[i]->SetPivot(0.5f, 0.f);
        m_SkillChargeBar[i]->SetAngle((i-1)*-73.f);
        m_SkillChargeBar[i]->SetOpacity(0.8f);
    }

    m_SkillCurtainDark = CreateWidget<CUIImage>("SkillCurtainDark");
    m_SkillCurtainDark->SetPos(960.f, 540.f);
    m_SkillCurtainDark->SetSize(850.f, 1080.f);
    m_SkillCurtainDark->SetPivot(0.5f, 0.5f);
    m_SkillCurtainDark->SetTexture("blackBG", TEXT("Cover//black_square.png"));
    m_SkillCurtainDark->SetOpacity(0.f);

    m_SkillCurtain = CreateWidget<CUIImage>("SkillCurtain");
    m_SkillCurtain->SetSize(768.f,378.f);
    m_SkillCurtain->SetPos(300.f, 250.f);
    m_SkillCurtain->SetPivot(0.5f, 0.5f);
    m_SkillCurtain->SetOpacity(0.f);
    m_SkillCurtain->AddAngle(-30.f);
    m_SkillCurtain->SetTexture("blackBG", TEXT("Cover//black_square.png"));

    m_BlackBG[0] = CreateWidget<CUIImage>("blackBGL");
    m_BlackBG[0]->SetPos(267.5f, 540.f);
    m_BlackBG[0]->SetSize(535.f, 1080.f);
    m_BlackBG[0]->SetPivot(0.5f, 0.5f);
    m_BlackBG[0]->SetTexture("blackBG", TEXT("Cover//black_square.png"));
    m_BlackBG[1] = CreateWidget<CUIImage>("blackBGR");
    m_BlackBG[1]->SetPos(1646.5f, 540.f);
    m_BlackBG[1]->SetSize(523.f, 1080.f);
    m_BlackBG[1]->SetPivot(0.5f, 0.5f);
    m_BlackBG[1]->SetTexture("blackBG", TEXT("Cover//black_square.png"));

    m_MousePosImg = CreateWidget<CUIImage>("MousePosImg");
    m_MousePosImg->SetSize(27.f, 27.f);
    m_MousePosImg->SetPivot(0.5f, 0.5f);
    m_MousePosImg->SetOpacity(0.f);
    m_MousePosImg->SetTexture("MousePosImg", TEXT("UI\\point.png"));
    m_CharDirMarker = CreateWidget<CUIImage>("CharDirMarker");
    m_CharDirMarker->SetSize(192.f, 9.f);
    m_CharDirMarker->SetPivot(0.75f, 0.5f);
    m_CharDirMarker->SetOpacity(0.f);
    m_CharDirMarker->SetTexture("dirMarker", TEXT("Effect\\line.png"));
//RetryUI
    m_GameOverImg = CreateWidget<CUIImage>("GameOverImg");
    m_GameOverImg->SetPos(960.f, 700.f);
    m_GameOverImg->SetSize(970.f*0.6f, 110.f * 0.6f);
    m_GameOverImg->SetPivot(0.5f, 0.5f);
    m_GameOverImg->SetOpacity(0.f);
    m_GameOverImg->SetTexture("GameOverImg", TEXT("UI\\game_over.png"));
    m_RetryBG = CreateWidget<CUIImage>("RetryBG");
    m_RetryBG->SetPos(960.f, 300.f);
    m_RetryBG->SetSize(850.f, 400.f);
    m_RetryBG->SetPivot(0.5f, 0.5f);
    m_RetryBG->SetOpacity(0.f);
    m_RetryBG->SetTexture("blackBG", TEXT("Cover//black_square.png"));
    m_RetryText = CreateWidget<CUIText>("RetryText");
    m_RetryText->SetPos(840.f, 350.f);
    m_RetryText->SetSize(250.f, 100.f);
    m_RetryText->SetPivot(0.5f, 0.5f);
    m_RetryText->SetFontSize(25.f);
    m_RetryText->SetText(TEXT("계속하시겠습니까?"));
    m_RetryText->SetAlignH(Text_Align_H::Center);
    m_RetryText->SetTransparency(true);
    m_RetryText->SetOpacity(0.f);
    m_RetrySemiText = CreateWidget<CUIText>("RetrySemiText");
    m_RetrySemiText->SetPos(840.f, 80.f);
    m_RetrySemiText->SetSize(270.f, 300.f);
    m_RetrySemiText->SetPivot(0.5f, 0.5f);
    m_RetrySemiText->SetFontSize(20.f);
    m_RetrySemiText->SetText(TEXT(" 성도석 x 50 를 사용합니다  캐릭터 전원의 HP가 회복하고 스킬 발동이 가능해집니다"));
    m_RetrySemiText->SetAlignH(Text_Align_H::Center);
    m_RetrySemiText->SetTransparency(true);
    m_RetrySemiText->SetOpacity(0.f);
    m_ForgiveBtn = CreateWidget<CUIButton>("ForgiveBtn");
    m_ForgiveBtn->SetPos(660.f, 120.f);
    m_ForgiveBtn->SetSize(250.f, 100.f);
    m_ForgiveBtn->SetTexture(EButtonState::Normal, "ForgiveBtn" , TEXT("Button//btn_red.png"));
    m_ForgiveBtn->SetTexture(EButtonState::Hovered, "ForgiveBtn" , TEXT("Button//btn_red.png"));
    m_ForgiveBtn->SetTexture(EButtonState::Click, "ForgiveBtn" , TEXT("Button//btn_red.png"));
    m_ForgiveBtn->SetTexture(EButtonState::Disable, "ForgiveBtn" , TEXT("Button//btn_red.png"));
    m_ForgiveBtn->SetOpacity(0.f);
    m_ForgiveBtn->SetEnable(false);
    m_RetryBtn = CreateWidget<CUIButton>("RetryBtn");
    m_RetryBtn->SetPos(1000.f, 120.f);
    m_RetryBtn->SetSize(250.f, 100.f);
    m_RetryBtn->SetTexture(EButtonState::Normal, "RetryBtn", TEXT("Button//btn_blue.png"));
    m_RetryBtn->SetTexture(EButtonState::Hovered, "RetryBtn", TEXT("Button//btn_blue.png"));
    m_RetryBtn->SetTexture(EButtonState::Click, "RetryBtn", TEXT("Button//btn_blue.png"));
    m_RetryBtn->SetTexture(EButtonState::Disable, "RetryBtn", TEXT("Button//btn_blue.png"));
    m_RetryBtn->SetOpacity(0.f);
    m_RetryBtn->SetEnable(false);
    m_ForgiveBtnText = CreateWidget<CUIText>("ForgiveBtnText");
    m_ForgiveBtnText->SetPos(735.f, 90.f);
    m_ForgiveBtnText->SetSize(100.f, 100.f);
    m_ForgiveBtnText->SetPivot(0.5f, 0.5f);
    m_ForgiveBtnText->SetFontSize(25.f);
    m_ForgiveBtnText->SetText(TEXT("포기하기"));
    m_ForgiveBtnText->SetAlignH(Text_Align_H::Center);
    m_ForgiveBtnText->SetTransparency(true);
    m_ForgiveBtnText->SetOpacity(0.f);
    m_RetryBtnText = CreateWidget<CUIText>("RetryBtnText");
    m_RetryBtnText->SetPos(1075.f, 90.f);
    m_RetryBtnText->SetSize(100.f, 100.f);
    m_RetryBtnText->SetPivot(0.5f, 0.5f);
    m_RetryBtnText->SetFontSize(25.f);
    m_RetryBtnText->SetText(TEXT("계속하기"));
    m_RetryBtnText->SetAlignH(Text_Align_H::Center);
    m_RetryBtnText->SetTransparency(true);
    m_RetryBtnText->SetOpacity(0.f);
//
    return true;
}

void CStageUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
    if(m_IsRetryUI)
    {
        if (m_FadeTime > 2.f)
        {
            return;
        }
        m_GameOverImg->SetOpacity(m_FadeTime*0.5f);
        m_RetryBG->SetOpacity(m_FadeTime*0.4f);
        m_ForgiveBtn->SetOpacity(m_FadeTime * 0.5f);
        m_RetryBtn->SetOpacity(m_FadeTime * 0.5f);
        m_RetryText->SetOpacity(m_FadeTime * 0.5f);
        m_RetrySemiText->SetOpacity(m_FadeTime * 0.5f);
        m_ForgiveBtnText->SetOpacity(m_FadeTime * 0.5f);
        m_RetryBtnText->SetOpacity(m_FadeTime * 0.5f);
    }
    if(m_FadeTime>=0.f)
    {
        m_FadeTime += deltaTime;
        if (m_FadeTime > 1.f)
        {
            m_SkillCurtainDark->AddOpacity(0.75f * deltaTime);
        }
        return;
    }
    if(m_IsGaming)
    {
        m_Time += deltaTime;
        if (m_Time >= 1.f)
        {
            m_TotalTime -= 1;
            int minute = m_TotalTime / 60;
            int second = m_TotalTime % 60;
            TCHAR	number[MAX_PATH] = {};
            if(minute<10)
            {
                wsprintf(number, TEXT("0%d"), minute);
            }
            else
            {
                wsprintf(number, TEXT("%d"), minute);
            }
            m_MinuteText->SetText(number);
            if (second < 10)
            {
                wsprintf(number, TEXT("0%d"), second);
            }
            else
            {
                wsprintf(number, TEXT("%d"), second);
            }
            m_SecondText->SetText(number);
            m_PlayTime += m_Time;
            m_Time = 0.f;
        }
    }
    if(m_SkillBarIndex!=-1)
    {
        m_DarkTime += deltaTime;
        if(m_SkillCurtain->GetPos().x>1200.f)
        {
            m_SkillCurtain->AddPos(50.f*deltaTime, 0);
        }
        else 
        {
            m_SkillCurtain->AddPos(3000.f * deltaTime, 0);
        }
        if (m_DarkTime > 1.f)
        {
            m_SkillBarIndex = -1;
            m_DarkTime = 0;
            m_SkillCurtain->SetPos(300.f, 200.f);
            m_SkillCurtain->SetOpacity(0.f);
            m_SkillCurtainDark->SetOpacity(0.f);
        }
    }
}

void CStageUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CStageUI::Render()
{
    CUIWindow::Render();
}

CStageUI* CStageUI::Clone()
{
    return new CStageUI(*this);
}

void CStageUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CStageUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_BlackImg = FindWidget<CUIImage>("BlackImg");
    m_SecondText = FindWidget<CUIText>("SecondText");
    m_MinuteText = FindWidget<CUIText>("MinuteText");
    m_ScoreText = FindWidget<CUIText>("ScoreText");
    m_ComboText = FindWidget<CUIText>("ComboText");
    m_FeverImg = FindWidget<CUIImage>("FeverImg");
    m_FeverBar = FindWidget<CUIProgressBar>("FeverBar");
    size_t size = std::size(m_CharImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_CharImg[i] = FindWidget<CUIImage>("CharImg" + std::to_string(i));
        m_CharCover[i] = FindWidget<CUIImage>("CharCover" + std::to_string(i));
        m_HpBar[i] = FindWidget<CUIProgressBar>("HpBar" + std::to_string(i));
        m_HpText[i] = FindWidget<CUIText>("HpText" + std::to_string(i));
        m_SkillBar[i] = FindWidget<CUIImage>("SkillBar" + std::to_string(i));
        m_SkillChargeBar[i] = FindWidget<CUIProgressBar>("SkillChargeBar" + std::to_string(i));
    }
    m_SkillCurtainDark = FindWidget<CUIImage>("SkillCurtainDark");
    m_SkillCurtain = FindWidget<CUIImage>("SkillCurtain");
    m_BlackBG[0] = FindWidget<CUIImage>("blackBGL");
    m_BlackBG[1] = FindWidget<CUIImage>("blackBGR");
    m_MousePosImg = FindWidget<CUIImage>("MousePosImg");
    m_CharDirMarker = FindWidget<CUIImage>("CharDirMarker");
    m_GameOverImg = FindWidget<CUIImage>("GameOverImg");
    m_RetryBG = FindWidget<CUIImage>("RetryBG");
    m_RetryText = FindWidget<CUIText>("RetryText");
    m_RetrySemiText = FindWidget<CUIText>("RetrySemiText");
    m_ForgiveBtn = FindWidget<CUIButton>("ForgiveBtn");
    m_RetryBtn = FindWidget<CUIButton>("RetryBtn");
    m_ForgiveBtnText = FindWidget<CUIText>("ForgiveBtnText");
    m_RetryBtnText = FindWidget<CUIText>("RetryBtnText");
}

void CStageUI::RetryUI()
{
    m_Scene->GetResource()->SoundPlay("PlayerDie");
    m_FadeTime = 0.f;
    m_IsRetryUI = true;
    m_RetryBtn->SetEnable(true);
    m_ForgiveBtn->SetEnable(true);
}

void CStageUI::Damaged(int index, float value)
{
    if (value < 0)
    {
        value = 0.f;
    }
    m_HpBar[index]->SetValue(value);
    if(value<60.f)
    {
        m_HpBar[index]->SetImageTint(EProgressBarTextureType::Bar, 100, 255, 0, 255);
    }
    else if(value<25.f)
    {
        m_HpBar[index]->SetImageTint(EProgressBarTextureType::Bar, 0, 0, 255, 255);
    }
    TCHAR	number[MAX_PATH] = {};
    swprintf_s(number, TEXT("%.1f%%"), value);
    m_HpText[index]->SetText(number);
}

void CStageUI::SetSkillBar(int index, float gauge, int wait)
{
    m_Scene->GetResource()->SoundPlay("ActivateSkill");
    m_SkillBarIndex = index;
    m_SkillChargeBar[index]->SetValue(gauge / wait);
    if (m_Scene->GetSceneInfo()->GetPlayerObject()->GetWorldPos().y > 500.f)
    {
        m_SkillCurtain->SetPos(300.f, 250.f);
    }
    else
    {
        m_SkillCurtain->SetPos(300.f, 800.f);
    }
    if (m_BG)
    {
        int index = m_BG->GetUserData().partyIndex;
        std::string name = m_BG->GetUserData().charName[index * 6 + m_SkillBarIndex];
        int ranNum = rand() % 2;
        m_Scene->GetResource()->SoundPlay("Skill_" + name+ std::to_string(ranNum));
        std::string imgPath = "Character//" + name + "//ui//skill_cutin_1.png";
        TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
        t_filename[imgPath.size()] = 0;
        std::copy(imgPath.begin(), imgPath.end(), t_filename);
        m_SkillCurtain->SetTexture("SkillCurtain_" + name, t_filename);
        m_SkillCurtain->SetOpacity(1.f);
        m_SkillCurtainDark->SetOpacity(0.5f);
        SAFE_DELETE_ARRAY(t_filename);
    }
}

void CStageUI::TouchWindow()
{
    m_IsGaming = true;
    Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
    m_MousePosImg->SetOpacity(0.85f);
    m_MousePosImg->SetPos(mouseWorldPos);
    m_CharDirMarker->SetOpacity(0.85f);
    m_CharDirMarker->SetPos(mouseWorldPos);
}

void CStageUI::HoldWindow(float angle, float distance)
{
    if(m_IsLarger)
    {
        m_MousePosImg->AddSize(-30*g_DeltaTime, -30 * g_DeltaTime);
        m_IsLarger = m_MousePosImg->GetSize().x < 25.f ? false : true;
    }
    else
    {
        m_MousePosImg->AddSize(30 * g_DeltaTime, 30 * g_DeltaTime);
        m_IsLarger = m_MousePosImg->GetSize().x > 30.f ? true : false;
    }
    m_CharDirMarker->SetAngle(angle+180.f);
    if (distance > 300.f)
    {
        distance = 300.f;
    }
    else if (distance < 100.f)
    {
        distance = 100.f;
    }
    m_CharDirMarker->SetSize(distance,9);
}

void CStageUI::ReleaseWindow(float gauge[3], int wait[6])
{
    m_MousePosImg->SetOpacity(0.f);
    m_CharDirMarker->SetOpacity(0.f);
    size_t size = std::size(m_SkillChargeBar);
    for (size_t i = 0; i < size; ++i)
    {
        m_SkillChargeBar[i]->SetValue(gauge[i]/ wait[i]*100.f);
    }
}

bool CStageUI::ViewSkill(float gauge[3], int wait[6])
{
	if (!m_IsGaming)
	{
		return false;
	}
    m_IsSkillOn = false;
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	size_t size = std::size(m_SkillBar);
	for (int i = 0; i < size; ++i)
	{
        if (gauge[i] < wait[i])
        {
            continue;
        }
		m_SkillBar[i]->SetPos(mouseWorldPos.x-10.f * (i - 1), mouseWorldPos.y);
		m_SkillBar[i]->SetOpacity(0.5f);
        m_IsSkillOn = true;
	}
	m_RClickPos = CInput::GetInst()->GetMousePos();
    return m_IsSkillOn;
}

void CStageUI::DragSkill()
{
    if (!m_IsGaming|| !m_IsSkillOn)
    {
        return;
    }
    m_barDistance = m_RClickPos.Distance(CInput::GetInst()->GetMousePos()) * 0.02f;
    if (m_barDistance > 1.f)
    {
        m_barDistance = 1.f;
    }
    else if (m_barDistance < 0.5f)
    {
        m_barDistance = 0.5f;
    }
    float angle = m_RClickPos.Angle(CInput::GetInst()->GetMousePos());
    if (angle > 120)
    {
        m_SkillBar[0]->SetOpacity(m_barDistance);
        m_SkillBar[1]->SetOpacity(0.5f);
        m_SkillBar[2]->SetOpacity(0.5f);
    }
    else if (angle < 60)
    {
        m_SkillBar[0]->SetOpacity(0.5f);
        m_SkillBar[1]->SetOpacity(0.5f);
        m_SkillBar[2]->SetOpacity(m_barDistance);
    }
    else
    {
        m_SkillBar[0]->SetOpacity(0.5f);
        m_SkillBar[1]->SetOpacity(m_barDistance);
        m_SkillBar[2]->SetOpacity(0.5f);
    }
}

int CStageUI::ActivateSkill(float gauge[3], int wait[6])
{
    size_t size = std::size(m_SkillBar);
    int index=-1;
    for (int i = 0; i < size; ++i)
    {
        if (m_barDistance != 1.f)
        {
            m_SkillBar[i]->SetOpacity(0.f);
            continue;
        }
        if (m_SkillBar[i]->GetOpacity() == m_barDistance&& gauge[i] >= wait[i])
        {
            gauge[i] -= wait[i];
            SetSkillBar(i, gauge[i], wait[i]);
            index = i;
            m_barDistance = 0.f;
        }
        m_SkillBar[i]->SetOpacity(0.f);
    }
    m_barDistance = 0.f;
    return index;
}

void CStageUI::Pause()
{
    m_Scene->GetResource()->SoundPlay("PopWindow");
    if (1.f == CEngine::GetInst()->GetTimeScale())
    {
        //ui창 enable
        CEngine::GetInst()->SetTimeScale(0.f);
        return;
    }
    //ui창 unenable
    CEngine::GetInst()->SetTimeScale(1.f);
}

void CStageUI::Back()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Lobby.scn");
}

void CStageUI::Retry()
{
    m_FadeTime = -1.f;
    m_IsRetryUI = false;
    m_GameOverImg->SetOpacity(0.f);
    m_RetryBG->SetOpacity(0.f);
    m_ForgiveBtn->SetOpacity(0.f);
    m_ForgiveBtn->SetEnable(false);
    m_RetryBtn->SetOpacity(0.f);
    m_RetryBtn->SetEnable(false);
    m_RetryText->SetOpacity(0.f);
    m_RetrySemiText->SetOpacity(0.f);
    m_ForgiveBtnText->SetOpacity(0.f);
    m_RetryBtnText->SetOpacity(0.f);
    m_SkillCurtainDark->SetOpacity(0.f);
    int size = (int)std::size(m_CharImg);
    for (int i = 0; i < size; ++i)
    {
        m_HpBar[i]->SetValue(100.f);
        m_HpText[i]->SetText(TEXT("100.0%"));
        m_SkillChargeBar[i]->SetValue(100.f);
    }
    m_Scene->GetSceneInfo()->GetPlayerObject()->Reset();
}

void CStageUI::AddScore(int score)
{
    int originScore=_wtoi(m_ScoreText->GetText());
    originScore = originScore > 0 ? originScore :0 ;
    originScore += score;
    TCHAR	number[MAX_PATH] = {};
    swprintf_s(number, TEXT("%d"), originScore);
    m_ScoreText->SetText(number);
}
