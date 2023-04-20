#pragma once
#include "../EngineInfo.h"
#include "RenderStateManager.h"

struct RenderLayer
{
	CSharedPtr<class CRenderState> alphaBlend;
	std::list<CSharedPtr<class CSceneComponent>>	renderList;
	std::string		name;
	int				layerPriority=0;
};

class CRenderManager
{
	DECLARE_SINGLE(CRenderManager)
private:
	CRenderStateManager* m_RenderStateManager;
	CSharedPtr<class CRenderState> m_AlphaBlend;
	CSharedPtr<class CRenderState> m_DepthDisable;
	std::vector<RenderLayer*>	m_RenderLayerList;
	static bool SortLayer(RenderLayer* src, RenderLayer* dest);
public:
	void CreateLayer(const std::string& name, int priority);
	void SetLayerPriority(const std::string& name, int priority);
	void SetLayerAlphaBlend(const std::string& name);
	void DeleteLayer(const std::string& name);
	void AddRenderList(class CSceneComponent* component);
	bool Init();
	void Render(float deltaTime);
	void SetBlendFactor(const std::string& name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& name, bool blendEnable = true, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcAlphBlend = D3D11_BLEND_ONE,	D3D11_BLEND destAlphBlend = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendAlphOp = D3D11_BLEND_OP_ADD, UINT8 writeMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& name, bool alphaToCoverageEnable, bool independentBlendEnable);
	bool CreateDepthStencil(const std::string& name, bool depthEnable = true, 
		D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS,
		bool stencilEnable = false, UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC frontFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC backFace = 
		{ D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });
	template <typename T>
	T* FindRenderState(const std::string& name)
	{
		return m_RenderStateManager->FindRenderState<T>(name);
	}
};