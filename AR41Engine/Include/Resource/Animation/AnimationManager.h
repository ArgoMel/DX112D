#pragma once
#include "AnimationSequence2D.h"

class CAnimationManager
{
private:
	friend class CResourceManager;
	class CAnimation2DConstantBuffer* m_Anim2DBuffer;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>>	m_mapSequence2D;
	CAnimationManager();
	~CAnimationManager();
public:
	class CAnimation2DConstantBuffer* GetAnim2DConstantBuffer()	const
	{
		return m_Anim2DBuffer;
	}
	bool Init();
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, const TCHAR* fullPath);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const std::vector<const TCHAR*>& vecFullPath);
	bool AddSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end);
	bool AddSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY);
	bool AddSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end);
	bool AddSequence2DFrameAll(const std::string& name, int count, float startX, float startY, float endX, float endY);
	bool SaveSequence2D(const std::string& name, const char* fullPath);
	bool LoadSequence2D(const std::string& name, const char* fullPath);
	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	bool LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
	void ReleaseAnimationSequence2D(const std::string& name);
};

