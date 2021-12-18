//*****************************************************************************
//!	@file	CS_LuminanceExtraction.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/31 作成：木村優
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

// 書き読み込みテクスチャ
RWTexture2D<float4> inTexture  : register(u0);
RWTexture2D<float4> outTexture : register(u1);

// 閾値(しきい値)
static const float threshold = 1.3f;

[RootSignature(RS_CS_LuminanceExtraction)]
[numthreads(8, 8, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID) {
    float4 color = inTexture[DTid.xy];

    outTexture[DTid.xy] = color;

    float gumma = GetGumma(float3(color.rgb));
    // 閾値以上の色を出力
    if (gumma < threshold)
    {
        outTexture[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}
