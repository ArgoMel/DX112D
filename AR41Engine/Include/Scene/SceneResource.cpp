#include "SceneResource.h"

CSceneResource::CSceneResource()
	: m_Owner(nullptr)
{
}

CSceneResource::~CSceneResource()
{
	{
		auto	iter = m_mapMesh.begin();
		auto	iterEnd = m_mapMesh.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapMesh.erase(iter);
			iterEnd = m_mapMesh.end();
			CResourceManager::GetInst()->ReleaseMesh(name);
		}
	}
	{
		auto	iter = m_mapShader.begin();
		auto	iterEnd = m_mapShader.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapShader.erase(iter);
			iterEnd = m_mapShader.end();
			CResourceManager::GetInst()->ReleaseShader(name);
		}
	}
	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();
			CResourceManager::GetInst()->ReleaseTexture(name);
		}
	}
	{
		auto	iter = m_mapAnimationSequence2D.begin();
		auto	iterEnd = m_mapAnimationSequence2D.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapAnimationSequence2D.erase(iter);
			iterEnd = m_mapAnimationSequence2D.end();
			CResourceManager::GetInst()->ReleaseAnimationSequence2D(name);
		}
	}
	{
		auto	iter = m_mapSound.begin();
		auto	iterEnd = m_mapSound.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();
			CResourceManager::GetInst()->ReleaseSound(name);
		}
	}
	{
		auto	iter = m_mapFont.begin();
		auto	iterEnd = m_mapFont.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapFont.erase(iter);
			iterEnd = m_mapFont.end();
			CResourceManager::GetInst()->ReleaseFont(name);
		}
	}
	{
		auto	iter = m_mapFontCollection.begin();
		auto	iterEnd = m_mapFontCollection.end();
		for (; iter != iterEnd;)
		{
			std::string	name = iter->first;
			iter = m_mapFontCollection.erase(iter);
			iterEnd = m_mapFontCollection.end();
			CResourceManager::GetInst()->ReleaseFontCollection(name);
		}
	}
}

bool CSceneResource::Init()
{
	return true;
}

bool CSceneResource::CreateMesh(MeshType type, const std::string& name, void* vtxData, int size, int count, 
	D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, 
	D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	if (FindMesh(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateMesh(m_Owner, type, name, vtxData, size, count,
		vtxUsage, primitive, idxData, idxSize, idxCount, idxUsage, fmt))
	{
		return false;
	}
	m_mapMesh.insert(std::make_pair(name, CResourceManager::GetInst()->FindMesh(name)));
	return true;
}

CMesh* CSceneResource::FindMesh(const std::string& name)
{
	auto	iter = m_mapMesh.find(name);
	if (iter == m_mapMesh.end())
	{
		CMesh* mesh = CResourceManager::GetInst()->FindMesh(name);
		if (!mesh)
		{
			return nullptr;
		}
		m_mapMesh.insert(std::make_pair(name, mesh));
		return mesh;
	}
	return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& name)
{
	auto	iter = m_mapShader.find(name);
	if (iter == m_mapShader.end())
	{
		CShader* shader = CResourceManager::GetInst()->FindShader(name);
		if (!shader)
		{
			return nullptr;
		}
		m_mapShader.insert(std::make_pair(name, shader));
		return shader;
	}
	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

bool CSceneResource::LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName
	, const std::string& pathName)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

bool CSceneResource::LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

bool CSceneResource::LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindTexture(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
	{
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));
	return true;
}

CTexture* CSceneResource::FindTexture(const std::string& name)
{
	auto	iter = m_mapTexture.find(name);
	if (iter == m_mapTexture.end())
	{
		CTexture* texture = CResourceManager::GetInst()->FindTexture(name);
		if (!texture)
		{
			return nullptr;
		}
		m_mapTexture.insert(std::make_pair(name, texture));
		return texture;
	}
	return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& name)
{
	auto	iter = m_mapMaterial.find(name);
	if (iter == m_mapMaterial.end())
	{
		CMaterial* material = CResourceManager::GetInst()->FindMaterial(name);
		if (!material)
		{
			return nullptr;
		}
		m_mapMaterial.insert(std::make_pair(name, material));
		return material;
	}
	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const TCHAR* fileName, const std::string& pathName)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, textureName, fileName, pathName))
	{
		return false;
	}
	m_mapAnimationSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));
	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, texture))
	{
		return false;
	}
	m_mapAnimationSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));
	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName
	, const TCHAR* fullPath)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(name, textureName, fullPath))
	{
		return false;
	}
	m_mapAnimationSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));
	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, textureName, vecFileName, pathName))
	{
		return false;
	}
	m_mapAnimationSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));
	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
	const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindAnimationSequence2D(name))
	{
		return true;
	}
	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(name, textureName, vecFullPath))
	{
		return false;
	}
	m_mapAnimationSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));
	return true;
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(name, start, end);
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& name, float startX, float startY
	, float endX, float endY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(name, startX, startY, endX, endY);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& name, int count
	, const Vector2& start, const Vector2& end)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(name, count, start, end);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& name, int count, float startX, float startY, 
	float endX, float endY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(name, count, startX, startY, endX, endY);
}

bool CSceneResource::SaveSequence2D(const std::string& name, const char* fullPath)
{
	return CResourceManager::GetInst()->SaveSequence2D(name, fullPath);
}

bool CSceneResource::LoadSequence2D(const std::string& name, const char* fullPath)
{
	return CResourceManager::GetInst()->LoadSequence2D(name, fullPath);
}

bool CSceneResource::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return CResourceManager::GetInst()->SaveSequence2D(name, fileName, pathName);
}

bool CSceneResource::LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return CResourceManager::GetInst()->LoadSequence2D(name,fileName, pathName);
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& name)
{
	auto	iter = m_mapAnimationSequence2D.find(name);
	if (iter == m_mapAnimationSequence2D.end())
	{
		CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(name);
		if (!sequence)
		{
			return nullptr;
		}
		m_mapAnimationSequence2D.insert(std::make_pair(name, sequence));
		return sequence;
	}
	return iter->second;
}

bool CSceneResource::CreateSoundChannel(const std::string& name)
{
	return CResourceManager::GetInst()->CreateSoundChannel(name);
}

bool CSceneResource::LoadSound(const std::string& groupName, const std::string& name, bool loop
	, const char* fileName, const std::string& pathName)
{
	if (FindSound(name))
	{
		return false;
	}
	if (!CResourceManager::GetInst()->LoadSound(groupName, name, loop, fileName, pathName))
	{
		return false;
	}
	CSound* sound = CResourceManager::GetInst()->FindSound(name);
	m_mapSound.insert(std::make_pair(name, sound));
	return true;
}

bool CSceneResource::SetVolume(int volume)
{
	return CResourceManager::GetInst()->SetVolume(volume);
}

bool CSceneResource::SetVolume(const std::string& groupName, int volume)
{
	return CResourceManager::GetInst()->SetVolume(groupName, volume);
}

bool CSceneResource::SoundPlay(const std::string& name)
{
	return CResourceManager::GetInst()->SoundPlay(name);
}

bool CSceneResource::SoundStop(const std::string& name)
{
	return CResourceManager::GetInst()->SoundStop(name);
}

bool CSceneResource::SoundPause(const std::string& name)
{
	return CResourceManager::GetInst()->SoundPause(name);
}

bool CSceneResource::SoundResume(const std::string& name)
{
	return CResourceManager::GetInst()->SoundResume(name);
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& name)
{
	return CResourceManager::GetInst()->FindChannelGroup(name);
}

CSound* CSceneResource::FindSound(const std::string& name)
{
	auto	iter = m_mapSound.find(name);
	if (iter == m_mapSound.end())
	{
		CSound* sound = CResourceManager::GetInst()->FindSound(name);
		if (!sound)
		{
			return nullptr;
		}
		m_mapSound.insert(std::make_pair(name, sound));
		return sound;
	}
	return iter->second;
}

bool CSceneResource::CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (FindFontCollection(name))
	{
		return false;
	}
	if (!CResourceManager::GetInst()->CreateFontCollection(name, fileName, pathName))
	{
		return false;
	}
	CFontCollection* font = CResourceManager::GetInst()->FindFontCollection(name);
	m_mapFontCollection.insert(std::make_pair(name, font));
	return true;
}

bool CSceneResource::LoadFont(const std::string& name, const TCHAR* fontName, int weight, float fontSize, 
	const TCHAR* localName, int stretch)
{
	if (FindFont(name))
	{
		return false;
	}
	if (!CResourceManager::GetInst()->LoadFont(name, fontName, weight, fontSize, localName, stretch))
	{
		return false;
	}
	CFont* font = CResourceManager::GetInst()->FindFont(name);
	m_mapFont.insert(std::make_pair(name, font));
	return true;
}

const TCHAR* CSceneResource::GetFontFaceName(const std::string& collectionName)
{
	return CResourceManager::GetInst()->GetFontFaceName(collectionName);
}

const char* CSceneResource::GetFontFaceNameMultibyte(const std::string& collectionName)
{
	return CResourceManager::GetInst()->GetFontFaceNameMultibyte(collectionName);
}

bool CSceneResource::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->CreateFontColor(r, g, b, a);
}

bool CSceneResource::CreateFontColor(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->CreateFontColor(r, g, b, a);
}

bool CSceneResource::CreateFontColor(const Vector4& color)
{
	return CResourceManager::GetInst()->CreateFontColor(color);
}

bool CSceneResource::CreateFontColor(unsigned int color)
{
	return CResourceManager::GetInst()->CreateFontColor(color);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(const Vector4& color)
{
	return CResourceManager::GetInst()->FindFontColor(color);
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned int color)
{
	return CResourceManager::GetInst()->FindFontColor(color);
}

unsigned int CSceneResource::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return CResourceManager::GetInst()->CreateFontColorKey(r, g, b, a);
}

unsigned int CSceneResource::CreateFontColorKey(float r, float g, float b, float a)
{
	return CResourceManager::GetInst()->CreateFontColorKey(r, g, b, a);
}

unsigned int CSceneResource::CreateFontColorKey(const Vector4& color)
{
	return CResourceManager::GetInst()->CreateFontColorKey(color);
}

CFont* CSceneResource::FindFont(const std::string& name)
{
	auto	iter = m_mapFont.find(name);
	if (iter == m_mapFont.end())
	{
		CFont* font = CResourceManager::GetInst()->FindFont(name);
		if (!font)
		{
			return nullptr;
		}
		m_mapFont.insert(std::make_pair(name, font));
		return font;
	}
	return iter->second;
}

CFontCollection* CSceneResource::FindFontCollection(const std::string& name)
{
	auto	iter = m_mapFontCollection.find(name);
	if (iter == m_mapFontCollection.end())
	{
		CFontCollection* font = CResourceManager::GetInst()->FindFontCollection(name);
		if (!font)
		{
			return nullptr;
		}
		m_mapFontCollection.insert(std::make_pair(name, font));
		return font;
	}
	return iter->second;
}

