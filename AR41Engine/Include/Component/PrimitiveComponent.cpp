#include "PrimitiveComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"

CPrimitiveComponent::CPrimitiveComponent()
{
	SetTypeID<CPrimitiveComponent>();
	m_ComponentTypeName = "PrimitiveComponent";
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& component)
	: CSceneComponent(component)
{
	m_Mesh = component.m_Mesh;
	size_t	size = m_vecMaterial.size();
	for (size_t i = 0; i < size; ++i)
	{
		CMaterial* material = component.m_vecMaterial[i]->Clone();
		m_vecMaterial.push_back(material);
	}
}

CPrimitiveComponent::~CPrimitiveComponent()
{
}

void CPrimitiveComponent::SetMesh(const std::string& name)
{
	if (m_Scene)
	{
		m_Mesh = m_Scene->GetResource()->FindMesh(name);
	}
	else
	{
		m_Mesh = CResourceManager::GetInst()->FindMesh(name);
	}
	if (m_Mesh)
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	m_vecMaterial.clear();
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
}

void CPrimitiveComponent::SetMesh(CMesh* mesh)
{
	m_Mesh = mesh;
	if (m_Mesh) 
	{
		SetMeshSize(m_Mesh->GetMeshSize());
	}
	int slotCount = m_Mesh->GetSlotCount();
	for (int i = 0; i < slotCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		m_vecMaterial.push_back(material->Clone());
	}
}

void CPrimitiveComponent::SetMaterial(int slot, const std::string& name)
{
	CMaterial* material = nullptr;
	if (m_Scene)
	{
		material = m_Scene->GetResource()->FindMaterial(name);
	}
	else
	{
		material = CResourceManager::GetInst()->FindMaterial(name);
	}
	m_vecMaterial[slot] = material->Clone();
}

void CPrimitiveComponent::SetMaterial(int slot, CMaterial* material)
{
	if (material)
	{
		m_vecMaterial[slot] = material->Clone();
	}
	else
	{
		m_vecMaterial[slot] = material;
	}
}

void CPrimitiveComponent::AddMaterial(const std::string& name)
{
	CMaterial* material = nullptr;
	if (m_Scene)
	{
		material = m_Scene->GetResource()->FindMaterial(name);
	}
	else
	{
		material = CResourceManager::GetInst()->FindMaterial(name);
	}
	m_vecMaterial.push_back(material->Clone());
}

void CPrimitiveComponent::AddMaterial(CMaterial* material)
{
	m_vecMaterial.push_back(material->Clone());
}

void CPrimitiveComponent::ClearMaterial()
{
	m_vecMaterial.clear();
}

void CPrimitiveComponent::Start()
{
	CSceneComponent::Start();
	// Scene에 배치가 되고 Start가 호출되면 출력 목록으로 지정한다.
	CRenderManager::GetInst()->AddRenderList(this);
}

bool CPrimitiveComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		return false;
	}
	return true;
}

void CPrimitiveComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CPrimitiveComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CPrimitiveComponent::Render()
{
	CSceneComponent::Render();
	int	size = (int)m_vecMaterial.size();
	for (int i = 0; i < size; ++i)
	{
		m_vecMaterial[i]->SetMaterial();
		m_Mesh->Render(i);
		m_vecMaterial[i]->ResetMaterial();
	}
}

CPrimitiveComponent* CPrimitiveComponent::Clone() const
{
	return new CPrimitiveComponent(*this);
}

void CPrimitiveComponent::Save(FILE* file)
{
	CSceneComponent::Save(file);
	int	length = (int)m_Mesh->GetName().length();
	fwrite(&length, 4, 1, file);
	fwrite(m_Mesh->GetName().c_str(), 1, length, file);
	int	materialCount = (int)m_vecMaterial.size();
	fwrite(&materialCount, 4, 1, file);
	for (int i = 0; i < materialCount; ++i)
	{
		m_vecMaterial[i]->Save(file);
	}
}

void CPrimitiveComponent::Load(FILE* file)
{
	CSceneComponent::Load(file);
	int	length = 0;
	char	meshName[256] = {};
	fread(&length, 4, 1, file);
	assert(length >= 0);
	fread(meshName, 1, length, file);
	SetMesh(meshName);
	int	materialCount = 0;
	fread(&materialCount, 4, 1, file);
	m_vecMaterial.clear();
	for (int i = 0; i < materialCount; ++i)
	{
		CMaterial* material = m_Mesh->GetMaterial(i);
		material = material->Clone();
		material->SetScene(m_Scene);
		material->Load(file);
		m_vecMaterial.push_back(material);
	}
}
