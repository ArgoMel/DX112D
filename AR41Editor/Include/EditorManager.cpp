#include "EditorManager.h"
#include "CollisionManager.h"
#include "Input.h"
#include "Scene/EditorDefaultScene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneViewport.h"
#include "Scene/Scene.h"
#include "GameObject/Player2D.h"
#include "GameObject/Bullet.h"
#include "GameObject/Monster.h"
#include "GameObject/Boss.h"
#include "Window/TestWindow.h"
#include "Window/ObjectWindow.h"
#include "Window/ClassWindow.h"
#include "Window/ComponentWindow.h"
#include "Window/TransformWindow.h"
#include "Window\SceneWindow.h"
#include "Window\DetailWindow.h"
#include "Window/ImageWindow.h"
#include "Window/AnimationWindow.h"
#include "Window/Animation2DWindow.h"
#include "Window/ColiderWindow.h"
#include "Window/ColliderPixelWindow.h"
#include "Window/ResourceWindow.h"
#include "Window/CameraWindow.h"
#include "Window/StatusWindow.h"
#include "Window/UIButtonWindow.h"
#include "Window/UIImageWindow.h"
#include "Window/UINumberWindow.h"
#include "Window/UIProgressBarWindow.h"
#include "Window/UITextWindow.h"
#include "Editor/EditorGUIManager.h"
#include "Setting/EngineShareSetting.h"

CEditorManager::CEditorManager()
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

bool CEditorManager::Init(HINSTANCE hInst)
{
    CEngine::GetInst()->EnableEditor();
    if (!CEngine::GetInst()->Init(hInst, TEXT("KBJ41_Editor"), TEXT("KBJ41_Editor"), IDI_ICON1, IDI_ICON1
        , 1280, 720, 1920, 1080, true, IDR_MENU1))
    {
        return false;
    }
    CSceneInfo* info = new CEditorDefaultScene;
    CScene::AddSceneInfoCDO("EditorDefaultScene", info);
    CEngineShareSetting::Setting();
    CEngine::SetWndProcCallback<CEditorManager>(this, &CEditorManager::WndProc);

    CEditorGUIManager::GetInst()->CreateEditorWindow<CObjectWindow>("ObjectWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CClassWindow>("ClassWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CComponentWindow>("ComponentWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CTransformWindow>("TransformWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CSceneWindow>("SceneWindow");
    //CEditorGUIManager::GetInst()->CreateEditorWindow<CResourceWindow>("ResourceWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("DetailWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CImageWindow>("ImageWindow");
    CEditorGUIManager::GetInst()->CreateEditorWindow<CStatusWindow>("StatusWindow");

    CSceneManager::GetInst()->CreateSceneInfo<CEditorDefaultScene>();

    CScene* scene = CSceneManager::GetInst()->GetScene();
    CInput::GetInst()->AddBindFunction<CEditorManager>
        ("Pause2", Input_Type::Down, this, &CEditorManager::Pause, scene);

//시작하자마자 현재씬의 컴포넌트들 출력
    CObjectWindow* objectWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if(objectWindow)
    {
        std::vector<HierarchyObjectName> vecName;
        CSceneManager::GetInst()->GetScene()->GetAllGameObjectHierarchyName(vecName);
        size_t	size = vecName.size();
        for (size_t i = 0; i < size; ++i)
        {
            objectWindow->AddItem(vecName[i].obj, vecName[i].name);
        }
        std::vector<HierarchyWindowName> vecWindowName;
        CSceneManager::GetInst()->GetScene()->GetViewport()->GetAllWindowHierarchyName(vecWindowName);
        size = vecWindowName.size();
        for (size_t i = 0; i < size; ++i)
        {
            objectWindow->AddItem(vecWindowName[i].window, vecWindowName[i].name);
        }
    }
    return true;
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::Pause()
{
    if (1.f == CEngine::GetInst()->GetTimeScale())
    {
        CEngine::GetInst()->SetTimeScale(0.f);
        return;
    }
    CEngine::GetInst()->SetTimeScale(1.f);
}

bool CEditorManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (wParam)
        {
        case ID_FILE_SCENE_SAVE:
            SaveScene();
            return true;
        case ID_FILE_SCENE_OPEN:
            OpenScene();
            return true;
        case ID_FILE_EXIT:
            DestroyWindow(hWnd);
            return true;
        case ID_OBJECT_EMPTY:
            CreateEmptyObject();
            return true;
        case ID_OBJECT:
            CreateObject();
            return true;
        case ID_WINDOW_TEST:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("TestWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("TestWindow");
            }
        }
            return true;
        case ID_WINDOW_OBJECT:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ObjectWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("ObjectWindow");
            }
        }
            return true;
        case ID_Window_Scene:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("SceneWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("SceneWindow");
            }
        }
            return true;
        case ID_Window_Detail:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("DetailWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("DetailWindow");
            }
        }
            return true;
        case ID_Window_Component:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ComponentWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CDetailWindow>("ComponentWindow");
            }
        }
            return true;
        case ID_Window_Camera:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("CameraWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CCameraWindow>("CameraWindow");
            }
        }
            return true;
        case ID_Window_Collider2D:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ColliderWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CColiderWindow>("ColliderWindow");
            }
        }
            return true;
        case ID_Window_ColliderPixel:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ColliderPixelWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CColliderPixelWindow>("ColliderPixelWindow");
            }
        }
            return true;
        case ID_Window_BUTTON:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIButtonWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIButtonWindow>("UIButtonWindow");
            }
        }
            return true;
        case ID_Window_UIImage:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIImageWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIImageWindow>("UIImageWindow");
            }
        }
            return true;  
        case ID_Window_NUMBER:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UINumberWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUINumberWindow>("UINumberWindow");
            }
        }
            return true;  
        case ID_Window_PROGRESSBAR:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UIProgressBarWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
            }
        }
            return true;
        case ID_Window_TEXT:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("UITextWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CUITextWindow>("UITextWindow");
            }
        }
            return true;
        case ID_Window_IMAGE:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("ImageWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CImageWindow>("ImageWindow");
            }
        }
            return true;
        case ID_Window_Animation:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("AnimationWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimationWindow>("AnimationWindow");
            }
        }
            return true;
        case ID_Window_Animation2D:
        {
            CEditorWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CEditorWindow>("Animation2DWindow");
            if (window)
            {
                window->Open();
            }
            else
            {
                CEditorGUIManager::GetInst()->CreateEditorWindow<CAnimation2DWindow>("Animation2DWindow");
            }
        }
            return true;
        }
        break;
    }
    return false;
}

void CEditorManager::SaveScene()
{
}

void CEditorManager::OpenScene()
{
}

void CEditorManager::CreateEmptyObject()
{
    CScene* scene = CSceneManager::GetInst()->GetScene();
    CGameObject* emptyObj = scene->CreateObject<CGameObject>("GameObjectEmpty");
    CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if (window)
    {
        window->AddItem(emptyObj,"GameObjectEmpty");
    }
}

void CEditorManager::CreateObject()
{
    CScene* scene = CSceneManager::GetInst()->GetScene();
    CGameObject* obj = nullptr;
    CClassWindow* classWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CClassWindow>("ClassWindow");
    if(!classWindow)
    {
        return;
    }
    std::string selectObjectItem = classWindow->GetSelectObjectItem();
    if (selectObjectItem == "")
    {
        return;
    }
    if (selectObjectItem == "GameObject")
    {
        obj = scene->CreateObject<CGameObject>(selectObjectItem);
    }
    else if (selectObjectItem == "Player2D")
    {
        obj = scene->CreateObject<CPlayer2D>(selectObjectItem);
    }
    else if (selectObjectItem == "Bullet")
    {
        obj = scene->CreateObject<CBullet>(selectObjectItem);
    }
    else if (selectObjectItem == "Monster")
    {
        obj = scene->CreateObject<CMonster>(selectObjectItem);
    }
    else if (selectObjectItem == "Boss")
    {
        obj = scene->CreateObject<CBoss>(selectObjectItem);
    }
    CObjectWindow* window = CEditorGUIManager::GetInst()->FindEditorWindow<CObjectWindow>("ObjectWindow");
    if (window)
    {
        window->AddItem(obj, selectObjectItem);
    }
}
