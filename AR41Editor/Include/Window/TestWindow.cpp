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
	m_Tree->AddItem(0, "��¥ �ֻ��� �θ�");
	m_Tree->AddItem(0, "�ڽ�1", "��¥ �ֻ��� �θ�");
	m_Tree->AddItem(0, "�ڽ�2", "Root");
	m_Tree->AddItem(0, "�ڽ�3", "�ڽ�2");
	m_Tree->SetSelectCallback<CTestWindow>(this, &CTestWindow::TreeCallback);

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("����� ĳ���� ����");
	m_Text->SetColor(255, 255, 0, 255);
	m_List = CreateWidget<CEditorListBox>("ListBox");
	m_List->SetHideName("ListBox");
	m_List->AddItem("Alk");
	m_List->AddItem("Zeta");
	m_List->AddItem("Marina");
	m_List->AddItem("Kohane");

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("�̹��� ����");
	m_Text->SetColor(255, 255, 0, 255);

	CEditorButton* btn = CreateWidget<CEditorButton>("ĳ���� ��������Ʈ");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::CharSprButtonCallback);
	btn->SetSize(150.f,30.f);
	CEditorSameLine* sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[0] = CreateWidget<CEditorImage>("Char_sprite");
	m_Image[0]->SetTexture("Char_sprite", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("��ų ��������Ʈ");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillSprButtonCallback);
	btn->SetSize(150.f, 30.f);
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[1] = CreateWidget<CEditorImage>("Skill_sprite");
	m_Image[1]->SetTexture("Skill_sprite", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("��ų ��");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillBarButtonCallback);
	btn->SetSize(150.f, 30.f);
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Image[2] = CreateWidget<CEditorImage>("Skill_bar");
	m_Image[2]->SetTexture("Skill_bar", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("��ų Ŀư");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::SkillImgButtonCallback);
	btn->SetSize(150.f, 30.f);
	m_Image[3] = CreateWidget<CEditorImage>("Skill_image");
	m_Image[3]->SetTexture("Skill_image", TEXT("teemo.png"));

	btn = CreateWidget<CEditorButton>("ĳ���� ����");
	btn->SetClickCallback<CTestWindow>(this, &CTestWindow::CharAllButtonCallback);
	btn->SetSize(150.f, 30.f);
	m_Image[4] = CreateWidget<CEditorImage>("Char_all");
	m_Image[4]->SetTexture("Char_all", TEXT("teemo.png"));


	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("���� ����");
	m_Text->SetColor(255, 255, 0, 255);

	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("��ų ����");
	m_Text->SetColor(255,255,0,255);
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("���ӽð� ����");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("��Ƽ���Ե� ����");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("����ȿ��");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("�Ϻ� �������� ����");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("�Ϻ� �Ӽ����� ����");
	m_Text = CreateWidget<CEditorText>("Text");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(132.f);
	m_Combo = CreateWidget<CEditorComboBox>("ComboBox");
	m_Combo->SetSelectPrevViewName(true);
	m_Combo->SetHideName("ComboBox");
	m_Combo->SetPrevViewName("�Ӽ�");
	m_Combo->AddItem("ȭ");
	m_Combo->AddItem("��");
	m_Combo->AddItem("��");
	m_Combo->AddItem("��");
	m_Combo->AddItem("��");
	m_Combo->AddItem("��");
	m_Text->SetText("�����̻� ����");
	m_Text = CreateWidget<CEditorText>("Text");
	m_Text->SetText("���������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo1");
	m_Input->SetHideName("SkillInfo1");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("���ݷ�����");
	m_Text->SetText("���ݷ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo2");
	m_Input->SetHideName("SkillInfo2");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("��ų������");
	m_Text->SetText("��ų������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo3");
	m_Input->SetHideName("SkillInfo3");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�������ݵ�����");
	m_Text->SetText("�������ݵ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(122.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo4");
	m_Input->SetHideName("SkillInfo4");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�Ŀ��ø�������");
	m_Text->SetText("�Ŀ��ø�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(122.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo5");
	m_Input->SetHideName("SkillInfo5");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�ٿ���̼�");
	m_Text->SetText("�ٿ���̼�");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo6");
	m_Input->SetHideName("SkillInfo6");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�ǹ�������");
	m_Text->SetText("�ǹ�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(150.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo7");
	m_Input->SetHideName("SkillInfo7");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�����̻��� ������ �߰�������");
	m_Text->SetText("�����̻��� ������ �߰�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(30.f);
	//�����̻� üũ�� �Է�
	m_Input = CreateWidget<CEditorInputText>("SkillInfo8");
	m_Input->SetHideName("SkillInfo8");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("ü������");
	m_Text->SetText("ü������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo9");
	m_Input->SetHideName("SkillInfo9");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("��������");
	m_Text->SetText("��������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo10");
	m_Input->SetHideName("SkillInfo10");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�޺�����");
	m_Text->SetText("�޺�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(164.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo11");
	m_Input->SetHideName("SkillInfo11");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("������ ���ӽð� ����");
	m_Text->SetText("������ ���ӽð� ����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(86.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo12");
	m_Input->SetHideName("SkillInfo12");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	m_Text = CreateWidget<CEditorText>("�̵��ӵ�����");
	m_Text->SetText("�̵��ӵ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(136.f);
	m_Input = CreateWidget<CEditorInputText>("SkillInfo13");
	m_Input->SetHideName("SkillInfo13");
	m_Input->SetSizeX(120.f);
	m_Input->SetSizeY(26.f);
	m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetHintText("0");
	//��ų�� �ݶ��̴��� �̸��ٿ��� �ʿ� �����س��� �ű⼭ ui�� �������� ���

	m_Text = CreateWidget<CEditorText>("�����Ƽ1 ����");
	m_Text->SetText("�����Ƽ1 ����");
	m_Text->SetColor(255, 255, 0, 255);
	m_Text = CreateWidget<CEditorText>("���۽� ����");
	m_Text->SetText("���۽� ����");
	m_Text = CreateWidget<CEditorText>("���ӽð� ����");
	m_Text->SetText("���ӽð� ����");
	m_Text = CreateWidget<CEditorText>("��Ƽ���Ե� ����");
	m_Text->SetText("��Ƽ���Ե� ����");
	m_Text = CreateWidget<CEditorText>("����ȿ��");
	m_Text->SetText("����ȿ��");
	m_Text = CreateWidget<CEditorText>("�Ϻ� �������� ����");
	m_Text->SetText("�Ϻ� �������� ����");
	m_Text = CreateWidget<CEditorText>("�Ϻ� �Ӽ����� ����");
	m_Text->SetText("�Ϻ� �Ӽ����� ����");
	m_Text = CreateWidget<CEditorText>("�����̻� �鿪");
	m_Text->SetText("�����̻� �鿪");
	m_Text = CreateWidget<CEditorText>("�����̻� ����");
	m_Text->SetText("�����̻� ����");
	m_Text = CreateWidget<CEditorText>("���ݷ�����");
	m_Text->SetText("���ݷ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("���ݷ�");
	m_Input->SetHideName("���ݷ�");
	//inputText->SetSizeX(10.f);	//�ȸ���
	m_Text = CreateWidget<CEditorText>("��ų������");
	m_Text->SetText("��ų������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("��ų������");
	m_Input->SetHideName("��ų������");
	m_Text = CreateWidget<CEditorText>("�������ݵ�����");
	m_Text->SetText("�������ݵ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�������ݵ�����");
	m_Input->SetHideName("�������ݵ�����");
	m_Text = CreateWidget<CEditorText>("�Ŀ��ø�������");
	m_Text->SetText("�Ŀ��ø�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�Ŀ��ø�������");
	m_Input->SetHideName("�Ŀ��ø�������");
	m_Text = CreateWidget<CEditorText>("�ٿ���̼�");
	m_Text->SetText("�ٿ���̼�");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�ٿ���̼�");
	m_Input->SetHideName("�ٿ���̼�");
	m_Text = CreateWidget<CEditorText>("�ǹ�������");
	m_Text->SetText("�ǹ�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�ǹ�������");
	m_Input->SetHideName("�ǹ�������");
	m_Text = CreateWidget<CEditorText>("�����̻��� ������ �߰�������");
	m_Text->SetText("�����̻��� ������ �߰�������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	//�����̻� üũ�� �Է�
	m_Input = CreateWidget<CEditorInputText>("������굥����");
	m_Input->SetHideName("������굥����");
	m_Text = CreateWidget<CEditorText>("ü������");
	m_Text->SetText("ü������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("ü������");
	m_Input->SetHideName("ü������");
	m_Text = CreateWidget<CEditorText>("��������");
	m_Text->SetText("��������");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("��������");
	m_Input->SetHideName("��������");
	m_Text = CreateWidget<CEditorText>("�޺�����");
	m_Text->SetText("�޺�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�޺�����");
	m_Input->SetHideName("�޺�����");
	m_Text = CreateWidget<CEditorText>("������ ���ӽð� ����");
	m_Text->SetText("������ ���ӽð� ����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("���ӽð�����");
	m_Input->SetHideName("���ӽð�����");
	m_Text = CreateWidget<CEditorText>("�̵��ӵ�����");
	m_Text->SetText("�̵��ӵ�����");
	sameLine = CreateWidget<CEditorSameLine>("Line");
	sameLine->SetSpacing(20.f);
	m_Input = CreateWidget<CEditorInputText>("�̵��ӵ�����");
	m_Input->SetHideName("�̵��ӵ�����");

	m_Text = CreateWidget<CEditorText>("�����Ƽ2 ����");
	m_Text->SetText("�����Ƽ2 ����");
	m_Text->SetColor(255, 255, 0, 255);

	m_Text = CreateWidget<CEditorText>("�����Ƽ3 ����");
	m_Text->SetText("�����Ƽ3 ����");
	m_Text->SetColor(255, 255, 0, 255);
	/*
		�����Ƽ, ��ų����
			bool�� ���۽� �������� üũ(�⺻ true)
			bool�� ���ӽð��� �����ϴ��� üũ(�⺻ false)
			bool�� ��Ƽ��ü�� �����ϴ��� üũ(�⺻ false)
			int�� ������ �����ϴ��� üũ(�⺻ 0)
			int�� �Ӽ��� �����ϴ��� üũ(�⺻ 0)

			���ݷ�����
			��ų����������
			��������
			�Ŀ��ø������� ����
			�ǹ������� ����
			�ٿ���̼� ����
			�����̻�鿪
			�����̻����� ������ �߰�������(�̰Ǵ� �����)
			HP����
			��������
			�޺�����
			����ȿ��(�ε����� �޺�+2)
			������ ���ӽð� ����

			Ư���� ���Ǻ� �����Ƽ�� �ϴ� ����(nȸ �ø���, n�޺��̻��,��ų�ߵ���,Ư�������Ͻ�,ȥ��,���,������ �Ӽ�)
			�����Ǹ� ������ ����Ʈ�� �ۼ�
	*/
	btn = CreateWidget<CEditorButton>("�����ϱ�");
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
	MessageBox(0, TEXT("�����߽��ϴ�"), TEXT("���� �Ϸ�"), MB_OK);
}

void CTestWindow::LoadButtonCallback()
{
	MessageBox(0, TEXT("�ҷ��Խ��ϴ�"), TEXT("�ҷ����� �Ϸ�"), MB_OK);
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