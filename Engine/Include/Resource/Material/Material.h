#pragma once
#include "../../Ref.h"
#include "../Texture/Texture.h"
#include "../../Render/RenderState.h"

struct MaterialTextureInfo
{
    CSharedPtr<class CTexture>  texture;
    std::string     name;
    ESamplerType    samplerType;
    int             reg;
    int             shaderBufferType;
    int             index;

    MaterialTextureInfo() 
        : samplerType(ESamplerType::Linear)
        , reg(0)
        , shaderBufferType((int)EShaderBufferType::Pixel)
        , index(0)
    {
    }
};

class CMaterial : public CRef
{
private:
    friend class CMaterialManager;
    friend class CMesh;
    friend class CPrimitiveComponent;
    class CScene* m_Scene;
protected:
    class CMaterialConstantBuffer* m_CBuffer;
    CSharedPtr<class CShader>   m_Shader;
    CSharedPtr<CRenderState>    m_RenderState[3];
    std::string     m_NullName;
    Vector4     m_BaseColor;
    Vector4     m_AmbientColor;
    Vector4     m_SpecularColor;
    Vector4     m_EmissiveColor;    // 자체적으로 빛을 발산하는 물체일 경우
    float       m_Opacity;          // 불투명도.
    std::vector<MaterialTextureInfo*>   m_vecTextureInfo;
    CMaterial();
    CMaterial(const CMaterial& material);
    virtual ~CMaterial();
public:
    class CTexture* GetTexture(int index = 0)   const;
    float GetOpacity()   const
    {
        return m_Opacity;
    }
    void SetScene(class CScene* scene)
    {
        m_Scene = scene;
    }
    void SetRenderState(const std::string& name);
    void SetShader(const std::string& name);
    void SetMaterial();
    void ResetMaterial();
    CMaterial* Clone()  const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);

    void SetBaseColor(const Vector4& color);
    void SetBaseColor(float r, float g, float b, float a);
    void SetBaseColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetAmbientColor(const Vector4& color);
    void SetAmbientColor(float r, float g, float b, float a);
    void SetAmbientColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetSpecularColor(const Vector4& color);
    void SetSpecularColor(float r, float g, float b, float a);
    void SetSpecularColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetEmissiveColor(const Vector4& color);
    void SetEmissiveColor(float r, float g, float b, float a);
    void SetEmissiveColorUnsignedChar(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetOpacity(float opacity);
    void AddOpacity(float opacity);
// === Texture 추가 ===
    void AddTexture(int reg, int shaderBufferType, const std::string& name, class CTexture* texture);
    void AddTexture(int reg, int shaderBufferType, const std::string& name, const TCHAR* fileName, 
        const std::string& pathName = TEXTURE_PATH);
    void AddTextureFullPath(int reg, int shaderBufferType, const std::string& name, const TCHAR* fullPath);
    void AddTexture(int reg, int shaderBufferType, const std::string& name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& pathName = TEXTURE_PATH);
    void AddTextureFullPath(int reg, int shaderBufferType, const std::string& name, 
        const std::vector<const TCHAR*>& vecFullPath);
    void AddTextureArray(int reg, int shaderBufferType, const std::string& name,
        const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    void AddTextureArrayFullPath(int reg, int shaderBufferType, const std::string& name, 
        const std::vector<const TCHAR*>& vecFullPath);
// === 추가되어 있는 Texture 변경 ===
    void SetTexture(int index, int reg, int shaderBufferType, const std::string& name, class CTexture* texture);
    void SetTexture(int index, int reg, int shaderBufferType, const std::string& name, const TCHAR* fileName,
        const std::string& pathName = TEXTURE_PATH);
    void SetTextureFullPath(int index, int reg, int shaderBufferType, const std::string& name, const TCHAR* fullPath);
    void SetTexture(int index, int reg, int shaderBufferType, const std::string& name, 
        const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    void SetTextureFullPath(int index, int reg, int shaderBufferType, 
        const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
    void SetTextureArray(int index, int reg, int shaderBufferType, const std::string& name,
        const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    void SetTextureArrayFullPath(int index, int reg, int shaderBufferType, const std::string& name, 
        const std::vector<const TCHAR*>& vecFullPath);
    void SetTextureSamplerType(int index, ESamplerType type);
    void SetTextureFrameIndex(int texIndex, int frameIndex);
};

