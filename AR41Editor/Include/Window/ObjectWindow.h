#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"
#include "GameObject/GameObject.h"
#include "UI/UIWindow.h"

class CObjectWindow :public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	CEditorTree<CGameObject*>* m_Tree;
	CEditorTree<CUIWindow*>* m_WindowTree;
	CSharedPtr<CGameObject>	m_SelectObject;
	CSharedPtr<CUIWindow>	m_SelectWindow;
	void TreeCallback(CEditorTreeItem<CGameObject*>* node, const std::string& item);
	void UICallback(CEditorTreeItem<CUIWindow*>* node, const std::string& item);
protected:
	CObjectWindow();
	virtual ~CObjectWindow();
public:
	class CGameObject* GetSelectObject()	const
	{
		return m_SelectObject;
	}
	class CUIWindow* GetSelectWindow()	const
	{
		return m_SelectWindow;
	}
	bool AddItem(class CGameObject* object, const std::string& name, const std::string& parentName = "Root");
	bool AddItem(class CUIWindow* window, const std::string& name, const std::string& parentName = "Canvas");
	virtual bool Init();
	virtual void Update(float deltaTime);
	void Clear();
	void ClearSelect();
	void Delete();
};

