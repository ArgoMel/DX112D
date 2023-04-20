#include "TestWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInputText.h"
//#include "Editor/EditorCursorPos.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"

CTestWindow::CTestWindow()
	: m_Image{}
	, m_Text(nullptr)
	, m_Input(nullptr)
	, m_Combo(nullptr)
	, m_List(nullptr)
	, m_Tree(nullptr)
{
}

CTestWindow::~CTestWindow()
{
	
}

bool CTestWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<int>>("Tree");
	m_Tree->SetHideName("Tree");
	m_Tree->AddItem(0, "Root");
	m_Tree->AddItem(0, "진짜 최상위 부모");
	m_Tree->AddItem(0, "자식1", "진짜 최상위 부모");
	m_Tree->AddItem(0, "자식2", "Root");
	m_Tree->AddItem(0, "자식3", "자식2");
	m_Tree->SetSelectCallback<CTestWindow>(this, &CTestWindow::TreeCallback);

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("저장된 캐릭터 정보");
	m_Text->SetColor(255, 255, 0, 255);
	m_List = CreateWidget<CEditorListBox>("ListBox");
	m_List->SetHideName("ListBox");
	m_List->AddItem("Alk");
	m_List->AddItem("Zeta");
	m_List->AddItem("Marina");
	m_List->AddItem("Kohane");

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("이미지 정보");
	m_Text->SetColor(255, 255, 0, 255);

	CEditorButton* btn = CreateWidget<CEditorButton>("캐릭터 스프라이트");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::CharSprButtonCallback);
	btn->SetSize(150.f,30.f);
	CEditorSameLine* sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[0] = CreateWidget<CEditorImage>("Char_sprite");
	m_Image[0]->SetTexture("Char_sprite", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("스킬 스프라이트");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillSprButtonCallback);
	btn->SetSize(150.f, 30.f);
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[1] = CreateWidget<CEditorImage>("Skill_sprite");
	m_Image[1]->SetTexture("Skill_sprite", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("스킬 바");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillBarButtonCallback);
	btn->SetSize(150.f, 30.f);
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[2] = CreateWidget<CEditorImage>("Skill_bar");
	m_Image[2]->SetTexture("Skill_bar", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("스킬 커튼");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillImgButtonCallback);
	btn->SetSize(150.f, 30.f);
	m_Image[3] = CreateWidget<CEditorImage>("Skill_image");
	m_Image[3]->SetTexture("Skill_image", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("캐릭터 전신");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::CharAllButtonCallback);
	btn->SetSize(150.f, 30.f);
	m_Image[4] = CreateWidget<CEditorImage>("Char_all");
	m_Image[4]->SetTexture("Char_all", TEXT("teemo.png"));


	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("리더 정보");
	m_Text->SetColor(255, 255, 0, 255);

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("스킬 정보");
	m_Text->SetColor(255,255,0,255);
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("지속시간 존재");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("파티에게도 적용");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("연격효과");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("일부 종족에만 적용");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("일부 속성에만 적용");
	m_Text = CreateWidget<CEditorText>("Text");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(132.f);
	m_Combo = CreateWidget<CEditorComboBox>("ComboBox");
	m_Combo->SetSelectPrevViewName(true);
	m_Combo->SetHideName("ComboBox");
	m_Combo->SetPrevViewName("속성");
	m_Combo->AddItem("화");
	m_Combo->AddItem("수");
	m_Combo->AddItem("목");
	m_Combo->AddItem("뇌");
	m_Combo->AddItem("광");
	m_Combo->AddItem("암");
	m_Text->SetText("상태이상 해제");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("데미지계수");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo1");
	m_Input->SetHideName("SkillInfo1");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("공격력증가");
	m_Text->SetText("공격력증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo2");
	m_Input->SetHideName("SkillInfo2");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("스킬데미지");
	m_Text->SetText("스킬데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo3");
	m_Input->SetHideName("SkillInfo3");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("직접공격데미지");
	m_Text->SetText("직접공격데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(122.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo4");
	m_Input->SetHideName("SkillInfo4");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("파워플립데미지");
	m_Text->SetText("파워플립데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(122.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo5");
	m_Input->SetHideName("SkillInfo5");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("다운용이성");
	m_Text->SetText("다운용이성");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo6");
	m_Input->SetHideName("SkillInfo6");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("피버게이지");
	m_Text->SetText("피버게이지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo7");
	m_Input->SetHideName("SkillInfo7");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("상태이상인 적에게 추가데미지");
	m_Text->SetText("상태이상인 적에게 추가데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(30.f);
	//상태이상 체크후 입력
	m_Input = CreateWidget<CEditorInputText>("SkillInfo8");
	m_Input->SetHideName("SkillInfo8");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("체력증가");
	m_Text->SetText("체력증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo9");
	m_Input->SetHideName("SkillInfo9");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("내성증가");
	m_Text->SetText("내성증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo10");
	m_Input->SetHideName("SkillInfo10");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("콤보증가");
	m_Text->SetText("콤보증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo11");
	m_Input->SetHideName("SkillInfo11");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("버프의 지속시간 증가");
	m_Text->SetText("버프의 지속시간 증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(86.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo12");
	m_Input->SetHideName("SkillInfo12");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("이동속도증가");
	m_Text->SetText("이동속도증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(136.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo13");
	m_Input->SetHideName("SkillInfo13");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	//스킬의 콜라이더는 이름붙여서 맵에 저장해놓고 거기서 ui로 가져오는 방식

	m_Text = CreateWidget<CEditorText>("어빌리티1 정보");
	m_Text->SetText("어빌리티1 정보");
	m_Text->SetColor(255, 255, 0, 255);
	m_Text = CreateWidget<CEditorText>("시작시 적용");
	m_Text->SetText("시작시 적용");
	m_Text = CreateWidget<CEditorText>("지속시간 존재");
	m_Text->SetText("지속시간 존재");
	m_Text = CreateWidget<CEditorText>("파티에게도 적용");
	m_Text->SetText("파티에게도 적용");
	m_Text = CreateWidget<CEditorText>("연격효과");
	m_Text->SetText("연격효과");
	m_Text = CreateWidget<CEditorText>("일부 종족에만 적용");
	m_Text->SetText("일부 종족에만 적용");
	m_Text = CreateWidget<CEditorText>("일부 속성에만 적용");
	m_Text->SetText("일부 속성에만 적용");
	m_Text = CreateWidget<CEditorText>("상태이상 면역");
	m_Text->SetText("상태이상 면역");
	m_Text = CreateWidget<CEditorText>("상태이상 해제");
	m_Text->SetText("상태이상 해제");
	m_Text = CreateWidget<CEditorText>("공격력증가");
	m_Text->SetText("공격력증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("공격력");
	m_Input->SetHideName("공격력");
	//inputText->SetSizeX(10.f);	//안먹힘
	m_Text = CreateWidget<CEditorText>("스킬데미지");
	m_Text->SetText("스킬데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("스킬데미지");
	m_Input->SetHideName("스킬데미지");
	m_Text = CreateWidget<CEditorText>("직접공격데미지");
	m_Text->SetText("직접공격데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("직접공격데미지");
	m_Input->SetHideName("직접공격데미지");
	m_Text = CreateWidget<CEditorText>("파워플립데미지");
	m_Text->SetText("파워플립데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("파워플립데미지");
	m_Input->SetHideName("파워플립데미지");
	m_Text = CreateWidget<CEditorText>("다운용이성");
	m_Text->SetText("다운용이성");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("다운용이성");
	m_Input->SetHideName("다운용이성");
	m_Text = CreateWidget<CEditorText>("피버게이지");
	m_Text->SetText("피버게이지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("피버게이지");
	m_Input->SetHideName("피버게이지");
	m_Text = CreateWidget<CEditorText>("상태이상인 적에게 추가데미지");
	m_Text->SetText("상태이상인 적에게 추가데미지");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	//상태이상 체크후 입력
	m_Input = CreateWidget<CEditorInputText>("별도계산데미지");
	m_Input->SetHideName("별도계산데미지");
	m_Text = CreateWidget<CEditorText>("체력증가");
	m_Text->SetText("체력증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("체력증가");
	m_Input->SetHideName("체력증가");
	m_Text = CreateWidget<CEditorText>("내성증가");
	m_Text->SetText("내성증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("내성증가");
	m_Input->SetHideName("내성증가");
	m_Text = CreateWidget<CEditorText>("콤보증가");
	m_Text->SetText("콤보증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("콤보증가");
	m_Input->SetHideName("콤보증가");
	m_Text = CreateWidget<CEditorText>("버프의 지속시간 증가");
	m_Text->SetText("버프의 지속시간 증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("지속시간증가");
	m_Input->SetHideName("지속시간증가");
	m_Text = CreateWidget<CEditorText>("이동속도증가");
	m_Text->SetText("이동속도증가");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("이동속도증가");
	m_Input->SetHideName("이동속도증가");

	m_Text = CreateWidget<CEditorText>("어빌리티2 정보");
	m_Text->SetText("어빌리티2 정보");
	m_Text->SetColor(255, 255, 0, 255);

	m_Text = CreateWidget<CEditorText>("어빌리티3 정보");
	m_Text->SetText("어빌리티3 정보");
	m_Text->SetColor(255, 255, 0, 255);
	/*
		어빌리티, 스킬사용시
			bool로 시작시 적용인지 체크(기본 true)
			bool로 지속시간이 존재하는지 체크(기본 false)
			bool로 파티전체에 적용하는지 체크(기본 false)
			int로 종족을 차별하는지 체크(기본 0)
			int로 속성을 존재하는지 체크(기본 0)

			공격력증가
			스킬데미지증가
			직공증가
			파워플립데미지 증가
			피버게이지 증가
			다운용이성 증가
			상태이상면역
			상태이상중인 적에게 추가데미지(이건느 힘들듯)
			HP증가
			내성증가
			콤보증가
			연격효과(부딧힐때 콤보+2)
			버프의 지속시간 증가

			특수한 조건부 어빌리티는 일단 제외(n회 플립시, n콤보이상시,스킬발동시,특정상태일시,혼신,배수,메인의 속성)
			여유되면 조건을 리스트로 작성
	*/
	btn = CreateWidget<CEditorButton>("저장하기");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SaveButtonCallback);
	return true;
}

void CTestWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
}

void CTestWindow::CharSprButtonCallback()
{
	m_Image[0]->SetTexture("Char_sprite_change", TEXT("Character//zeta//pixelart//animated//neutral.gif"));
	m_Image[0]->SetSize(34.f, 26.f);
}

void CTestWindow::SkillSprButtonCallback()
{
	m_Image[1]->SetTexture("Skill_sprite_change", TEXT("Character//zeta//pixelart//animated//special.gif"));
	m_Image[1]->SetSize(200.f, 200.f);
}

void CTestWindow::SkillBarButtonCallback()
{
	m_Image[2]->SetTexture("Skill_bar_change", TEXT("Character//zeta//ui//battle_control_board_1.png"));
	m_Image[2]->SetSize(104.f, 268.f);
}

void CTestWindow::SkillImgButtonCallback()
{
	m_Image[3]->SetTexture("Skill_image_change", TEXT("Character//zeta//ui//skill_cutin_1.png"));
	m_Image[3]->SetSize(512.f, 256.f);
}

void CTestWindow::CharAllButtonCallback()
{
	m_Image[4]->SetTexture("Char_all_change", TEXT("Character//zeta//ui//full_shot_1440_1920_1_resized.png"));
	m_Image[4]->SetSize(500.f, 500.f);
}

void CTestWindow::SaveButtonCallback()
{
	MessageBox(0, TEXT("저장했습니다"), TEXT("저장 완료"), MB_OK);
}

void CTestWindow::LoadButtonCallback()
{
	MessageBox(0, TEXT("불러왔습니다"), TEXT("불러오기 완료"), MB_OK);
}

void CTestWindow::InputCallback()
{
	char	text[256] = {};
	sprintf_s(text, "%.5f\n", m_Input->GetFloat());
	OutputDebugStringA(text);
}

void CTestWindow::ListCallback(int selectIndex, const std::string& item)
{
	char	text[256] = {};
	sprintf_s(text, "%d : %s\n", selectIndex, item.c_str());
	OutputDebugStringA(text);
}

void CTestWindow::TreeCallback(CEditorTreeItem<int>* node, const std::string& item)
{
	char	text[256] = {};
	sprintf_s(text, "%s\n", item.c_str());
	OutputDebugStringA(text);
}

/*
	init
		strcpy_s(m_AddText, " Failed");
		m_AddCount = (int)strlen(m_AddText);
	update
		if (m_AddIndex < m_AddCount)
		{
			m_AddTime += deltaTime;

			if (m_AddTime >= 1.f)
			{
				m_AddTime -= 1.f;
				char	text[2] = {};
				text[0] = m_AddText[m_AddIndex];
				m_Text->AddText(text);
				++m_AddIndex;
			}
		}
*/