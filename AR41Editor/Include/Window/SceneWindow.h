#pragma once
#include "Editor\EditorWindow.h"

class CSceneWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_SceneList;
	class CEditorListBox* m_SceneInfoList;
	class CEditorInputText* m_SceneSaveName;
	class CEditorInputText* m_SceneSelectName;
	std::string			m_SelectSceneItem;
	std::string			m_SelectSceneInfoItem;
	void SceneChange();
	void SceneSave();
	void SceneCreate();
	void SceneClickCallback(int index, const std::string& item);
	void SceneInfoClickCallback(int index, const std::string& item);
	void LoadSceneDirectory();
	void LoadSceneInfoDirectory();
protected:
	CSceneWindow();
	virtual ~CSceneWindow();
public:
	const std::string& GetSelectSceneItem()	const
	{
		return m_SelectSceneItem;
	}
	virtual bool Init();
	virtual void Update(float deltaTime);
};

