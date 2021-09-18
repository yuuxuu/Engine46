//*****************************************************************************
//!	@file	constantBuffer.hlsli
//!	@brief	シェーダーインクルードヘッダー
//!	@note	
//!	@author	2020/03/13 作成：木村優
//*****************************************************************************

#ifndef _CONSTANT_BUFFER_H_
#define _CONSTANT_BUFFER_H_

#define POSITION_MAX	25000

#define OFFSET_MAX		15

#define TRI_POINTS		3
#define QUAD_POINTS		4

#define LIGHT_MAX		1024 / 2

#define INSTANCE_MAX	1024

// テクスチャ
Texture2D			diffuseTex		: register(t0);
Texture2D			specularTex		: register(t1);
Texture2D			normalTex		: register(t2);
Texture2D			posTex			: register(t3);
Texture2D			lightMapTex		: register(t4);
Texture2D<float>	depthTex		: register(t5);
Texture2D<uint2>	stencilTex		: register(t6);
TextureCube			cubeTex			: register(t7);
TextureCube			dynamicCubeTex	: register(t8);
Texture2D			albedoTex		: register(t9);
Texture2D			displacementTex	: register(t10);
Texture2D			roughnessTex	: register(t11);
Texture2D			hdrTex			: register(t12);
Texture2D			toneMapTex		: register(t13);
Texture2D			BlurTex0		: register(t14);
Texture2D			BlurTex1		: register(t15);
Texture2D			BlurTex2		: register(t16);
Texture2D			BlurTex3		: register(t17);

// サンプラー
SamplerState sampleState					: register(s0);
SamplerComparisonState shadowSamplerState	: register(s1);
SamplerState sampleState1					: register(s2);
// アンオーダードアクセスビュー(出力)
RWByteAddressBuffer outBuffer				: register(u0);

cbuffer CbWorld : register(b0)
{
	float4x4	matW;	// ワールド行列
}

struct Material {
	float4	diffuse;	// マテリアルディフューズ色
	float4	specular;	// マテリアルスペキュラー色
	float4	ambinet;	// マテリアルアンビエント色
};	float4	emissive;	// マテリアルエミッシブ色

// マテリアル
cbuffer CbMaterial : register(b1)
{
	Material material;
}

cbuffer CbCamera : register(b2)
{
	float4x4	matVP;		// ワールドビュープロジェクション行列

	float3		cameraPos;	// カメラの位置
}

struct DirectionalLight {
	float4 pos;
	float4 diffuse;
	float4 specular;
};

// ディレクショナルライト
cbuffer CbDirectionalLight : register(b3)
{
	DirectionalLight directionalLight;
}

struct PointLight {
	float3	pos;
	float	radius;
	float4	diffuse;
	float4	specular;
	float4	attenuation;
};

cbuffer CbPointLight : register(b4)
{
	PointLight pointLights[LIGHT_MAX];

	int numPointLight;
}

struct SpotLight {
	float3	pos;
	float	angle;
	float4	diffuse;
	float4	specular;
	float4	attenuation;
};

cbuffer CbSpotLight : register(b5)
{
	SpotLight spotLights[LIGHT_MAX];

	int numSpotLight;
}

//// 行列コンスタントバッファ
//cbuffer CbMatrix : register(b4)
//{
//	float4x4	insWorldMatrix[INSTANCE_MAX];	// インスタンスワールド変換行列
//}
//
//cbuffer cbDynamicCube : register(b5)
//{
//	float4x4	views[6];				// 動的キューブマップビュー変換行列
//}
//
//// PBRパラメーターコンスタントバッファ
//cbuffer CbPBRParam : register(b6)
//{
//	float4		PBRParam;				// PBRパラメーター(x:metallic, y:roughness, z:reflectance)
//}
//
//// ブラーコンスタントバッファ
//cbuffer CbBlur : register(b7)
//{
//	float4		offset[OFFSET_MAX];
//}
//
//// Tessパラメーターコンスタントバッファ
//cbuffer CbTessParam : register(b8)
//{
//	float4		tessParam;
//}
//
//// 波パラメーターコンスタントバッファ
//cbuffer CbWaveParam : register(b9)
//{
//	float4		wavePos;		// 波の原点位置
//	float4		waveParam;		// 波パラメーター(x:振幅, y:波長, z:角度, w:波のエッジ)
//}
//
//// パーティクルパラメーターコンスタントバッファ
//cbuffer CbParticleParam : register(b10)
//{
//	float		particleSize;
//	float		particleLifeTime;
//	float		particleVelocity;
//	float		dummy;
//
//	float4		particleStartColor;
//	float4		particleLerpColor;
//}
//
//cbuffer CbToneMapParam : register(b11)
//{
//	float		keyValue;
//	float		avgBrightness;
//	float		targetAvgBrightness;
//
//	float		alpha;
//}

#endif