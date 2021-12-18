//*****************************************************************************
//!	@file	PostEffect_Bloom.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/24 作成：木村優
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

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

// 閾値(しきい値)
static const float threshold = 0.9f;

[RootSignature(RS_LuminanceExtraction)]
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

    float gumma = GetGumma(output.color.rgb);
    // 閾値以上の色を出力
    if (gumma < threshold) discard;

    return output;
}
