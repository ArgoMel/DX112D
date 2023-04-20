#include "Mesh.h"
#include "../../Device.h"
#include "../Material/Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"

CMesh::CMesh()
	: m_Scene(nullptr)
	, m_Min(FLT_MAX, FLT_MAX, FLT_MAX)
	, m_Max(FLT_MIN, FLT_MIN, FLT_MIN)
	, m_MeshType(MeshType::Sprite)
{
}

CMesh::~CMesh()
{
	size_t	size = m_vecContainer.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecMeshSlot[i]);
	}
}

void CMesh::SetMaterial(int container, int subset, const std::string& name)
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
	m_vecContainer[container]->vecSubset[subset].material = material;
	m_vecContainer[container]->vecSubset[subset].slot->material = material;
}

void CMesh::SetMaterial(int container, int subset, CMaterial* material)
{
	m_vecContainer[container]->vecSubset[subset].material = material;
	m_vecContainer[container]->vecSubset[subset].slot->material = material;
}

bool CMesh::CreateMesh(void* vtxData, int size, int count, D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,
	void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	MeshContainer* container = new MeshContainer;
	container->VB.size = size;
	container->VB.count = count;
	container->primitive = primitive;
	int dataSize = size * count;
	container->VB.data = new char[dataSize];
	memcpy(container->VB.data, vtxData, dataSize);
	if (!CreateBuffer(BufferType::Vertex, vtxData, size, count, vtxUsage, &container->VB.buffer))
	{
		return false;
	}
	m_vecContainer.push_back(container);
	MeshSlot* slot = new MeshSlot;
	m_vecMeshSlot.push_back(slot);
	slot->VB = &container->VB;
	slot->primitive = container->primitive;
	if (idxData != nullptr)
	{
		MeshSubset	subset;
		container->vecSubset.push_back(subset);
		int index = (int)container->vecSubset.size() - 1;
		container->vecSubset[index].slot = slot;
		slot->IB = &container->vecSubset[index].IB;
		slot->IB->size = idxSize;
		slot->IB->count = idxCount;
		slot->IB->fmt = fmt;
		dataSize = idxSize * idxCount;
		slot->IB->data = new char[dataSize];
		memcpy(slot->IB->data, idxData, dataSize);
		if (!CreateBuffer(BufferType::Index, idxData, idxSize, idxCount, idxUsage, &slot->IB->buffer))
		{
			return false;
		}
	}
	return true;
}

void CMesh::Render()
{
	size_t	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		unsigned int	stride = (unsigned int)m_vecMeshSlot[i]->VB->size;
		unsigned int	offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,&m_vecMeshSlot[i]->VB->buffer, &stride, &offset);
		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecMeshSlot[i]->IB->buffer, m_vecMeshSlot[i]->IB->fmt,0);
			CDevice::GetInst()->GetContext()->DrawIndexed(m_vecMeshSlot[i]->IB->count, 0, 0);
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(m_vecMeshSlot[i]->VB->count, 0);
		}
	}
}

void CMesh::Render(int slotNumber)
{
	unsigned int	stride = (unsigned int)m_vecMeshSlot[slotNumber]->VB->size;
	unsigned int	offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[slotNumber]->primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1,&m_vecMeshSlot[slotNumber]->VB->buffer, &stride, &offset);
	if (m_vecMeshSlot[slotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->
			IASetIndexBuffer(m_vecMeshSlot[slotNumber]->IB->buffer, m_vecMeshSlot[slotNumber]->IB->fmt,0);
		CDevice::GetInst()->GetContext()->DrawIndexed(m_vecMeshSlot[slotNumber]->IB->count, 0, 0);
	}
	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->Draw(m_vecMeshSlot[slotNumber]->VB->count, 0);
	}
}

void CMesh::RenderInstancing(int count)
{
	size_t	size = m_vecMeshSlot.size();
	for (size_t i = 0; i < size; ++i)
	{
		unsigned int	stride = (unsigned int)m_vecMeshSlot[i]->VB->size;
		unsigned int	offset = 0;
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[i]->primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshSlot[i]->VB->buffer, &stride, &offset);
		if (m_vecMeshSlot[i]->IB)
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecMeshSlot[i]->IB->buffer, m_vecMeshSlot[i]->IB->fmt, 0);
			CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecMeshSlot[i]->IB->count, count, 0, 0, 0);
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecMeshSlot[i]->VB->count, count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(int count, int slotNumber)
{
	unsigned int	stride = (unsigned int)m_vecMeshSlot[slotNumber]->VB->size;
	unsigned int	offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecMeshSlot[slotNumber]->primitive);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecMeshSlot[slotNumber]->VB->buffer, &stride, &offset);
	if (m_vecMeshSlot[slotNumber]->IB)
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(
			m_vecMeshSlot[slotNumber]->IB->buffer, m_vecMeshSlot[slotNumber]->IB->fmt,0);
		CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecMeshSlot[slotNumber]->IB->count, count, 0, 0, 0);
	}
	else
	{
		CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
		CDevice::GetInst()->GetContext()->DrawInstanced(m_vecMeshSlot[slotNumber]->VB->count, count, 0, 0);
	}
}

bool CMesh::CreateBuffer(BufferType type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	D3D11_BUFFER_DESC	desc = {};
	desc.ByteWidth = size * count;
	desc.Usage = usage;
	if (type == BufferType::Vertex)
	{
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}
	if (usage == D3D11_USAGE_DYNAMIC)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (usage == D3D11_USAGE_STAGING)
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	D3D11_SUBRESOURCE_DATA	bufferData = {};
	bufferData.pSysMem = data;
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
	{
		return false;
	}
	if (type == BufferType::Vertex)
	{
		char* vertexData = (char*)data;		// 정점은 위치 12바이트가 항상 먼저 들어온다
		for (int i = 0; i < count; ++i)
		{
			Vector3* pos = (Vector3*)vertexData;
			vertexData += size;
			if (m_Min.x > pos->x)
			{
				m_Min.x = pos->x;
			}
			if (m_Min.y > pos->y)
			{
				m_Min.y = pos->y;
			}
			if (m_Min.z > pos->z)
			{
				m_Min.z = pos->z;
			}
			if (m_Max.x < pos->x)
			{
				m_Max.x = pos->x;
			}
			if (m_Max.y < pos->y)
			{
				m_Max.y = pos->y;
			}
			if (m_Max.z < pos->z)
			{
				m_Max.z = pos->z;
			}
		}
	}
	return true;
}