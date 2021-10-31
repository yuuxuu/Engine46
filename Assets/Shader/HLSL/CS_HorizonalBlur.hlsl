//*****************************************************************************
//!	@file	CS_Blur.hlsl
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
    float4 color = inTexture[uint2(DTid.x * 2, DTid.y * 2)];

    const int pivot = OFFSET_MAX / 2;

    uint2 uv = DTid.xy;

    for (int i = 0; i < pivot; ++i) {
        uv.y = min(DTid.y + i, texHeight);

        outTexture[uv] += color * blurOffset[i].z;
    }
    for (int i = pivot; i < OFFSET_MAX; ++i) {
        uv.y = max(DTid.y - (i - pivot), 0);

        outTexture[uv] += color * blurOffset[i].z;
    }
}
