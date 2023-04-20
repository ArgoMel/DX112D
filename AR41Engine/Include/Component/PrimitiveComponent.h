#pragma once
#include "SceneComponent.h"
class CPrimitiveComponent :public CSceneComponent
{
protected:
    friend class CGameObject;
    friend class CScene;
    CSharedPtr<class CMesh> m_Mesh;
    std::vector<CSharedPtr<class CMaterial>>    m_vecMaterial;
    CPrimitiveComponent();
    CPrimitiveComponent(const CPrimitiveComponent& component);
    virtual ~CPrimitiveComponent();
public:
    class CMaterial* GetMaterial(int slot)
    {
        return m_vecMaterial[slot];
    }
    void SetMesh(const std::string& name);
    void SetMesh(class CMesh* mesh);
    void SetMaterial(int slot, const std::string& name);
    void SetMaterial(int slot, class CMaterial* material);
    void AddMaterial(const std::string& name);
    void AddMaterial(class CMaterial* material);
    void ClearMaterial();
    virtual void Start();
    virtual bool Init();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CPrimitiveComponent* Clone()    const;
    virtual void Save(FILE* file);
    virtual void Load(FILE* file);
};

