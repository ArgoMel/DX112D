#pragma once
#include "Editor\EditorWindow.h"

class CCameraWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_CameraCB;
	class CEditorComboBox* m_TargetArmCB;
	class CEditorInputText* m_CameraViewDistance;
	class CEditorInputText* m_TargetArmOffset[3];
	class CEditorInputText* m_TargetDistance;
	CSharedPtr<class CCameraComponent>	m_SelectCamera;
	CSharedPtr<class CTargetArm>	m_SelectArm;
	CSharedPtr<class CSpriteComponent>	m_SelectSprite;
	int m_CameraCBIndex;
	int m_TargetArmCBIndex;
	void CameraCreateCallback();
	void CameraChangeCallback();
	void TargetArmChangeCallback();
	void CameraCBClickCallback(int index, const std::string& item);
	void TargetArmCBClickCallback(int index, const std::string& item);
protected:
	CCameraWindow();
	virtual ~CCameraWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	void SetSelectSprite(class CSpriteComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

