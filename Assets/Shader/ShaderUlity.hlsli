//*****************************************************************************
//!	@file	ShaderUlity.hlsli
//!	@brief	シェーダーヘッダー
//!	@note	
//!	@author	2020/04/11 作成：木村優
//*****************************************************************************

#ifndef _SHADER_ULITY_H_
#define _SHADER_ULITY_H_

#define PI 3.141592

float4x4 GetScaleMatrix(float3 scale) {
    float4x4 mat = {
        float4(scale.x,       0,       0,    0),
        float4(0, scale.y,       0,    0),
        float4(0,       0, scale.z,    0),
        float4(0,       0,       0, 1.0f),
    };
    return mat;
}

float4x4 GetTransMatrix(float3 pos) {
    float4x4 mat = {
        float4(1.0f,     0,     0,    0),
        float4(0,  1.0f,     0,    0),
        float4(0,     0,  1.0f,    0),
        float4(pos.x, pos.y, pos.z, 1.0f),
    };
    return mat;
}

float4x4 GetRotationMatrix(float4x4 rotationMat) {
    float4x4 mat = {
        float4(rotationMat._11, rotationMat._12, rotationMat._13,    0),
        float4(rotationMat._21, rotationMat._22, rotationMat._23,    0),
        float4(rotationMat._31, rotationMat._32, rotationMat._33,    0),
        float4(0,               0,               0, 1.0f),
    };
    return mat;
}

// ガンマ値を取得
float GetGumma(float3 color) {
    return dot(color, float3(0.299f, 0.587f, 0.144f));
}

// 高輝度の取得
float GetBrightness(float3 color) {
    return max(color.r, max(color.g, color.b));
}

// ランバート
float Lambert(float3 normal, float3 light) {
    return saturate(dot(normal, light));
}

// ハーフランバート
float HalfLambert(float3 normal, float3 light) {
    float d = Lambert(normal, light);
    d = d * 0.5f + 0.5f;
    return d * d;
}

// フォン
float Phong(float3 normal, float3 halfVec, float specularPower) {
    return pow(saturate(dot(normal, halfVec)), specularPower);
}

// 法線分布関数
float D_GGX(float ndoth, float a) {
    float a2 = a * a;
    float f = (ndoth * ndoth) * (a2 - 1.0f) + 1.0f;

    return a2 / (PI * f * f);
}

// 幾何減衰項
float G_CookTrrance(float ndotl, float ndotv, float ndoth, float vdoth) {
    float nh2 = 2.0f * ndoth;

    float g1 = (nh2 * ndotv) / vdoth;
    float g2 = (nh2 * ndotl) / vdoth;

    return min(1.0f, min(g1, g2));
}

// フレネル項
float F_Schlick(float f0, float f90) {
    return f0 + (1.0f - f0) * pow(1.0f - f90, 5);
}

// 拡散反射
float Fd_Burley(float ndotv, float ndotl, float ldoth, float roughness) {
    float f90 = 0.5f + 2.0f * ldoth * ldoth * roughness;
    float FL = 1.0f + (f90 - 1.0f) * pow(1.0f - ndotl, 5);//F_Schlick(f0, ndotl);
    float FV = 1.0f + (f90 - 1.0f) * pow(1.0f - ndotv, 5);//F_Schlick(f0, ndotv);

    return (FL * FV) / PI;
}

// 
float3 BRDF(float3 albedo, float metallic, float roughness, float3 light, float3 normal, float3 view) {
    float3 halfVec = normalize(light + view);

    float ndotl = saturate(dot(normal, light));
    float ndotv = saturate(dot(normal, view));
    float ndoth = saturate(dot(normal, halfVec));
    float vdoth = saturate(dot(view, halfVec));
    float ldoth = saturate(dot(light, halfVec));

    float3 diffuse = albedo + Fd_Burley(ndotv, ndotl, ldoth, roughness);

    float D = D_GGX(ndoth, roughness);
    float F = F_Schlick(0.16f, vdoth);
    float G = G_CookTrrance(ndotl, ndotv, ndoth, vdoth);

    float3 specular = (D * F * G) / (4.0f * ndotv * ndotl + 0.000001f);

    return diffuse + specular;
}

#endif