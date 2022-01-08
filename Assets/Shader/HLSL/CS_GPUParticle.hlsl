//*****************************************************************************
//!	@file   CS_GPUParticle.hlsl
//!	@brief
//!	@note
//!	@author 2021/11/25 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct Particle {
    float3 pos;
    float4 color;
    float2 uv;
    float3 normal;

    float3 initPos;
    float3 velocity;
    float lifeTime;
    float gravity;
};

RWStructuredBuffer<Particle> particleSB : register(u0);

[RootSignature(RS_CS_GPUPARTICLE)]
[numthreads(1000, 1, 1)]
void CS_main(uint3 DTid : SV_DispatchThreadID)
{
    uint index =
        DTid.z * 1000 * 1 +
        DTid.y * 1000 +
        DTid.x;

    particleSB[index].pos += particleSB[index].velocity;
    
    particleSB[index].lifeTime -= 0.016f;

    if (particleSB[index].lifeTime <= 0.0f) {
        particleSB[index].pos = particleSB[index].initPos;
        //particleSB[index].pos = float3(matW._41, matW._42, matW._43);

        particleSB[index].lifeTime = GetRandomNumber(float2(1.0f / index, 1.0f / index)) * 10.0f;
    }
}
