//*****************************************************************************
//!	@file	GBUffer.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/26 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../ShaderUlity.hlsli"
#include "../RootSignature.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
    float3 normal   : NORMAL;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD0;
    float3 normalw  : NORMAL;
    float4 posw     : TEXCOORD1;
};

struct PS_OUT {
    float4 diffuse  : SV_TARGET0;
    float4 specular : SV_TARGET1;
    float4 normal   : SV_TARGET2;
    float4 pos      : SV_TARGET3;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.posw = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.posw, matVP);

    output.color = input.color;
    output.uv = input.uv;

    output.normalw = mul(input.normal, (float3x3)matW);

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output;

    output.diffuse = diffuseTex.Sample(sampleState, input.uv) * material.diffuse;

    output.diffuse = all(output.diffuse.xyz) ? output.diffuse : float4(1.0f, 1.0f, 1.0f, 1.0f);

    output.specular = material.specular;

    float3 n = normalize(input.normalw);
    float depth = input.posw.z / input.posw.w;

    output.normal = float4(n * 0.5f + 0.5f, depth);

    output.pos = input.posw;

    float3 v = normalize(input.posw.xyz - cameraPos);
    float3 ref = reflect(v, n);

    //output.diffuse *= cubeTex.Sample(sampleState, ref);

    return output;
}