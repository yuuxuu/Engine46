//*****************************************************************************
//!	@file	Sprite.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/24 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

[RootSignature(RS_MODEL)]
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

    output.color = diffuseTex.Sample(sampleState, input.uv);

    return output;
}