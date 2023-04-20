#pragma once
#include "../../EngineInfo.h"

class CMeshManager
{
private:
	friend class CResourceManager;
	std::unordered_map<std::string, CSharedPtr<class CMesh>>	m_mapMesh;
	CMeshManager();
	~CMeshManager();
public:
	bool Init();
	bool CreateMesh(class CScene* scene, MeshType type, const std::string& name,void* vtxData, int size, int count,
		D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);
};

