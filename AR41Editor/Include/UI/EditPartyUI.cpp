#include "EditPartyUI.h"
#include "Input.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"
#include "UI/UIProgressBar.h"
#include "UI/UINumber.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../Scene/LoadingSceneInfo.h"
#include "../GameObject/BackGround.h"

CEditPartyUI::CEditPartyUI()
	: m_Index(0)
{
	m_WindowTypeName = "EditPartyUI";
	m_Scene = CSceneManager::GetInst()->GetScene();
}

CEditPartyUI::CEditPartyUI(const CEditPartyUI& window)
	: CUIWindow(window)
	, m_Index(window.m_Index)
{
    m_LeftButton = FindWidget<CUIButton>("ChangeLButton");
    m_RightButton = FindWidget<CUIButton>("ChangeRButton");
    m_SortButton = FindWidget<CUIButton>("SortButton");
    m_BackButton = FindWidget<CUIButton>("BackButton");
    size_t size = std::size(m_SpriteImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_SpriteImg[i] = FindWidget<CUIImage>("SpriteImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    size = std::size(m_AllCharSatue);
    for (size_t i = 0; i < size; ++i)
    {
        m_AllCharSatue[i] = FindWidget<CUIImage>("AllCharSatue" + std::to_string(i));
        m_AllCharImg[i] = FindWidget<CUIImage>("AllCharImg" + std::to_string(i));
    }
}

CEditPartyUI::~CEditPartyUI()
{
    m_Scene->GetResource()->SoundStop("Party");
}

void CEditPartyUI::Start()
{
    CUIWindow::Start();
    m_Scene->GetResource()->SoundPlay("Party");
    m_LeftButton->SetSound(EButtonEventState::Click, "UI", "ButtonClick", false, "System\\se_character_exchange.mp3");
    m_RightButton->SetSound(EButtonEventState::Click, "ButtonClick");
    m_SortButton->SetSound(EButtonEventState::Click, "ButtonClick");
    m_BackButton->SetSound(EButtonEventState::Click, "UI", "ButtonClick3", false, "System\\se_character_speech.mp3");

    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        int index = bg->GetUserData().partyIndex;
        size_t size = std::size(m_SpriteImg);
        for (int i = (index) * 6; i < (index +1) * 6; ++i)
        {
            std::string name = bg->GetUserData().charName[i];
            if (name == "none")
            {
                m_SpriteImg[i % 6]->SetOpacity(0.f);
                m_SpriteImg[i % 6]->SetSize(60.f, 150.f);
            }
            else
            {
                m_SpriteImg[i % 6]->SetOpacity(1.f);
                std::string imgPath = "Character//" + name + "//pixelart//animated//neutral.gif";
                TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
                t_filename[imgPath.size()] = 0;
                std::copy(imgPath.begin(), imgPath.end(), t_filename);
                m_SpriteImg[i % 6]->SetTexture(name, t_filename);
                m_SpriteImg[i % 6]->SetSize(m_SpriteImg[i % 6]->GetTextureInfo().texture->GetWidth()*2.5f, 
                                            m_SpriteImg[i % 6]->GetTextureInfo().texture->GetHeight() * 2.5f);
                SAFE_DELETE_ARRAY(t_filename);
            }
        }
        int count = bg->GetUserData().charCount;
        size = std::size(m_AllCharImg);
        for (size_t i = 0; i < size; ++i)
        {
            if(i>= count)
            {
                m_AllCharImg[i]->SetEnable(false);
                continue;
            }
            m_AllCharImg[i]->SetEnable(true);
            std::string name = bg->GetUserData().holdCharNames[i];
            std::string imgPath = "Character//" + name + "//pixelart//animated//neutral.gif";
            TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
            t_filename[imgPath.size()] = 0;
            std::copy(imgPath.begin(), imgPath.end(), t_filename);
            m_AllCharImg[i]->SetTexture(name, t_filename);
            m_AllCharImg[i]->SetSize(m_AllCharImg[i]->GetTextureInfo().texture->GetWidth() * 2.5f,
                                     m_AllCharImg[i]->GetTextureInfo().texture->GetHeight() * 2.5f);
            SAFE_DELETE_ARRAY(t_filename);
        }
    }

    m_LeftButton->SetCallback<CEditPartyUI>(EButtonEventState::Click, this, &CEditPartyUI::Left);
    m_RightButton->SetCallback<CEditPartyUI>(EButtonEventState::Click, this, &CEditPartyUI::Right);
    m_SortButton->SetCallback<CEditPartyUI>(EButtonEventState::Click, this, &CEditPartyUI::Sort);
    m_BackButton->SetCallback<CEditPartyUI>(EButtonEventState::Click, this, &CEditPartyUI::Back);
    CInput::GetInst()->AddBindFunction<CEditPartyUI>("Touch", Input_Type::Down, this, &CEditPartyUI::IconClick, m_Scene);
    CInput::GetInst()->AddBindFunction<CEditPartyUI>("Touch", Input_Type::Up, this, &CEditPartyUI::IconRelease, m_Scene);
}

bool CEditPartyUI::Init()
{
    CUIWindow::Init();
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

    m_SortButton = CreateWidget<CUIButton>("SortButton");
    m_SortButton->SetSize(124.f * 0.8f, 129.f * 0.8f);
    m_SortButton->SetPos(550.f, 880.f);
    m_SortButton->SetTexture(EButtonState::Normal, "SortButton", TEXT("Button//sort.png"));
    m_SortButton->SetTexture(EButtonState::Hovered, "SortButton", TEXT("Button//sort.png"));
    m_SortButton->SetTexture(EButtonState::Click, "SortButton", TEXT("Button//sort.png"));
    m_SortButton->SetTexture(EButtonState::Disable, "SortButton", TEXT("Button//sort.png"));

    m_BackButton = CreateWidget<CUIButton>("BackButton");
    m_BackButton->SetSize(147.f, 137.f);
    m_BackButton->SetPos(536.f, 0.f);
    m_BackButton->SetTexture(EButtonState::Normal, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Hovered, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Click, "BackButton", TEXT("Button//back.png"));
    m_BackButton->SetTexture(EButtonState::Disable, "BackButton", TEXT("Button//back.png"));

    size_t size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = CreateWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_MainStatueImg[i]->SetSize(160.f*0.6f, 155.f*0.6f);
        m_MainStatueImg[i]->SetPos(735.f + i * 190.f, 800.f);
        m_MainStatueImg[i]->SetPivot(0.5f, 0.5f);
        m_MainStatueImg[i]->SetTexture("MainStatueImg", TEXT("UI//mainStatue_sprite.png"));
        m_UnisonStatueImg[i] = CreateWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i]->SetSize(164.f * 0.6f,117.f * 0.6f);
        m_UnisonStatueImg[i]->SetPos(805.f + i * 190.f, 778.f);
        m_UnisonStatueImg[i]->SetPivot(0.5f, 0.5f);
        m_UnisonStatueImg[i]->SetTexture("UnisonStatueImg", TEXT("UI//unisonStatue_sprite.png"));
    }
    size = std::size(m_SpriteImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_SpriteImg[i] = CreateWidget<CUIImage>("SpriteImg" + std::to_string(i));
        if(i<3)
        {
            m_SpriteImg[i]->SetPos(738.f + i * 190.f, 851.f);
        }
        else
        {
            m_SpriteImg[i]->SetPos(235.f + i * 190.f, 815.f);
        }
        m_SpriteImg[i]->SetPivot(0.5f, 0.5f);
    }
    size = std::size(m_AllCharSatue);
    for (size_t i = 0; i < size; ++i)
    {
        m_AllCharSatue[i] = CreateWidget<CUIImage>("AllCharSatue" + std::to_string(i));
        m_AllCharSatue[i]->SetSize(160.f * 0.6f, 155.f * 0.6f);
        m_AllCharSatue[i]->SetPos(772.f + (i / 4) * 135.f, 565.f - (i % 4) * 135.f);
        if (i % 2 == 1)
        {
            m_AllCharSatue[i]->AddPos(70.f, 0.f);
        }
        m_AllCharSatue[i]->SetPivot(0.5f, 0.5f);
        m_AllCharSatue[i]->SetTexture("AllCharSatue", TEXT("UI//charStatue_sprite.png"));

        m_AllCharImg[i] = CreateWidget<CUIImage>("AllCharImg" + std::to_string(i));
        m_AllCharImg[i]->SetPos(774.f + (i / 4) * 135.f, 615.f - (i % 4) * 135.f);
        if (i % 2 == 1)
        {
            m_AllCharImg[i]->AddPos(70.f, 0.f);
        }
        m_AllCharImg[i]->SetPivot(0.5f, 0.5f);
    }
    return true;
}

void CEditPartyUI::Update(float deltaTime)
{
    CUIWindow::Update(deltaTime);
}

void CEditPartyUI::PostUpdate(float deltaTime)
{
    CUIWindow::PostUpdate(deltaTime);
}

void CEditPartyUI::Render()
{
    CUIWindow::Render();
}

CEditPartyUI* CEditPartyUI::Clone()
{
    return new CEditPartyUI(*this);
}

void CEditPartyUI::Save(FILE* file)
{
    CUIWindow::Save(file);
}

void CEditPartyUI::Load(FILE* file)
{
    CUIWindow::Load(file);
    m_LeftButton = FindWidget<CUIButton>("ChangeLButton");
    m_RightButton = FindWidget<CUIButton>("ChangeRButton");
    m_SortButton = FindWidget<CUIButton>("SortButton");
    m_BackButton = FindWidget<CUIButton>("BackButton");
    size_t size = std::size(m_SpriteImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_SpriteImg[i] = FindWidget<CUIImage>("SpriteImg" + std::to_string(i));
    }
    size = std::size(m_MainStatueImg);
    for (size_t i = 0; i < size; ++i)
    {
        m_MainStatueImg[i] = FindWidget<CUIImage>("MainStatueImg" + std::to_string(i));
        m_UnisonStatueImg[i] = FindWidget<CUIImage>("UnisonStatueImg" + std::to_string(i));
    }
    size = std::size(m_AllCharSatue);
    for (size_t i = 0; i < size; ++i)
    {
        m_AllCharSatue[i] = FindWidget<CUIImage>("AllCharSatue" + std::to_string(i));
        m_AllCharImg[i] = FindWidget<CUIImage>("AllCharImg" + std::to_string(i));
    }
}

void CEditPartyUI::Sort()
{
}

void CEditPartyUI::Back()
{
    CSceneManager::GetInst()->CreateNextScene();
    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, 
        m_Scene->GetSceneInfo()->GetPrevFileName(), m_Scene->GetSceneInfo()->GetFileName());
}

void CEditPartyUI::Left()
{
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        int count = bg->GetUserData().charCount;
        size_t size = std::size(m_AllCharSatue);
        count -= (int)size;
        if(count <=0)
        {
            return;
        }
        if(--m_Index<0)
        {
            m_Index = bg->GetUserData().charCount/ size;
        }
        for (size_t i = 0; i < size; ++i)
        {
            if (i >= count)
            {
                m_AllCharSatue[i]->SetEnable(false);
                m_AllCharImg[i]->SetEnable(false);
                continue;
            }
            m_AllCharSatue[i]->SetEnable(true);
            m_AllCharImg[i]->SetEnable(true);
            std::string name = bg->GetUserData().holdCharNames[i+(size*m_Index)];
            std::string imgPath = "Character//" + name + "//pixelart//animated//neutral.gif";
            TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
            t_filename[imgPath.size()] = 0;
            std::copy(imgPath.begin(), imgPath.end(), t_filename);
            m_AllCharImg[i]->SetTexture("AllCharImg" + std::to_string(i) + name, t_filename);
            m_AllCharImg[i]->SetSize(m_AllCharImg[i]->GetTextureInfo().texture->GetWidth() * 2.5f,
                m_AllCharImg[i]->GetTextureInfo().texture->GetHeight() * 2.5f);
            SAFE_DELETE_ARRAY(t_filename);
        }
    }
}

void CEditPartyUI::Right()
{
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (bg)
    {
        int count = bg->GetUserData().charCount;
        size_t size = std::size(m_AllCharSatue);
        count -= (int)size;
        if (count <= 0)
        {
            return;
        }
        ++m_Index;
        for (size_t i = 0; i < size; ++i)
        {
            if (i >= count)
            {
                m_AllCharSatue[i]->SetEnable(false);
                m_AllCharImg[i]->SetEnable(false);
                continue;
            }
            m_AllCharSatue[i]->SetEnable(true);
            m_AllCharImg[i]->SetEnable(true);
            std::string name = bg->GetUserData().holdCharNames[i+(size*m_Index)];
            std::string imgPath = "Character//" + name + "//pixelart//animated//neutral.gif";
            TCHAR* t_filename = new TCHAR[imgPath.size() + 1];
            t_filename[imgPath.size()] = 0;
            std::copy(imgPath.begin(), imgPath.end(), t_filename);
            m_AllCharImg[i]->SetTexture("AllCharImg" + std::to_string(i) + name, t_filename);
            m_AllCharImg[i]->SetSize(m_AllCharImg[i]->GetTextureInfo().texture->GetWidth() * 2.5f,
                m_AllCharImg[i]->GetTextureInfo().texture->GetHeight() * 2.5f);
            SAFE_DELETE_ARRAY(t_filename);
        }
    }
}

void CEditPartyUI::IconClick()
{
    size_t size = std::size(m_SpriteImg);
    for (size_t i = 0; i < size; ++i)
    {
        if (m_SpriteImg[i]->GetMouseHovered())
        {
            m_Scene->GetResource()->SoundPlay("CharGrap");
            CTexture* texture = m_SpriteImg[i]->GetTextureInfo().texture;
            if(texture&&texture->GetName()=="none")
            {
                return;
            }
            m_MouseImg = CInput::GetInst()->GetMouseImg();
            m_MouseImg->SetTexture(texture);
            m_MouseImg->SetSize(m_SpriteImg[i]->GetSize() * 0.7f);
            m_MouseImg->SetPivot(0.5f, 0.5f);
            return;
        }
    }
    size = std::size(m_AllCharImg);
    for (size_t i = 0; i < size; ++i)
    {
        if (m_AllCharImg[i]->GetMouseHovered())
        {
            m_Scene->GetResource()->SoundPlay("CharGrap");
            CTexture* texture = m_AllCharImg[i]->GetTextureInfo().texture;
            m_MouseImg = CInput::GetInst()->GetMouseImg();
            m_MouseImg->SetTexture(texture);
            m_MouseImg->SetSize(m_AllCharImg[i]->GetSize() * 0.7f);
            m_MouseImg->SetPivot(0.5f, 0.5f);
            return;
        }
    }
}

void CEditPartyUI::IconRelease()
{
    std::string textureName = CInput::GetInst()->GetMouseImg()->GetTextureInfo().texture->GetName();
    if(textureName == "Mouse")
    {
        return;
    }
    int partyIndex=-1;
    size_t size = std::size(m_SpriteImg);
    CBackGround* bg = (CBackGround*)m_Scene->FindObject("Background");
    if (!bg)
    {
        return;
    }
    int index = bg->GetUserData().partyIndex;
    for (int i = (index) * 6; i < (index + 1) * 6; ++i)
    {
        std::string name = bg->GetUserData().charName[i];
        if (name == textureName)
        {
            partyIndex = i%6;
            break;
        }
    }
    for (size_t i = 0; i < size; ++i)
    {
        if (m_SpriteImg[i]->GetMouseHovered())
        {
            if(partyIndex==i)
            {
                partyIndex = -1;
                break;   //선택한 이미지가 교체할 이미지와 같은 경우
            }
            m_Scene->GetResource()->SoundPlay("CharChange");
            if(m_SpriteImg[i]->GetOpacity()==0.f)
            {
                m_SpriteImg[i]->SetOpacity(1.f);
                m_SpriteImg[i]->SetTexture(m_MouseImg->GetTextureInfo().texture);
                m_SpriteImg[i]->SetSize(m_MouseImg->GetTextureInfo().texture->GetWidth() * 2.5f,
                                        m_MouseImg->GetTextureInfo().texture->GetHeight() * 2.5f);
                bg->SetCharName((int)i + index * 6, textureName);
                if (partyIndex != -1)
                {
                    m_SpriteImg[partyIndex]->SetOpacity(0.f);
                    m_SpriteImg[partyIndex]->SetTexture("none",TEXT("none.png"));//안돼면 이구문 참조
                    bg->SetCharName(partyIndex + index * 6, "none");
                    partyIndex = -1;
                }
                break;
            }
            if (partyIndex != -1)
            {
                CTexture* texture = m_SpriteImg[i]->GetTextureInfo().texture;
                m_SpriteImg[partyIndex]->SetTexture(texture);
                m_SpriteImg[partyIndex]->SetSize(texture->GetWidth() * 2.5f, texture->GetHeight() * 2.5f);
                bg->SetCharName(partyIndex + index * 6, texture->GetName());
                partyIndex = -1;
            }
            m_SpriteImg[i]->SetTexture(m_MouseImg->GetTextureInfo().texture);
            m_SpriteImg[i]->SetSize(m_MouseImg->GetTextureInfo().texture->GetWidth() * 2.5f,
                m_MouseImg->GetTextureInfo().texture->GetHeight() * 2.5f);
            bg->SetCharName((int)i + index * 6, textureName);
            break;
        }
    }
    if (partyIndex != -1)
    {
        m_Scene->GetResource()->SoundPlay("CharRelease");
        m_SpriteImg[partyIndex]->SetOpacity(0.f);
        m_SpriteImg[partyIndex]->SetTexture("none", TEXT("none.png"));
        bg->SetCharName(partyIndex + index * 6, "none");
    }
    CInput::GetInst()->InitMouseImg();  //마우스 초기화 함수 무조건 실행
}
