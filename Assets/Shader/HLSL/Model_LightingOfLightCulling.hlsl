//*****************************************************************************
//!	@file	Model_LightingOfLightCulling.hlsl
//!	@brief	
//!	@note	
//!	@author	2022/01/24 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
    float4 posw	    : POSITION0;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

RWStructuredBuffer<uint> lightIndices : register(u1);

[RootSignature(RS_MODEL_LIGHTING_OF_LIGHTCULLING)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.posw = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.posw, matVP);

    output.color = input.color;
    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3)matW);

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    output.color = diffuseTex.Sample(sampleState, input.uv);
    if (output.color.a <= 0.1f)
    {
        discard;
    }

    output.color = all(output.color.xyz) ? output.color * material.diffuse : material.diffuse;

    float3 l = normalize(directionalLight.pos.xyz);
    float3 n = normalize(input.normal);
    float d = HalfLambert(n, l);
    float4 dLight = directionalLight.diffuse * d;

    const int TILE_X = 16;
    const int TILE_Y = 16;

    uint numCellX = (textureWidth + TILE_X - 1) / TILE_X;
    
    float2 viewportPos = input.pos.xy;

    uint tileIndex = floor(viewportPos.x / TILE_X) + floor(viewportPos.y / TILE_X) * numCellX;

    uint start = tileIndex * numPointLight;

    uint end = start + numPointLight;

    float4 pLight;
    for (int i = start; i < end; ++i) {

        uint lightNo = lightIndices[i];
        if (lightNo == 0xffffffff) {
            break;
        }

        PointLight pointLight = pointLights[lightNo];

        float3 l = pointLight.pos - input.posw.xyz;
        float len = length(l);

        l = normalize(l);
        // åıåπÇ∆ñ@ê¸ÇÃì‡êœÇåvéZ
        float d = HalfLambert(n, l);
        // å∏êä
        float affect = 1.0f - min(1.0f, len / pointLight.radius);

        pLight += (pointLight.diffuse * d) * affect;
    }

    output.color *= dLight + pLight;

    //float3 v = normalize(cameraPos.xyz - input.posw.xyz);
    //output.color.xyz = BRDF(output.color.xyz, 1.0f, 0.0f, dLight.xyz + pLight.xyz, n, v);

    return output;
}