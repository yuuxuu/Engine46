//*****************************************************************************
//!	@file	CS_VerticalBlur.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/31 作成：木村優
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"

// 書き読み込みテクスチャ
RWTexture2D<float4> inTexture  : register(u0);
RWTexture2D<float4> outTexture : register(u1);

[RootSignature(RS_CS_BLUR)]
[numthreads(8, 8, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID) {
    float4 color = inTexture[uint2(DTid.x, DTid.y)];

    const int pivot = OFFSET_MAX / 2;

    uint2 uv = DTid.xy;

    for (int i = 0; i < pivot; ++i) {
        uv.x = min(DTid.x + i, texWidth);

        outTexture[uv] += color * blurOffset[i].z;
    }
    for (int i = pivot; i < OFFSET_MAX; ++i) {
        uv.x = max(DTid.x - (i - pivot), 0);

        outTexture[uv] += color * blurOffset[i].z;
    }
}
