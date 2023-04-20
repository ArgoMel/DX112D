#pragma once
#include "EditorWidget.h"
class CEditorText :public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::string		m_Text;
	std::wstring	m_wText;
	std::string		m_TextUTF8;
protected:
	CEditorText();
	virtual ~CEditorText();
public:
	const std::string& GetText()	const
	{
		return m_Text;
	}
	const std::wstring& GetWText()	const
	{
		return m_wText;
	}
	const std::string& GetTextUTF8()	const
	{
		return m_TextUTF8;
	}
	void SetText(const std::string& text);
	void AddText(const std::string& text);
	virtual bool Init();
	virtual void Render();
};

