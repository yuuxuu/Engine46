//*****************************************************************************
//!	@file   CS_ButterFly.hlsl
//!	@brief
//!	@note
//!	@author 2021/11/25 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct ButterFlyParticle {
    float3 pos;
    float4 color;
    float2 uv; // uv.x:degree uv.y:speed
    float3 normal;

    float3 initPos;
    float3 velocity;
    float lifeTime;
    float gravity;
};

RWStructuredBuffer<ButterFlyParticle> butterFlyParticleSB : register(u0);

[RootSignature(RS_CS_GPUPARTICLE)]
[numthreads(1000, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
    uint index =
        DTid.z * 1000 * 1 +
        DTid.y * 1000 +
        DTid.x;

    int degree = int(butterFlyParticleSB[index].uv.x + butterFlyParticleSB[index].uv.y);
    butterFlyParticleSB[index].uv.x = float(degree % 360);

    butterFlyParticleSB[index].lifeTime -= 0.016f;

    butterFlyParticleSB[index].pos += butterFlyParticleSB[index].velocity;

    if (butterFlyParticleSB[index].lifeTime <= 0.0f) {
        butterFlyParticleSB[index].pos = butterFlyParticleSB[index].initPos;

        if (index % 2 == 0) {
            butterFlyParticleSB[index].color = float4(1.0f, 1.0f, 0.0f, 1.0f);
        }
        else {
            butterFlyParticleSB[index].color = float4(0.4f, 1.0f, 1.0f, 1.0f);
        }

        butterFlyParticleSB[index].lifeTime = GetRandomNumber(float2(1.0f / index, 1.0f / index)) * 15.0f;
    }
}
