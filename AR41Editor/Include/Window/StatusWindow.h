#pragma once
#include "Editor\EditorWindow.h"

class CStatusWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Check;
	class CEditorInputText* m_Frame;
	class CEditorInputText* m_PosX;
	class CEditorInputText* m_PosY;
	class CEditorInputText* m_PosZ;
	class CEditorInputText* m_SizeX;
	class CEditorInputText* m_SizeY;
	class CEditorInputText* m_SizeZ;
	CSharedPtr<class CSceneComponent>	m_SelectComponent;
	CSharedPtr<class CUIWidget>	m_SelectWidget;
protected:
	CStatusWindow();
	virtual ~CStatusWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	void SetSelectWidget(class CUIWidget* widget);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

