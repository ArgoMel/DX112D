#pragma once
#include "Scene\SceneInfo.h"

class CLoadingSceneInfo : public CSceneInfo
{
private:
    class CLoadingThread* m_LoadingThread;
    class CThreadQueue* m_LoadingQueue;
    class CLoadingUI* m_LoadingUI;
public:
    CLoadingSceneInfo();
    ~CLoadingSceneInfo();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void SceneChangeComplete();
};

