#include "Share.fx"

cbuffer ColliderCBuffer : register(b10)
{
    float4 g_ColliderColor;
    matrix g_ColliderWVP;
};

float4 ColliderVS(float3 Pos : POSITION) : SV_Position
{
    return mul(float4(Pos, 1.f), g_ColliderWVP);
}

float4 ColliderPS() : SV_TARGET
{
    return g_ColliderColor;
}

struct VS_INPUT_UV
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

VS_OUTPUT_UV ColliderImageVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;
    output.Pos = mul(float4(input.Pos, 1.f), g_ColliderWVP);
    output.UV = input.UV;
    return output;
}

float4 ColliderImagePS(VS_OUTPUT_UV input) : SV_TARGET
{
    return g_BaseTexture.Sample(g_PointSmp, input.UV);
}