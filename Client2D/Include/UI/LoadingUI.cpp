#include "LoadingUI.h"
#include "UI/UIProgressBar.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UINumber.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CLoadingUI::CLoadingUI()
{
	m_WindowTypeName = "LoadingUI";
}

CLoadingUI::CLoadingUI(const CLoadingUI& window)
	: CUIWindow(window)
{
    size_t size = std::size(m_LoadingNumber);
    for (size_t h = 0; h < size; ++h)
    {
        m_LoadingNumber[h] = FindWidget<CUINumber>("Number" + std::to_string(h));
    }
    m_LoadingBar = FindWidget<CUIProgressBar>("LoadingBar");
    m_LoadingCircle = FindWidget<CUIImage>("loadingcircle");
    m_LoadingText = FindWidget<CUIText>("Title");
}

CLoadingUI::~CLoadingUI()
{
}

void CLoadingUI::SetLoadingPercent(float percent)
{
    float number = percent * 100.f;
    m_LoadingBar->SetValue(number);
    m_LoadingNumber[0]->SetNumber((int)number);
    if (number > 10)
    {
        m_LoadingNumber[0]->SetPos(855.f, 100.f);
    }
    if (number >= 100)
    {
        m_LoadingNumber[0]->SetPos(835.f, 100.f);
    }
    number -= (int)number;
    number *= 100.f;
    std::stack<int>	numberStack;
    while ((int)number > 0)
    {
        numberStack.push((int)number % 10);
        number /= 10;
    }
    while (numberStack.size() < 2)
    {
        numberStack.push(0);
    }
    size_t size = std::size(m_LoadingNumber);
    for (size_t h = 1; h < size; ++h)
    {
        m_LoadingNumber[h]->SetNumber(numberStack.top());
        numberStack.pop();
    }
}

void CLoadingUI::Start()
{
	CUIWindow::Start();
}

bool CLoadingUI::Init()
{
    CUIWindow::Init();
    m_LoadingTips = CreateWidget<CUIImage>("LoadingTips");
    m_LoadingTips->SetPos(940.f, 600.f);
    m_LoadingTips->SetSize(705.f, 180.f);
    m_LoadingTips->SetPivot(0.5f, 0.5f);
    int random = rand() % 36;
    TCHAR	fileName[MAX_PATH] = {};
    if(random<10)
    {
        wsprintf(fileName, TEXT("loading_tips\\tips00%d.png"), random);
    }
    else
    {
        wsprintf(fileName, TEXT("loading_tips\\tips0%d.png"), random);
    }
    m_LoadingTips->SetTexture("LoadingTips", fileName);

    size_t size = std::size(m_LoadingNumber);
    for (size_t h = 0; h < size;++h)
    {
        m_LoadingNumber[h] = CreateWidget<CUINumber>("Number"+ std::to_string(h));
        m_LoadingNumber[h]->SetPos(875.f+h*20.f, 100.f);
        if(h==1|| h == 2)
        {
            m_LoadingNumber[h]->AddPos(3.f, 0.f);
        }
        m_LoadingNumber[h]->SetSize(30.f, 30.f);
        m_LoadingNumber[h]->SetPivot(0.5f, 0.5f);
        m_LoadingNumber[h]->SetSpace(-10.f);
        m_LoadingNumber[h]->SetTexture("Number", TEXT("Text//rankingNumber.png"));
        for (int i = 0; i < 10; ++i)
        {
            m_LoadingNumber[h]->AddFrameData(Vector2(i * 25.f, 0.f), Vector2((i + 1) * 25.f, 25.f));
        }
    }

    CUIText* pointText = CreateWidget<CUIText>("PointText");
    pointText->SetPos(836.f, 20.f);
    pointText->SetSize(100.f, 100.f);
    pointText->SetPivot(0.5f, 0.5f);
    pointText->SetFontSize(30.f);
    pointText->SetText(TEXT("."));
    pointText->SetAlignH(Text_Align_H::Center);

    CUIImage* m_LoadingPercent = CreateWidget<CUIImage>("LoadingPercent");
    m_LoadingPercent->SetPos(940.f, 100.f);
    m_LoadingPercent->SetSize(30.f, 30.f);
    m_LoadingPercent->SetPivot(0.5f, 0.5f);
    m_LoadingPercent->SetTexture("LoadingPercent", TEXT("Text\\ranking_percent.png"));

    m_LoadingBar = CreateWidget<CUIProgressBar>("LoadingBar");
    m_LoadingBar->SetPos(550.f, 50.f);
    m_LoadingBar->SetSize(400.f, 32.f);
    m_LoadingBar->SetImageTint(EProgressBarTextureType::Back, 255, 255, 255, 255);
    m_LoadingBar->SetTexture(EProgressBarTextureType::Bar, "LoagindBar", TEXT("Frame\\loadingGauge.png"));
    m_LoadingBar->SetProgressBarMin(0.f);
    m_LoadingBar->SetProgressBarMax(100.f);
    m_LoadingBar->SetValue(0.f);
    m_LoadingBar->SetBarDir(EProgressBarDir::RightToLeft);

    m_LoadingCircle = CreateWidget<CUIImage>("loadingcircle");
    m_LoadingCircle->SetPos(1250.f, 200.f);
    m_LoadingCircle->SetSize(205.f, 205.f);
    m_LoadingCircle->SetPivot(0.5f, 0.5f);
    m_LoadingCircle->SetTexture("loadingcircle", TEXT("Frame\\loadingcircle.png"));
    
    m_LoadingText = CreateWidget<CUIText>("loadingText");
    m_LoadingText->SetPos(1110.f, 0.f);
    m_LoadingText->SetSize(300.f, 100.f);
    m_LoadingText->SetPivot(0.5f, 0.5f);
    m_LoadingText->SetFontSize(35.f);
    m_LoadingText->SetText(TEXT("Please wait..."));
    m_LoadingText->SetAlignH(Text_Align_H::Center);
    return true;
}

void CLoadingUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
    m_LoadingCircle->AddAngle(-360.f * deltaTime);
}

void CLoadingUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CLoadingUI::Render()
{
    CUIWindow::Render();
}

CLoadingUI* CLoadingUI::Clone()
{
    return new CLoadingUI(*this);
}

void CLoadingUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CLoadingUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    size_t size = std::size(m_LoadingNumber);
    for (size_t h = 0; h < size; ++h)
    {
        m_LoadingNumber[h] = FindWidget<CUINumber>("Number" + std::to_string(h));
    }
    m_LoadingBar = FindWidget<CUIProgressBar>("LoadingBar");
    m_LoadingCircle = FindWidget<CUIImage>("loadingcircle");
    m_LoadingText = FindWidget<CUIText>("Title");
}
