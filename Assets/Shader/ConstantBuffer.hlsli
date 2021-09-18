//*****************************************************************************
//!	@file	constantBuffer.hlsli
//!	@brief	�V�F�[�_�[�C���N���[�h�w�b�_�[
//!	@note	
//!	@author	2020/03/13 �쐬�F�ؑ��D
//*****************************************************************************

#ifndef _CONSTANT_BUFFER_H_
#define _CONSTANT_BUFFER_H_

#define POSITION_MAX	25000

#define OFFSET_MAX		15

#define TRI_POINTS		3
#define QUAD_POINTS		4

#define LIGHT_MAX		1024 / 2

#define INSTANCE_MAX	1024

// �e�N�X�`��
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

// �T���v���[
SamplerState sampleState					: register(s0);
SamplerComparisonState shadowSamplerState	: register(s1);
SamplerState sampleState1					: register(s2);
// �A���I�[�_�[�h�A�N�Z�X�r���[(�o��)
RWByteAddressBuffer outBuffer				: register(u0);

cbuffer CbWorld : register(b0)
{
	float4x4	matW;	// ���[���h�s��
}

struct Material {
	float4	diffuse;	// �}�e���A���f�B�t���[�Y�F
	float4	specular;	// �}�e���A���X�y�L�����[�F
	float4	ambinet;	// �}�e���A���A���r�G���g�F
};	float4	emissive;	// �}�e���A���G�~�b�V�u�F

// �}�e���A��
cbuffer CbMaterial : register(b1)
{
	Material material;
}

cbuffer CbCamera : register(b2)
{
	float4x4	matVP;		// ���[���h�r���[�v���W�F�N�V�����s��

	float3		cameraPos;	// �J�����̈ʒu
}

struct DirectionalLight {
	float4 pos;
	float4 diffuse;
	float4 specular;
};

// �f�B���N�V���i�����C�g
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

//// �s��R���X�^���g�o�b�t�@
//cbuffer CbMatrix : register(b4)
//{
//	float4x4	insWorldMatrix[INSTANCE_MAX];	// �C���X�^���X���[���h�ϊ��s��
//}
//
//cbuffer cbDynamicCube : register(b5)
//{
//	float4x4	views[6];				// ���I�L���[�u�}�b�v�r���[�ϊ��s��
//}
//
//// PBR�p�����[�^�[�R���X�^���g�o�b�t�@
//cbuffer CbPBRParam : register(b6)
//{
//	float4		PBRParam;				// PBR�p�����[�^�[(x:metallic, y:roughness, z:reflectance)
//}
//
//// �u���[�R���X�^���g�o�b�t�@
//cbuffer CbBlur : register(b7)
//{
//	float4		offset[OFFSET_MAX];
//}
//
//// Tess�p�����[�^�[�R���X�^���g�o�b�t�@
//cbuffer CbTessParam : register(b8)
//{
//	float4		tessParam;
//}
//
//// �g�p�����[�^�[�R���X�^���g�o�b�t�@
//cbuffer CbWaveParam : register(b9)
//{
//	float4		wavePos;		// �g�̌��_�ʒu
//	float4		waveParam;		// �g�p�����[�^�[(x:�U��, y:�g��, z:�p�x, w:�g�̃G�b�W)
//}
//
//// �p�[�e�B�N���p�����[�^�[�R���X�^���g�o�b�t�@
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