#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	CMaterial* material = CreateMaterial<CMaterial>("DefaultColor");
	material->SetShader("SpriteColorShader");

	material = CreateMaterial<CMaterial>("alk");
	material->SetShader("SpriteShader");
	material->AddTexture(0, (int)EShaderBufferType::Pixel, "alk", 
		TEXT("Character\\alk\\pixelart\\animated\\neutral.gif"));

	material = CreateMaterial<CMaterial>("TileMap");
	std::vector<const TCHAR*>	vecFileNames;
	for (int i = 0; i <= 379; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor_%d.png"), i);
		vecFileNames.push_back(fileName);
	}
	material->SetShader("TileMapShader");
	material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultTileIsometric", vecFileNames);
	material->SetRenderState("DepthLessEqual");
	material->SetRenderState("AlphaBlend");
	for (int i = 0; i <= 379; ++i)
	{
		SAFE_DELETE_ARRAY(vecFileNames[i]);
	}

	material = CreateMaterial<CMaterial>("DefaultTileMapBack");
	material->SetShader("TileMapBackShader");
	material->SetRenderState("DepthLessEqual");
	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& name)
{
	auto	iter = m_mapMaterial.find(name);
	if (iter == m_mapMaterial.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& name)
{
	auto	iter = m_mapMaterial.find(name);
	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMaterial.erase(iter);
		}
	}
}