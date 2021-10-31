//*****************************************************************************
//!	@file	GBuffer_Lighting.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/28 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN {
    float3 pos		: POSITION;
    float4 color	: COLOR;
    float2 uv		: TEXCOORD;
};

struct VS_OUT {
    float4 pos		: SV_POSITION;
    float4 color	: COLOR;
    float2 uv		: TEXCOORD;
};

struct PS_OUT {
    float4 color	: SV_TARGET;
};

[RootSignature(RS_GBUFFER_LIGHTING)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.0f);

    output.color = input.color;
    output.uv = input.uv;

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    float4 diffuse = diffuseTex.Sample(sampleState, input.uv);

    float4 specular = specularTex.Sample(sampleState, input.uv);

    float3 posw = posTex.Sample(sampleState, input.uv).xyz;

    float3 n = normalTex.Sample(sampleState, input.uv).xyz;
    n = normalize(n * 2 - 1);

    float depth = normalTex.Sample(sampleState, input.uv).w;

    float3 l = normalize(directionalLight.pos.xyz);
    float d = Lambert(n, l);
    float4 dLight = directionalLight.diffuse * d;

    float4 pLight;
    for (int i = 0; i < numPointLight; ++i) {
        float3 l = pointLights[i].pos - posw;
        float len = length(l);

        l = normalize(l);
        // åıåπÇ∆ñ@ê¸ÇÃì‡êœÇåvéZ
        float d = Lambert(n, l);
        // å∏êä
        float4 att = saturate(1.0f / (pointLights[i].attenuation.x + pointLights[i].attenuation.y * len + pointLights[i].attenuation.z * len * len));

        pLight += (pointLights[i].diffuse * d) * att;
    }

    output.color = diffuse * (dLight + pLight);

    return output;
}