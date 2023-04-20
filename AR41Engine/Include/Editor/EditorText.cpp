#include "EditorText.h"

CEditorText::CEditorText()
{
}

CEditorText::~CEditorText()
{
}

void CEditorText::SetText(const std::string& text)
{
	TCHAR	wText[256] = {};
	int length = (int)MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, wText, length);
	char	textUTF8[256] = {};
	length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
	m_Text = text;
	m_wText = wText;
	m_TextUTF8 = textUTF8;
}

void CEditorText::AddText(const std::string& text)
{
	SetText(m_Text + text);
}

bool CEditorText::Init()
{
	return true;
}

void CEditorText::Render()
{
	//ImGui::Text(m_TextUTF8.c_str());
	ImGui::TextColored(m_Color, m_TextUTF8.c_str());
}
