//*****************************************************************************
//!	@file	CS_Blur.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/31 作成：木村優
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"

// 書き読み込みテクスチャ
RWTexture2D<float4> inOutTexture  : register(u0);

[RootSignature(RS_CS_CLEAR_COLOR)]
[numthreads(8, 8, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID) {
    inOutTexture[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}
