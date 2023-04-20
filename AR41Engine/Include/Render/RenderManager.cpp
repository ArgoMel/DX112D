#include "RenderManager.h"
#include "RenderState.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "../Component/SceneComponent.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneViewport.h"
DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
	m_RenderStateManager = new CRenderStateManager;
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::CreateLayer(const std::string& name, int priority)
{
	RenderLayer* layer = new RenderLayer;
	layer->name = name;
	layer->layerPriority = priority;
	m_RenderLayerList.push_back(layer);
	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& name, int priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			(*iter)->layerPriority = priority;
			break;
		}
	}
	std::sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerAlphaBlend(const std::string& name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			(*iter)->alphaBlend = FindRenderState<CRenderState>("AlphaBlend");
			break;
		}
	}
}

void CRenderManager::DeleteLayer(const std::string& name)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == name)
		{
			SAFE_DELETE((*iter));
			iter = m_RenderLayerList.erase(iter);
			break;
		}
	}
}

void CRenderManager::AddRenderList(CSceneComponent* component)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->name == component->GetRenderLayerName())
		{
			(*iter)->renderList.push_back(component);
			break;
		}
	}
}

bool CRenderManager::Init()
{
	if (!m_RenderStateManager->Init())
	{
		return false;
	}
	CreateLayer("Effect", 0);
	SetLayerAlphaBlend("Effect");
	CreateLayer("Player", 1);
	SetLayerAlphaBlend("Player");
	CreateLayer("Default", 2);
	SetLayerAlphaBlend("Default");
	CreateLayer("Back", 9);
	m_AlphaBlend = m_RenderStateManager->FindRenderState<CBlendState>("AlphaBlend");
	m_DepthDisable = m_RenderStateManager->FindRenderState<CDepthStencilState>("DepthDisable");
	return true;
}

void CRenderManager::Render(float deltaTime)
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();
		for (; iter != iterEnd; ++iter)
		{
			auto	iter1 = (*iter)->renderList.begin();
			auto	iter1End = (*iter)->renderList.end();
			if ((*iter)->alphaBlend)
			{
				(*iter)->alphaBlend->SetState();
			}
			for (; iter1 != iter1End;)
			{
				if ((*iter1)->GetTypeName() == "class CColliderPixel"&& (*iter1)->GetRefCount()==1)
				{
					(*iter1)->Destroy();
				}
				if (!(*iter1)->GetActive())
				{
					iter1 = (*iter)->renderList.erase(iter1);
					iter1End = (*iter)->renderList.end();
					continue;
				}
				else if (!(*iter1)->GetEnable())
				{
					++iter1;
					continue;
				}
				(*iter1)->Render();
				++iter1;
			}
			if ((*iter)->alphaBlend)
			{
				(*iter)->alphaBlend->ResetState();
			}
		}
	}
	//UI
	m_AlphaBlend->SetState();
	m_DepthDisable->SetState();
	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();
	m_DepthDisable->ResetState();
	m_AlphaBlend->ResetState();
}

void CRenderManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& name, bool blendEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, 
	D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphBlend, D3D11_BLEND destAlphBlend, D3D11_BLEND_OP blendAlphOp, UINT8 writeMask)
{
	m_RenderStateManager->AddBlendInfo(name, blendEnable, srcBlend, destBlend, blendOp,	
		srcAlphBlend, destAlphBlend, blendAlphOp, writeMask);
}

bool CRenderManager::CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(name, alphaToCoverageEnable, independentBlendEnable);
}

bool CRenderManager::CreateDepthStencil(const std::string& name, bool depthEnable, D3D11_DEPTH_WRITE_MASK depthWriteMask,
	D3D11_COMPARISON_FUNC depthFunc, bool stencilEnable, UINT8 stencilReadMask, UINT8 stencilWriteMask, 
	D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
	return m_RenderStateManager->CreateDepthStencil(name, depthEnable, depthWriteMask, depthFunc, 
		stencilEnable, stencilReadMask, stencilWriteMask, frontFace, backFace);
}

bool CRenderManager::SortLayer(RenderLayer* src, RenderLayer* dest)
{
	return src->layerPriority < dest->layerPriority;	//오름차순
}