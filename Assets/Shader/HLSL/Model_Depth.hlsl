//*****************************************************************************
//!	@file	Model.hlsl
//!	@brief	
//!	@note	
//!	@author	2022/01/24 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
};

[RootSignature(RS_MODEL_Depth)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.pos = mul(float4(input.pos, 1.0f), matW);
    output.pos = mul(output.pos, matVP);

    return output;
}
