#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager()
	: m_Anim2DBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Anim2DBuffer);
}

bool CAnimationManager::Init()
{
	m_Anim2DBuffer = new CAnimation2DConstantBuffer;
	m_Anim2DBuffer->Init();
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const TCHAR* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(textureName, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(texture))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
	const TCHAR* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->InitFullPath(textureName, fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, 
	const std::vector<const TCHAR*>& vecFileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->Init(textureName, vecFileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName,
	const std::vector<const TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (sequence)
	{
		return true;
	}
	sequence = new CAnimationSequence2D;
	sequence->SetName(name);
	if (!sequence->InitFullPath(textureName, vecFullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}
	m_mapSequence2D.insert(std::make_pair(name, sequence));
	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& end)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrame(start, end);
	return true;
}

bool CAnimationManager::AddSequence2DFrame(const std::string& name, float startX, float startY, float endX, float endY)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrame(startX, startY, endX, endY);
	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& name, int count, const Vector2& start, const Vector2& end)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrameAll(count, start, end);
	return true;
}

bool CAnimationManager::AddSequence2DFrameAll(const std::string& name, int count, float startX, float startY, float endX, float endY)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	sequence->AddFrameAll(count, startX, startY, endX, endY);
	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	return sequence->Save(fullPath);
}

bool CAnimationManager::LoadSequence2D(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	bool	find = true;
	if (!sequence)
	{
		sequence = new CAnimationSequence2D;
		sequence->SetName(name);
		find = false;
	}
	if (!sequence->Load(fullPath))
	{
		return false;
	}
	if (!find)
	{
		m_mapSequence2D.insert(std::make_pair(name, sequence));
	}
	return true;
}

bool CAnimationManager::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	if (!sequence)
	{
		return false;
	}
	return sequence->Save(fileName, pathName);
}

bool CAnimationManager::LoadSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindAnimationSequence2D(name);
	bool	find = true;
	if (!sequence)
	{
		sequence = new CAnimationSequence2D;
		sequence->SetName(name);
		find = false;
	}
	if (!sequence->Load(fileName, pathName))
	{
		return false;
	}
	if (!find)
	{
		m_mapSequence2D.insert(std::make_pair(name, sequence));
	}
	return true;
}

CAnimationSequence2D* CAnimationManager::FindAnimationSequence2D(const std::string& name)
{
	auto	iter = m_mapSequence2D.find(name);
	if (iter == m_mapSequence2D.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CAnimationManager::ReleaseAnimationSequence2D(const std::string& name)
{
	auto	iter = m_mapSequence2D.find(name);
	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapSequence2D.erase(iter);
		}
	}
}