#pragma once
#include "Shader.h"
class CComputeShader :public CShader
{
protected:
    friend class CShaderManager;
    CComputeShader();
    virtual ~CComputeShader() = 0;
public:
    virtual bool Init() = 0;
    virtual void SetShader();
};

