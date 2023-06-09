#include "ResourceManager.h"
#include "Mesh/MeshManager.h"
#include "Texture/TextureManager.h"
#include "Shader/UIProgressBarConstantBuffer.h"
#include "../UI/UIWidget.h"
#include "../UI/UIProgressBar.h"
DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager()
{
	m_ShaderManager = new CShaderManager;
	m_TextureManager = new CTextureManager;
	m_MaterialManager = new CMaterialManager;
	m_MeshManager = new CMeshManager;
	m_AnimationManager = new CAnimationManager;
	m_SoundManager = new CSoundManager;
	m_FontManager = new CFontManager;
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(CUIProgressBar::m_ProgressBarCBuffer);
	SAFE_DELETE(CUIWidget::m_CBuffer);
	SAFE_DELETE(CUIWidget::m_AnimCBuffer);
	SAFE_DELETE(m_FontManager);
	SAFE_DELETE(m_SoundManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_MaterialManager);
	SAFE_DELETE(m_TextureManager);
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MeshManager);
}

bool CResourceManager::Init()
{
	m_ShaderManager->Init();
	CUIWidget::m_CBuffer = new CUIConstantBuffer;
	CUIWidget::m_CBuffer->Init();
	CUIWidget::m_AnimCBuffer = new CAnimation2DConstantBuffer;
	CUIWidget::m_AnimCBuffer->Init();
	CUIProgressBar::m_ProgressBarCBuffer = new CUIProgressBarConstantBuffer;
	CUIProgressBar::m_ProgressBarCBuffer->Init();
	m_TextureManager->Init();
	m_MaterialManager->Init();
	m_MeshManager->Init();
	m_AnimationManager->Init();
	m_SoundManager->Init();
	m_FontManager->Init();
	return true;
}

void CResourceManager::Update()
{
	m_SoundManager->Update();
}

bool CResourceManager::CreateMesh(CScene* scene, MeshType type, const std::string& name, void* vtxData, int size
	, int count, D3D11_USAGE vtxUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, 
	D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	return m_MeshManager->CreateMesh(scene, type, name, vtxData, size,count, 
		vtxUsage, primitive, idxData, idxSize, idxCount, idxUsage,fmt);
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{
	return m_MeshManager->FindMesh(name);
}

void CResourceManager::ReleaseMesh(const std::string& name)
{
	m_MeshManager->ReleaseMesh(name);
}

CColliderConstantBuffer* CResourceManager::GetColliderCBuffer() const
{
	return m_ShaderManager->GetColliderCBuffer();
}

CShader* CResourceManager::FindShader(const std::string& name)
{
	return m_ShaderManager->FindShader(name);
}

void CResourceManager::ReleaseShader(const std::string& name)
{
	m_ShaderManager->ReleaseShader(name);
}

bool CResourceManager::CreateConstantBuffer(const std::string& name, int size, int reg, int shaderBufferType)
{
	return m_ShaderManager->CreateConstantBuffer(name, size, reg, shaderBufferType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& name)
{
	return m_ShaderManager->FindConstantBuffer(name);
}

bool CResourceManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return m_TextureManager->LoadTexture(name, fileName, pathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	return m_TextureManager->LoadTextureFullPath(name, fullPath);
}

bool CResourceManager::LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName 
	, const std::string& pathName)
{
	return m_TextureManager->LoadTexture(name, vecFileName, pathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	return m_TextureManager->LoadTextureFullPath(name, vecFullPath);
}

bool CResourceManager::LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	return m_TextureManager->LoadTextureArray(name, vecFileName, pathName);
}

bool CResourceManager::LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	return m_TextureManager->LoadTextureArrayFullPath(name, vecFullPath);
}

CTexture* CResourceManager::FindTexture(const std::string& name)
{
	return m_TextureManager->FindTexture(name);
}

void CResourceManager::ReleaseTexture(const std::string& name)
{
	m_TextureManager->ReleaseTexture(name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& name)
{
	return m_MaterialManager->FindMaterial(name);
}

void CResourceManager::ReleaseMaterial(const std::string& name)
{
	m_MaterialManager->ReleaseMaterial(name);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const TCHAR* fileName, const std::string& pathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, textureName, fileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, texture);
}

bool CResourceManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
	const TCHAR* fullPath)
{
	return m_AnimationManager->CreateAnimationSequence2DFullPath(name, textureName, fullPath);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, textureName, vecFileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	return m_AnimationManager->CreateAnimationSequence2DFullPath(name, textureName, vecFullPath);
}

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end)
{
	return m_AnimationManager->AddSequence2DFrame(name, start, end);
}

bool CResourceManager::AddAnimationSequence2DFrame(const std::string& name, float startX, float startY
	, float endX, float endY)
{
	return m_AnimationManager->AddSequence2DFrame(name, startX, startY, endX, endY);
}

bool CResourceManager::AddAnimationSequence2DFrameAll(const std::string& name, int count
	, const Vector2& start, const Vector2& end)
{
	return m_AnimationManager->AddSequence2DFrameAll(name, count, start, end);
}

bool CResourceManager::AddAnimationSequence2DFrameAll(const std::string& name, int count, float startX, float startY,
	float endX, float endY)
{
	return m_AnimationManager->AddSequence2DFrameAll(name, count, startX, startY, endX, endY);
}

bool CResourceManager::SaveSequence2D(const std::string& name, const char* fullPath)
{
	return m_AnimationManager->SaveSequence2D(name, fullPath);
}

bool CResourceManager::LoadSequence2D(const std::string& name, const char* fullPath)
{
	return m_AnimationManager->LoadSequence2D(name, fullPath);
}

bool CResourceManager::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return m_AnimationManager->SaveSequence2D(name, fileName, pathName);
}

bool CResourceManager::LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return m_AnimationManager->LoadSequence2D(name, fileName, pathName);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& name)
{
	return m_AnimationManager->FindAnimationSequence2D(name);
}

void CResourceManager::ReleaseAnimationSequence2D(const std::string& name)
{
	m_AnimationManager->ReleaseAnimationSequence2D(name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnim2DConstantBuffer() const
{
	return m_AnimationManager->GetAnim2DConstantBuffer();
}

bool CResourceManager::CreateSoundChannel(const std::string& name)
{
	return m_SoundManager->CreateSoundChannel(name);
}

bool CResourceManager::LoadSound(const std::string& groupName, const std::string& name, bool loop, const char* fileName
	, const std::string& pathName)
{
	return m_SoundManager->LoadSound(groupName, name, loop, fileName, pathName);
}

bool CResourceManager::SetVolume(int volume)
{
	return m_SoundManager->SetVolume(volume);
}

bool CResourceManager::SetVolume(const std::string& groupName, int volume)
{
	return m_SoundManager->SetVolume(groupName, volume);
}

bool CResourceManager::SoundPlay(const std::string& name)
{
	return m_SoundManager->SoundPlay(name);
}

bool CResourceManager::SoundStop(const std::string& name)
{
	return m_SoundManager->SoundStop(name);
}

bool CResourceManager::SoundPause(const std::string& name)
{
	return m_SoundManager->SoundPause(name);
}

bool CResourceManager::SoundResume(const std::string& name)
{
	return m_SoundManager->SoundResume(name);
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& name)
{
	return m_SoundManager->FindChannelGroup(name);
}

CSound* CResourceManager::FindSound(const std::string& name)
{
	return m_SoundManager->FindSound(name);
}

void CResourceManager::ReleaseSound(const std::string& name)
{
	m_SoundManager->ReleaseSound(name);
}

bool CResourceManager::CreateFontCollection(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return m_FontManager->CreateFontCollection(name, fileName, pathName);
}

bool CResourceManager::LoadFont(const std::string& name, const TCHAR* fontName, int weight, float fontSize, 
	const TCHAR* localName, int stretch)
{
	return m_FontManager->LoadFont(name, fontName, weight, fontSize, localName, stretch);
}

const TCHAR* CResourceManager::GetFontFaceName(const std::string& collectionName)
{
	return m_FontManager->GetFontFaceName(collectionName);
}

const char* CResourceManager::GetFontFaceNameMultibyte(const std::string& collectionName)
{
	return m_FontManager->GetFontFaceNameMultibyte(collectionName);
}

bool CResourceManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const Vector4& color)
{
	return m_FontManager->CreateFontColor(color);
}

bool CResourceManager::CreateFontColor(unsigned int color)
{
	return m_FontManager->CreateFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(float r, float g, float b, float a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& color)
{
	return m_FontManager->FindFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned int color)
{
	return m_FontManager->FindFontColor(color);
}

unsigned int CResourceManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(const Vector4& color)
{
	return m_FontManager->CreateFontColorKey(color);
}

CFont* CResourceManager::FindFont(const std::string& name)
{
	return m_FontManager->FindFont(name);
}

CFontCollection* CResourceManager::FindFontCollection(const std::string& name)
{
	return m_FontManager->FindFontCollection(name);
}

void CResourceManager::ReleaseFont(const std::string& name)
{
	m_FontManager->ReleaseFont(name);
}

void CResourceManager::ReleaseFontCollection(const std::string& name)
{
	m_FontManager->ReleaseFontCollection(name);
}
