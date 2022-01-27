//*****************************************************************************
//!	@file   CS_LuminanceExtraction.hlsl
//!	@brief
//!	@note
//!	@author 2022/01/15 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

static const int TILE_SIZE_X = 16;
static const int TILE_SIZE_Y = 16;
static const int TILE_SIZE = TILE_SIZE_X * TILE_SIZE_Y;

RWStructuredBuffer<uint> lightIndices : register(u0);

// ã§óLÉÅÉÇÉä
groupshared uint tileMinZ;
groupshared uint tileMaxZ;

groupshared uint tileLightIndices[LIGHT_MAX];
groupshared uint tileNumLights;

[RootSignature(RS_CS_LIGHT_CULLING)]
[numthreads(TILE_SIZE_X, TILE_SIZE_Y, 1)]
void CS_main(
    uint3 DTid : SV_DispatchThreadID,
    uint3 Gid  : SV_GroupID,
    uint3 GTid : SV_GroupThreadID) {

    int groupIndex = GTid.y * TILE_SIZE_Y + GTid.x;

    uint DTindex =
        DTid.z * TILE_SIZE_Y * 1 +
        DTid.y * TILE_SIZE_X +
        DTid.x;

    if (groupIndex == 0) {
        tileNumLights = 0;
        tileMinZ = 0x7F7FFFFF;
        tileMaxZ = 0;
    }

    float2 uv = ((float2)DTid.xy + 0.5f) * rcp(float2(textureWidth, textureHeight));
    uv = uv * float2(2.0f, -2.0f) - float2(1.0f, -1.0f);

    float3 screenPos;
    screenPos.xy = uv.xy;
    screenPos.z = depthTex.Load(uint3(DTid.xy, 0.0f));

    float4 viewPos = mul(invMatProj, float4(screenPos, 1.0f));
    viewPos.xyz /= viewPos.w;

    // ìØä˙
    GroupMemoryBarrierWithGroupSync();

    InterlockedMin(tileMinZ, asuint(viewPos.z));
    InterlockedMax(tileMaxZ, asuint(viewPos.z));

    // ìØä˙
    GroupMemoryBarrierWithGroupSync();

    float minZ = asfloat(tileMinZ);
    float maxZ = asfloat(tileMaxZ);

    float2 tileScale = float2(textureWidth, textureHeight) * rcp(float(TILE_SIZE_X * 2));
    float2 tileBias = tileScale - float2(Gid.xy);

    float4 c1 = float4(matProj._11 * tileScale.x, 0.0f, tileBias.x, 0.0f);
    float4 c2 = float4(0.0f, -matProj._22 * tileScale.y, tileBias.y, 0.0f);
    float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

    float4 planes[6];
    planes[0] = c4 - c1;
    planes[1] = c4 + c1;
    planes[2] = c4 - c2;
    planes[3] = c4 + c2;
    planes[4] = float4(0.0f, 0.0f,  1.0f, -minZ);
    planes[5] = float4(0.0f, 0.0f, -1.0f,  maxZ);

    [unroll]
    for (int i = 0; i < 4; ++i) {
        planes[i] *= rcp(length(planes[i].xyz));
    }

    for (int i = groupIndex; i < numPointLight; i += TILE_SIZE) {

        PointLight pLight = pointLights[i];

        float4 lightViewPos = mul(float4(pLight.pos, 1.0f), matView);

        bool inFrustum = true;
        for (int j = 0; j < 6; ++j) {
            float d = dot(planes[j], lightViewPos);
            inFrustum = inFrustum && (d >= -pLight.radius);
        }
        if (inFrustum) {
            int index;
            InterlockedAdd(tileNumLights, 1, index);
            tileLightIndices[index] = i;
        }
    }

    // ìØä˙
    GroupMemoryBarrierWithGroupSync();

    int cellX = (textureWidth + TILE_SIZE_X - 1) / TILE_SIZE_X;
    int index = floor(DTid.x / TILE_SIZE_X) + floor(DTid.y / TILE_SIZE_Y) * cellX;
    int start = numPointLight * index;
    for (int i = groupIndex; i < tileNumLights; i += TILE_SIZE) {
        lightIndices[start + i] = tileLightIndices[i];
    }

    if ((groupIndex == 0) && (tileNumLights < numPointLight)) {
        lightIndices[start + tileNumLights] = 0xffffffff;
    }
}
