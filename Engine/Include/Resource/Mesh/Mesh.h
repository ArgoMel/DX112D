#pragma once
#include "../../Ref.h"
#include "../Material/Material.h"

struct MeshSubset
{
    CSharedPtr<CMaterial> material;
    struct MeshSlot* slot;
    IndexBuffer IB;
};

struct MeshContainer
{
    VertexBuffer	VB;
    std::vector<MeshSubset>	vecSubset;
    // 위상구조. 어떤 도형으로 그려낼지를 결정한다.
    D3D11_PRIMITIVE_TOPOLOGY	primitive;		
    MeshContainer()
        : primitive(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
    {
    }
};

struct MeshSlot
{
    CSharedPtr<class CMaterial>	material;
    VertexBuffer* VB;
    IndexBuffer* IB;
    D3D11_PRIMITIVE_TOPOLOGY	primitive;

    MeshSlot()
        : VB(nullptr)
        , IB(nullptr)
        , primitive(D3D11_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
    {
    }
};

class CMesh :public CRef
{
protected:
    friend class CMeshManager;
    class CScene* m_Scene;
    std::vector<MeshContainer*> m_vecContainer;
    std::vector<MeshSlot*>      m_vecMeshSlot;
    Vector3 m_Min;
    Vector3 m_Max;
    MeshType    m_MeshType;
    CMesh();
    virtual ~CMesh() = 0;
    bool CreateBuffer(BufferType type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer);
public:
    int GetSlotCount()  const
    {
        return (int)m_vecMeshSlot.size();
    }
    CMaterial* GetMaterial(int Slot)	const
    {
        return m_vecMeshSlot[Slot]->material;
    }
    const Vector3& GetMin() const
    {
        return m_Min;
    }
    const Vector3& GetMax() const
    {
        return m_Max;
    }
    MeshType GetMeshType()  const
    {
        return m_MeshType;
    }
    Vector3 GetMeshSize()    const
    {
        return m_Max - m_Min;
    }
    void SetScene(class CScene* scene)
    {
        m_Scene = scene;
    }
    void SetMaterial(int container, int subset, const std::string& name);
    void SetMaterial(int container, int subset, class CMaterial* material);
    virtual bool CreateMesh(void* vtxData, int size, int count, D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,
        void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
        D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
    virtual void Render();
    virtual void Render(int slotNumber);
    virtual void RenderInstancing(int count);
    virtual void RenderInstancing(int count, int slotNumber);
};

