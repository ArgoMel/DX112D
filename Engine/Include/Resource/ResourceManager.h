#pragma once
#include "../EngineInfo.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"
#include "Animation/AnimationManager.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"

class CResourceManager
{
	DECLARE_SINGLE(CResourceManager)
private:
	class CMeshManager* m_MeshManager;
	class CTextureManager* m_TextureManager;
	CMaterialManager* m_MaterialManager;
	CShaderManager* m_ShaderManager;
	CAnimationManager* m_AnimationManager;
	CSoundManager* m_SoundManager;
	CFontManager* m_FontManager;
public:
	bool Init();
	void Update();
// ===================== Mesh =========================
	bool CreateMesh(class CScene* scene, MeshType type, const std::string& name,void* vtxData, int size, int count,
		D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive,void* idxData = nullptr, int idxSize = 0, 
		int idxCount = 0, D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT,DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);
// ===================== Shader =========================
	class CColliderConstantBuffer* GetColliderCBuffer()	const;
	class CShader* FindShader(const std::string& name);
	void ReleaseShader(const std::string& name);
	bool CreateConstantBuffer(const std::string& name, int size, int reg, int shaderBufferType = 
		(int)EShaderBufferType::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& name);
	template <typename T>
	bool CreateShader(const std::string& name, bool globalShader = false)
	{
		return m_ShaderManager->CreateShader<T>(name, globalShader);
	}
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
	void ReleaseTexture(const std::string& name);
// ===================== Material =========================
	CMaterial* FindMaterial(const std::string& name);
	void ReleaseMaterial(const std::string& name);
	template <typename T>
	T* CreateMaterial(const std::string& name)
	{
		return m_MaterialManager->CreateMaterial<T>(name);
	}
// ===================== Animation =========================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const TCHAR* fullPath);
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
	void ReleaseAnimationSequence2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const;
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
	void ReleaseSound(const std::string& name);
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
	void ReleaseFont(const std::string& name);
	void ReleaseFontCollection(const std::string& name);
};
