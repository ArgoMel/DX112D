#include "ClientManager.h"
#include "Input.h"
#include "Scene/SceneManager.h"
#include "Scene/TitleSceneInfo.h"
#include "Scene/MainSceneInfo.h"
#include "Setting/EngineShareSetting.h"

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	CEngine::DestroyInst();
}

bool CClientManager::Init(HINSTANCE hInst)
{
    if (!CEngine::GetInst()->Init(hInst, TEXT("WorldFlipper_Client"), TEXT("WorldFlipper_Client"), 
        IDI_ICON1, IDI_ICON1, 1280, 720, 1920, 1080, true))
    {
        return false;
    }
    CEngineShareSetting::Setting();
    
    CSceneManager::GetInst()->GetScene()->Load("Title.scn", SCENE_PATH);
    return true;
}

int CClientManager::Run()
{
    return CEngine::GetInst()->Run();
}
