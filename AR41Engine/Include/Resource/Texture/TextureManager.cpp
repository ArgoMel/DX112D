#include "TextureManager.h"
#include "Texture.h"
#include "../../Device.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
	auto	iter = m_mapSampler.begin();
	auto	iterEnd = m_mapSampler.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	float BorderColor[4] = { 1.f, 1.f,1.f, 1.f };
	if (FAILED((int)CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_WRAP,	D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
	{
		return false;
	}
	if (FAILED((int)CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
	{
		return false;
	}
	if (FAILED((int)CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, 
		D3D11_TEXTURE_ADDRESS_WRAP,D3D11_TEXTURE_ADDRESS_WRAP, BorderColor)))
	{
		return false;
	}
	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);
	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTexture(name, fileName, pathName))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTextureFullPath(name, fullPath))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTexture(name, vecFileName, pathName))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTextureFullPath(name, vecFullPath))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

bool CTextureManager::LoadTextureArray(const std::string& name, const std::vector<const TCHAR*>& vecFileName, 
	const std::string& pathName)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTextureArray(name, vecFileName, pathName))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

bool CTextureManager::LoadTextureArrayFullPath(const std::string& name, const std::vector<const TCHAR*>& vecFullPath)
{
	CTexture* texture = FindTexture(name);
	if (texture)
	{
		return true;
	}
	texture = new CTexture;
	if (!texture->LoadTextureArrayFullPath(name, vecFullPath))
	{
		SAFE_DELETE(texture);
		return false;
	}
	m_mapTexture.insert(std::make_pair(name, texture));
	return true;
}

CTexture* CTextureManager::FindTexture(const std::string& name)
{
	auto	iter = m_mapTexture.find(name);
	if (iter == m_mapTexture.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CTextureManager::ReleaseTexture(const std::string& name)
{
	auto	iter = m_mapTexture.find(name);
	if (iter != m_mapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapTexture.erase(iter);
		}
	}
}

bool CTextureManager::CreateSampler(const std::string& name, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, 
	D3D11_TEXTURE_ADDRESS_MODE addressV, D3D11_TEXTURE_ADDRESS_MODE addressW, float borderColor[4])
{
	ID3D11SamplerState* sampler = FindSampler(name);
	if (sampler)
	{
		return true;
	}
	D3D11_SAMPLER_DESC	desc = {};
	desc.Filter = filter;
	desc.AddressU = addressU;
	desc.AddressV = addressV;
	desc.AddressW = addressW;
	desc.MipLODBias = 0.f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	memcpy(desc.BorderColor, borderColor, sizeof(float) * 4);
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&desc, &sampler)))
	{
		return false;
	}
	m_mapSampler.insert(std::make_pair(name, sampler));
	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& name)
{
	auto	iter = m_mapSampler.find(name);
	if (iter == m_mapSampler.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CTextureManager::SetSampler(const std::string& name, int reg, int shaderBufferType)
{
	ID3D11SamplerState* sampler = FindSampler(name);
	if (!sampler)
	{
		return;
	}
	if (shaderBufferType & (int)EShaderBufferType::Vertex)
	{
		CDevice::GetInst()->GetContext()->VSSetSamplers(reg, 1, &sampler);
	}
	if (shaderBufferType & (int)EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetSamplers(reg, 1, &sampler);
	}
	if (shaderBufferType & (int)EShaderBufferType::Hull)
	{
		CDevice::GetInst()->GetContext()->HSSetSamplers(reg, 1, &sampler);
	}
	if (shaderBufferType & (int)EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetSamplers(reg, 1, &sampler);
	}
	if (shaderBufferType & (int)EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetSamplers(reg, 1, &sampler);
	}
	if (shaderBufferType & (int)EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetSamplers(reg, 1, &sampler);
	}
}