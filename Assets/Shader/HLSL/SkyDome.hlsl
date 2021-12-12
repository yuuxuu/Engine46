//*****************************************************************************
//!	@file  SkyDome.hlsl
//!	@brief
//!	@note
//!	@author	2021/12/11 çÏê¨ÅFñÿë∫óD
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
    float4 posw     : TEXCOORD1;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.posw = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.posw, matVP);

    output.color = input.color;

    return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    // 3éüå≥íºåç¿ïWÇ©ÇÁ2éüå≥ã…ç¿ïWÇ÷ïœä∑
    float2 uv;
    {
        float r = sqrt((input.posw.x * input.posw.x) + (input.posw.y * input.posw.y) + (input.posw.z * input.posw.z));

        float theta = atan2(input.posw.z, input.posw.x);
        float phi = acos(input.posw.y / r);

        uv.x = theta / (2.0f * PI);
        uv.y = phi / PI;
    }

    output.color = diffuseTex.Sample(sampleState, uv);

    return output;
}