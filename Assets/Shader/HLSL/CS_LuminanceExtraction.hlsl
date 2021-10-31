//*****************************************************************************
//!	@file	CS_LuminanceExtraction.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/10/31 �쐬�F�ؑ��D
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

// �����ǂݍ��݃e�N�X�`��
RWTexture2D<float4> inTexture  : register(u0);
RWTexture2D<float4> outTexture : register(u1);

// 臒l(�������l)
static const float threshold = 1.3f;

[RootSignature(RS_CS_LuminanceExtraction)]
[numthreads(8, 8, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID) {
    float4 color = inTexture[DTid.xy];

    outTexture[DTid.xy] = color;

    float gumma = GetGumma(float3(color.rgb));
    // 臒l�ȏ�̐F���o��
    if (gumma < threshold)
    {
        outTexture[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}
