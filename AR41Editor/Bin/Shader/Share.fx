/*
    새 항목-유틸리티-텍스트파일-
	ar41engine -bin-shader 에 생성
	생성후 속성-빌드에서 제외
*/

struct PS_OUTPUT_SINGLE
{
    float4 Color : SV_TARGET;
};

cbuffer Transform : register(b0)
{
    matrix  g_matWorld; //월드
    matrix  g_matView;  //뷰
    matrix  g_matProj;  //투영
    matrix  g_matWVP;
    float3  g_Pivot;
    float  g_TransformEmpty1;
    float3  g_MeshSize;
    float  g_TransformEmpty2;
};

cbuffer Material : register(b1)
{
    float4 g_MtrlBaseColor;
    float4 g_MtrlAmbientColor;
    float4 g_MtrlSpecularColor;
    float4 g_MtrlEmissiveColor;
    float g_MtrlOpacity;
    int g_MtrlTextureType;
    float g_MtrlTextureWidth;
    float g_MtrlTextureHeight;
};

SamplerState g_PointSmp : register(s0);
SamplerState g_LinearSmp : register(s1);
SamplerState g_AnisotropicSmp : register(s2);
Texture2D g_BaseTexture : register(t0);

cbuffer Animation2D : register(b2)
{
    float g_Anim2DImageWidth;
    float g_Anim2DImageHeight;
    float2 g_Anim2DFrameStart;
    float2 g_Anim2DFrameEnd;
    int g_Anim2DType;
    int g_Anim2DEnable;
    int g_Anim2DFrame;
    float3 g_Anim2DEmpty;
};

#define Anim2D_None -1
#define Anim2D_Atlas 0
#define Anim2D_Frame 1
#define Anim2D_Array 2
#define Image_Atlas 0
#define Image_Frame 1
#define Image_Array 2

float2 UpdateAnimation2D(float2 UV)
{
    if (g_Anim2DEnable == 0)
    {
        return UV;
    }
    float2 result = (float2) 0;
    if (g_Anim2DType == Anim2D_Atlas)
    {
        if (UV.x == 0.f)
        {
            result.x = g_Anim2DFrameStart.x / g_Anim2DImageWidth;
        }
        else
        {
            result.x = g_Anim2DFrameEnd.x / g_Anim2DImageWidth;
        }
        if (UV.y == 0.f)
        {
            result.y = g_Anim2DFrameStart.y / g_Anim2DImageHeight;
        }
        else
        {
            result.y = g_Anim2DFrameEnd.y / g_Anim2DImageHeight;
        }
    }
    else
    {
        result = UV;
    }
    return result;
}