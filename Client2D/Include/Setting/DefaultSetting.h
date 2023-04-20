#pragma once
#include "EngineSetting.h"

class CDefaultSetting : public CEngineSetting
{
private:
    void CreateCDO();
    void LoadResource();
    void SetInput();
    void SetCollision();
public:
    CDefaultSetting();
    virtual ~CDefaultSetting();
    virtual void Init();
};

