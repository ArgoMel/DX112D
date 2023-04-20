#include "MeshManager.h"
#include "SpriteMesh.h"

CMeshManager::CMeshManager()
{
}

CMeshManager::~CMeshManager()
{
}

bool CMeshManager::Init()
{
	// 기본 메쉬 생성 센터 중심, 좌하단 중심
	VertexColor	centerColorMesh[4] =
	{
		VertexColor(Vector3(-0.5f, 0.5f, 0.f), Vector4::red),
		VertexColor(Vector3(0.5f, 0.5f, 0.f), Vector4::green),
		VertexColor(Vector3(-0.5f, -0.5f, 0.f), Vector4::blue),
		VertexColor(Vector3(0.5f, -0.5f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};
	unsigned int	rectIndex[6] = { 0, 1, 3, 0, 3, 2 };
	CreateMesh(nullptr, MeshType::Sprite, "CenterColorRect", centerColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	CMesh* mesh = FindMesh("CenterColorRect");
	mesh->SetMaterial(0, 0, "DefaultColor");

	VertexColor	LBColorMesh[4] =
	{
		VertexColor(Vector3(0.f, 1.f, 0.f), Vector4::red),
		VertexColor(Vector3(1.f, 1.f, 0.f), Vector4::green),
		VertexColor(Vector3(0.f, 0.f, 0.f), Vector4::blue),
		VertexColor(Vector3(1.f, 0.f, 0.f), Vector4(1.f, 0.f, 1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "LBColorRect", LBColorMesh, sizeof(VertexColor), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("LBColorRect");
	mesh->SetMaterial(0, 0, "DefaultColor");

	VertexUV	CenterUVMesh[4] =
	{
		VertexUV(Vector3(-0.5f, 0.5f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(0.5f, 0.5f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(-0.5f, -0.5f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(0.5f, -0.5f, 0.f), Vector2(1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "CenterUVRect", CenterUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("CenterUVRect");
	mesh->SetMaterial(0, 0, "alk");

	VertexUV	LBUVMesh[4] =
	{
		VertexUV(Vector3(0.f, 1.f, 0.f), Vector2(0.f, 0.f)),
		VertexUV(Vector3(1.f, 1.f, 0.f), Vector2(1.f, 0.f)),
		VertexUV(Vector3(0.f, 0.f, 0.f), Vector2(0.f, 1.f)),
		VertexUV(Vector3(1.f, 0.f, 0.f), Vector2(1.f, 1.f))
	};
	CreateMesh(nullptr, MeshType::Sprite, "LBUVRect", LBUVMesh, sizeof(VertexUV), 4, D3D11_USAGE_IMMUTABLE,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, 4, 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT);
	mesh = FindMesh("LBUVRect");
	mesh->SetMaterial(0, 0, "alk");

	Vector3	box2DLineMesh[5] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
		Vector3(-0.5f, 0.5f, 0.f)
	};
	CreateMesh(nullptr, MeshType::Sprite, "Box2DLineMesh", box2DLineMesh, sizeof(Vector3), 5, 
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	Vector3	sphere2DLineMesh[361];
	sphere2DLineMesh[0].x = 1.f;
	sphere2DLineMesh[0].y = 0.f;
	for (int i = 0; i < 360; ++i)
	{
		float radian = (i + 1) * PI / 180.f;
		sphere2DLineMesh[i + 1].x = cosf(radian);
		sphere2DLineMesh[i + 1].y = sinf(radian);
	}
	sphere2DLineMesh[360].x = 1.f;
	sphere2DLineMesh[360].y = 0.f;
	CreateMesh(nullptr, MeshType::Sprite, "Sphere2DLineMesh", sphere2DLineMesh, sizeof(Vector3), 361, 
		D3D11_USAGE_IMMUTABLE, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	return true;
}

bool CMeshManager::CreateMesh(CScene* scene, MeshType type, const std::string& name, void* vtxData, int size, int count, 
    D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, 
    D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	if (FindMesh(name))
	{
		return true;
	}
	CMesh* mesh = nullptr;
	switch (type)
	{
	case MeshType::Sprite:
		mesh = new CSpriteMesh;
		break;
	case MeshType::UI:
		break;
	case MeshType::Static:
		break;
	case MeshType::Animation:
		break;
	}
	mesh->SetScene(scene);
	mesh->SetName(name);
	if (!mesh->CreateMesh(vtxData, size, count, vtxUsage, primitive,idxData, idxSize, idxCount, idxUsage, fmt))
	{
		SAFE_RELEASE(mesh);
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, mesh));
	return true;
}

CMesh* CMeshManager::FindMesh(const std::string& name)
{
	auto	iter = m_mapMesh.find(name);
	if (iter == m_mapMesh.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CMeshManager::ReleaseMesh(const std::string& name)
{
	auto	iter = m_mapMesh.find(name);
	if (iter != m_mapMesh.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMesh.erase(iter);
		}
	}
}
