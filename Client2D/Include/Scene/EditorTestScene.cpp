#include "EditorTestScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"

CEditorTestScene::CEditorTestScene()
{
	m_ClassTypeName = "EditorTestScene";
}

CEditorTestScene::~CEditorTestScene()
{
}

bool CEditorTestScene::Init()
{
	//m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");
	return true;
}
