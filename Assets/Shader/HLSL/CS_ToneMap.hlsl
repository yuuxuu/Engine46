//*****************************************************************************
//!	@file  CS_ToneMap.hlsl
//!	@brief
//!	@note
//!	@author	2020/12/08 作成：木村優
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

// テクスチャ
RWTexture2D<float4> inTexture  : register(u0);
RWTexture2D<float4> outTexture : register(u1);

// 共有メモリ
groupshared float4 sharedPixel[8 * 8];

static const float keyValue = 0.18f;

float4 Reinhard(float4 color) {
    return color / (1.0f + color);
}

float4 GammaCorrection(float4 color, float gamma) {
    return pow(color, 1.0f / gamma);
}

[RootSignature(RS_CS_ToneMap)]
[numthreads(8, 8, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID, uint GI : SV_GroupIndex) {
    
    //sharedPixel[GI] = inTexture[DTid.xy];
    //GroupMemoryBarrierWithGroupSync();

    //if (GI < 32)
    //    sharedPixel[GI] += sharedPixel[32 + GI];
    //GroupMemoryBarrierWithGroupSync();
    //if (GI < 16)
    //    sharedPixel[GI] += sharedPixel[16 + GI];
    //GroupMemoryBarrierWithGroupSync();
    //if (GI < 8)
    //    sharedPixel[GI] += sharedPixel[8 + GI];
    //GroupMemoryBarrierWithGroupSync();
    //if (GI < 4)
    //    sharedPixel[GI] += sharedPixel[4 + GI];
    //GroupMemoryBarrierWithGroupSync();
    //if (GI < 2)
    //    sharedPixel[GI] += sharedPixel[2 + GI];
    //GroupMemoryBarrierWithGroupSync();
    //if (GI < 1)
    //    sharedPixel[GI] += sharedPixel[1 + GI];
    //GroupMemoryBarrierWithGroupSync();

    //float luminance = GetLuminance(sharedPixel[0].xyz);
    //luminance = log(luminance + 0.5f) / 64;
    //// 露光値
    //float exposure = (keyValue / luminance);

    // トーンマップ
    float4 color = inTexture[DTid.xy];
    color = Reinhard(color);
   
    // ガンマ補正
    //outTexture[DTid.xy] = GammaCorrection(color, 2.2f);
    outTexture[DTid.xy] = color;
}
