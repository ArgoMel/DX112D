#pragma once
#include "ConstantBufferData.h"
class CMaterialConstantBuffer : public CConstantBufferData
{
private:
	MaterialCBuffer	m_BufferData;
public:
	void SetBaseColor(const Vector4& color)
	{
		m_BufferData.baseColor = color;
	}
	void SetAmbientColor(const Vector4& color)
	{
		m_BufferData.ambientColor = color;
	}
	void SetSpecularColor(const Vector4& color)
	{
		m_BufferData.specularColor = color;
	}
	void SetEmissiveColor(const Vector4& color)
	{
		m_BufferData.emissiveColor = color;
	}
	void SetOpacity(float opacity)
	{
		m_BufferData.opacity = opacity;
	}
	void SetImageType(EImageType type)
	{
		m_BufferData.textureType = (int)type;
	}
	void SetTextureWidth(float width)
	{
		m_BufferData.textureWidth = width;
	}
	void SetTextureHeight(float height)
	{
		m_BufferData.textureHeight = height;
	}
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer);
	virtual ~CMaterialConstantBuffer();
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CMaterialConstantBuffer* Clone();
};

