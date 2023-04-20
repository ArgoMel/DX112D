#pragma once
#include "Editor/EditorWindow.h"
#include "Editor/EditorTree.h"

class CTestWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorImage* m_Image[5];
	class CEditorText* m_Text;
	class CEditorInputText* m_Input;
	class CEditorListBox* m_List;
	class CEditorComboBox* m_Combo;
	CEditorTree<int>* m_Tree;
	void CharSprButtonCallback();
	void SkillSprButtonCallback();
	void SkillBarButtonCallback();
	void SkillImgButtonCallback();
	void CharAllButtonCallback();
	void SaveButtonCallback();
	void LoadButtonCallback();
	void InputCallback();
	void ListCallback(int selectIndex, const std::string& item);
	void TreeCallback(CEditorTreeItem<int>* node, const std::string& item);
protected:
	CTestWindow();
	virtual ~CTestWindow();
public:
	virtual bool Init();
	virtual void Update(float deltaTime);
};

//char	m_AddText[1024];
//int		m_AddIndex;
//int		m_AddCount;
//float	m_AddTime;