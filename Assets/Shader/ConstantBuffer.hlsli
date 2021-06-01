//*****************************************************************************
//!	@file	constantBuffer.hlsli
//!	@brief	�V�F�[�_�[�C���N���[�h�w�b�_�[
//!	@note	
//!	@author	2020/03/13 �쐬�F�ؑ��D
//*****************************************************************************

#ifndef _CONSTANTBUFFER_H_
#define _CONSTANFBUFFER_H_

#define PI				3.141592
#define POSITION_MAX	25000

#define OFFSET_MAX		15

#define TRI_POINTS		3
#define QUAD_POINTS		4

#define LIGHT_MAX		1024

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

// ���C��
cbuffer CbMain : register(b0)
{
	float4x4	matWVP;			// ���[���h�r���[�v���W�F�N�V�����s��

	float4x4	matLightWVP;	// ���C�g���[���h�r���[�v���W�F�N�V�����s��

	float4		cameraPos;		// �J�����̈ʒu
}

// �}�e���A��
cbuffer CbMaterial : register(b1)
{
	float4		diffuse;					// �}�e���A���f�B�t���[�Y�F
	float4		specular;					// �}�e���A���X�y�L�����[�F
	float4		ambinet;					// �}�e���A���A���r�G���g�F
	float4		emissive;					// �}�e���A���G�~�b�V�u�F
	float4		brightness;					// �P�x�̐F
}

// ���C�g�R���X�^���g�o�b�t�@
cbuffer CbLight : register(b2)
{
	float4		lightPos[LIGHT_MAX];		// �X�|�b�g���C�g�̈ʒu
	float4		lightDiffuse[LIGHT_MAX];	// �X�|�b�g���C�g�f�B�t���[�Y�F
	float4		lightSpecular[LIGHT_MAX];	// �X�|�b�g���C�g�X�y�L�����[�F
	
	float4		attenuation;				// �X�|�b�g���C�g�̌����p�����[�^�[

	int			lightNum;
}

// �s��R���X�^���g�o�b�t�@
cbuffer CbMatrix : register(b3)
{
	float4x4	insWorldMatrix[INSTANCE_MAX];	// �C���X�^���X���[���h�ϊ��s��
}

cbuffer cbDynamicCube : register(b4)
{
	float4x4	views[6];				// ���I�L���[�u�}�b�v�r���[�ϊ��s��
}

// PBR�p�����[�^�[�R���X�^���g�o�b�t�@
cbuffer CbPBRParam : register(b5)
{
	float4		PBRParam;				// PBR�p�����[�^�[(x:metallic, y:roughness, z:reflectance)
}

// �u���[�R���X�^���g�o�b�t�@
cbuffer CbBlur : register(b6)
{
	float4		offset[OFFSET_MAX];
}

// Tess�p�����[�^�[�R���X�^���g�o�b�t�@
cbuffer CbTessParam : register(b7)
{
	float4		tessParam;
}

// �g�p�����[�^�[�R���X�^���g�o�b�t�@
cbuffer CbWaveParam : register(b8)
{
	float4		wavePos;		// �g�̌��_�ʒu
	float4		waveParam;		// �g�p�����[�^�[(x:�U��, y:�g��, z:�p�x, w:�g�̃G�b�W)
}

// �p�[�e�B�N���p�����[�^�[�R���X�^���g�o�b�t�@
cbuffer CbParticleParam : register(b9)
{
	float		particleSize;
	float		particleLifeTime;
	float		particleVelocity;
	float		dummy;

	float4		particleStartColor;
	float4		particleLerpColor;
}

cbuffer CbToneMapParam : register(b10)
{
	float		keyValue;
	float		avgBrightness;
	float		targetAvgBrightness;

	float		alpha;
}

#endif