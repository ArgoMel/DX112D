#pragma once
#include "../EngineInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Font/Font.h"
#include "../Resource/Font/FontCollection.h"

class CSceneResource
{
private:
	friend class CScene;
	class CScene* m_Owner;
	std::unordered_map<std::string, CSharedPtr<CMesh>>		m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>>	m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CMaterial>>	m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapAnimationSequence2D;
	std::unordered_map<std::string, CSharedPtr<CSound>>	m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;
	std::unordered_map<std::string, CSharedPtr<CFontCollection>>	m_mapFontCollection;
	CSceneResource();
	~CSceneResource();
public:
	bool Init();
// ===================== Mesh =========================
	bool CreateMesh(MeshType type, const std::string& name, void* vtxData, int size, int count,
		D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, 
		int idxCount = 0, D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,	DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	class CMesh* FindMesh(const std::string& name);
// ===================== Shader =========================
	class CShader* FindShader(const std::string& name);
// ===================== Texture =========================
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	bool LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath);
	class CTexture* FindTexture(const std::string& name);
// ===================== Material =========================
	CMaterial* FindMaterial(const std::string& name);
	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		if (FindMaterial(name))
		{
			return true;
		}
		if (!CResourceManager::GetInst()->CreateMaterial<T>(name))
		{
			return false;
		}
		m_mapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));
		return true;
	}
// ===================== Animation =========================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, const TCHAR* fullPath);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end);
	bool AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY);
	bool AddAnimationSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end);
	bool AddAnimationSequence2DFrameAll(const std::string& name, int count, float startX, float startY,
		float endX, float endY);
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(const std::string& name, const char* fullPath);
	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	bool LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
// ============================ Sound ================================
	bool CreateSoundChannel(const std::string& name);
	bool LoadSound(const std::string& groupName, const std::string& name,
		bool loop, const char* fileName, const std::string& pathName = SOUND_PATH);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	class CSound* FindSound(const std::string& name);
// ============================ Font ================================
	bool CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName = FONT_PATH);
	bool LoadFont(const std::string& name, const TCHAR* fontName,
		int weight, float fontSize, const TCHAR* localName, int stretch = 5);
	const TCHAR* GetFontFaceName(const std::string& collectionName);
	const char* GetFontFaceNameMultibyte(const std::string& collectionName);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(unsigned int color);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int color);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& color);
	CFont* FindFont(const std::string& name);
	CFontCollection* FindFontCollection(const std::string& name);
};
