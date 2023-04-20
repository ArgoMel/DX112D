#include "BossUI.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CBossUI::CBossUI()
    : m_TextTime(-1.f)
{
    m_WindowTypeName = "BossUI";
    m_Scene = CSceneManager::GetInst()->GetScene();
}

CBossUI::CBossUI(const CBossUI& window)
    : CUIWindow(window)
    , m_TextTime(-1.f)
{
    m_BossCover = FindWidget<CUIImage>("BossCover");
    m_BossName = FindWidget<CUIText>("BossName");
    m_BossSkillCover = FindWidget<CUIImage>("BossSkillCover");
    m_BossSkillName = FindWidget<CUIText>("BossSkillName");
    m_BossHPBar = FindWidget<CUIProgressBar>("BossHPBar");
    size_t size = std::size(m_BossBuff);
    for (size_t i = 0; i < size; ++i)
    {
        m_BossBuff[i] = FindWidget<CUIImage>("BossBuff" + std::to_string(i));
    }
    size = std::size(m_BossDeBuff);
    for (size_t i = 0; i < size; ++i)
    {
        m_BossDeBuff[i] = FindWidget<CUIImage>("BossDeBuff" + std::to_string(i));
    }
}

CBossUI::~CBossUI()
{
}

void CBossUI::Start()
{
    CUIWindow::Start();
}

bool CBossUI::Init()
{
    CUIWindow::Init();
    m_BossCover = CreateWidget<CUIImage>("BossCover");
    m_BossCover->SetPos(0.f, 50.f);
    m_BossCover->SetSize(100.f, 20.f);
    m_BossCover->SetPivot(0.5f, 0.5f);
    m_BossCover->SetImageTint(240,240,240,255);
    m_BossCover->SetTexture("WhiteRoundRec", TEXT("Frame\\white_round_rec.png"));

    m_BossName = CreateWidget<CUIText>("BossName");
    m_BossName->SetPos(-75.f, 45.f);
    m_BossName->SetSize(150.f, 20.f);
    m_BossName->SetPivot(0.5f, 0.5f);
    m_BossName->SetFontSize(20.f);
    m_BossName->SetText(TEXT("이름"));
    m_BossName->SetAlignH(Text_Align_H::Center);
    m_BossName->SetShadowEnable(true);
    m_BossName->SetShadowColor(128, 128, 128);
    m_BossName->SetShadowOffset(1.5f, 1.5f);

    m_BossHPBar = CreateWidget<CUIProgressBar>("BossHPBar");
    m_BossHPBar->SetPos(0.f, 65.f);
    m_BossHPBar->SetSize(150.f, 10.f);
    m_BossHPBar->SetImageTint(EProgressBarTextureType::Back, 255, 255, 255, 100);
    m_BossHPBar->SetImageTint(EProgressBarTextureType::Bar, 0, 255, 50, 255);
    m_BossHPBar->SetTexture(EProgressBarTextureType::Bar, "LoagindBar", TEXT("Frame\\loadingGauge.png"));
    m_BossHPBar->SetProgressBarMin(0.f);
    m_BossHPBar->SetProgressBarMax(100.f);
    m_BossHPBar->SetValue(100.f);
    m_BossHPBar->SetBarDir(EProgressBarDir::RightToLeft);
    m_BossHPBar->SetPivot(0.5f, 0.5f);

    m_BossSkillCover = CreateWidget<CUIImage>("BossSkillCover");
    m_BossSkillCover->SetPos(0.f, -50.f);
    m_BossSkillCover->SetSize(203.5f, 61.f);
    m_BossSkillCover->SetPivot(0.5f, 0.5f);
    m_BossSkillCover->SetAngle(5.f);
    m_BossSkillCover->SetTexture("BossSkillCover", TEXT("Frame\\boss_text_cover.png"));
    m_BossSkillCover->SetEnable(false);

    m_BossSkillName = CreateWidget<CUIText>("BossSkillName");
    m_BossSkillName->SetPos(-100.f, -55.f);
    m_BossSkillName->SetSize(200.f, 20.f);
    m_BossSkillName->SetPivot(0.5f, 0.5f);
    m_BossSkillName->SetFontSize(20.f);
    m_BossSkillName->SetText(TEXT("스킬 이름"));
    m_BossSkillName->SetAlignH(Text_Align_H::Center);
    m_BossSkillName->SetShadowEnable(true);
    m_BossSkillName->SetShadowColor(80, 80, 80);
    m_BossSkillName->SetShadowOffset(1.5f, 1.5f);
    m_BossSkillName->SetEnable(false);

    int size =(int)std::size(m_BossBuff);
    for (int i = 0; i < size; ++i)
    {
        m_BossBuff[i] = CreateWidget<CUIImage>("BossBuff" + std::to_string(i));
        m_BossBuff[i]->SetPos(-64.f+(i%8)*18.f, 80.f+ 18.f * (i / 8));
        m_BossBuff[i]->SetSize(16.f, 16.f);
        m_BossBuff[i]->SetPivot(0.5f, 0.5f);
        TCHAR	fileName[MAX_PATH] = {};
        if (i < 10)
        {
            wsprintf(fileName, TEXT("Icon\\skill\\buff00%d.png"), i);
        }
        else
        {
            wsprintf(fileName, TEXT("Icon\\skill\\buff0%d.png"), i);
        }
        m_BossBuff[i]->SetTexture("BossBuff" + std::to_string(i), fileName);
        m_BossBuff[i]->SetOpacity(0.f);
    }
    size = (int)std::size(m_BossDeBuff);
    for (int i = 0; i < size; ++i)
    {
        m_BossDeBuff[i] = CreateWidget<CUIImage>("BossDeBuff" + std::to_string(i));
        m_BossDeBuff[i]->SetPos(-64.f + (i % 8) * 18.f, 116.f + 18.f * (i / 8));
        m_BossDeBuff[i]->SetSize(16.f, 16.f);
        m_BossDeBuff[i]->SetPivot(0.5f, 0.5f);
        TCHAR	fileName[MAX_PATH] = {};
        if (i < 10)
        {
            wsprintf(fileName, TEXT("Icon\\skill\\debuff00%d.png"), i);
        }
        else
        {
            wsprintf(fileName, TEXT("Icon\\skill\\debuff0%d.png"), i);
        }
        m_BossDeBuff[i]->SetTexture("BossDeBuff" + std::to_string(i), fileName);
        m_BossDeBuff[i]->SetOpacity(0.f);
    }
    return true;
}

void CBossUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
    if(m_TextTime>0.f)
    {
        m_TextTime -= deltaTime;
        if (m_TextTime <= 0.f)
        {
            m_BossSkillCover->SetEnable(false);
            m_BossSkillName->SetEnable(false);
        }
    }
}

void CBossUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CBossUI::Render()
{
    CUIWindow::Render();
}

CBossUI* CBossUI::Clone()
{
    return new CBossUI(*this);
}

void CBossUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CBossUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_BossCover = FindWidget<CUIImage>("BossCover");
    m_BossName = FindWidget<CUIText>("BossName");
    m_BossSkillCover = FindWidget<CUIImage>("BossSkillCover");
    m_BossSkillName = FindWidget<CUIText>("BossSkillName");
    m_BossHPBar = FindWidget<CUIProgressBar>("BossHPBar");
    size_t size = std::size(m_BossBuff);
    for (size_t i = 0; i < size; ++i)
    {
        m_BossBuff[i] = FindWidget<CUIImage>("BossBuff" + std::to_string(i));
    }
    size = std::size(m_BossDeBuff);
    for (size_t i = 0; i < size; ++i)
    {
        m_BossDeBuff[i] = FindWidget<CUIImage>("BossDeBuff" + std::to_string(i));
    }
}

void CBossUI::Damaged(float value)
{
    m_BossHPBar->SetValue(value);
    if (value < 50.f)
    {
        m_BossHPBar->SetImageTint(EProgressBarTextureType::Bar, 255, 127, 39, 255);
    }
    else if (value < 20.f)
    {
        m_BossHPBar->SetImageTint(EProgressBarTextureType::Bar, 237, 28, 36, 255);
    }
}

void CBossUI::SetSkillName(const std::string& name, float sizeX, float sizeY)
{
    m_TextTime = 1.f;
    m_BossSkillCover->SetEnable(true);
    m_BossSkillCover->SetSize(sizeX, sizeY);
    m_BossSkillName->SetEnable(true);
    TCHAR* t_filename = new TCHAR[name.size() + 1];
    t_filename[name.size()] = 0;
    std::copy(name.begin(), name.end(), t_filename);
    m_BossSkillName->SetText(t_filename);
    SAFE_DELETE_ARRAY(t_filename);
}

void CBossUI::SetText(const TCHAR* text)
{
    m_BossName->SetText(text);
    int count=m_BossName->GetTextCount();
    m_BossCover->SetSize(12.f*count,20.f);
}

void CBossUI::SetBuffImg(int index, bool b)
{
    m_BossBuff[index]->SetOpacity(b);
}

void CBossUI::SetDeBuffImg(int index, bool b)
{
    m_BossDeBuff[index]->SetOpacity(b);
}
