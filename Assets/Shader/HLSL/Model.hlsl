//*****************************************************************************
//!	@file	Model.hlsl
//!	@brief	
//!	@note	
//!	@author	2020/09/16 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN {
	float3 pos		: POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD;
	float3 normal	: NORMAL;
};

struct VS_OUT {
	float4 pos		: SV_POSITION;
	float4 color	: COLOR;
	float2 uv		: TEXCOORD;
	float3 normal	: NORMAL;
	float4 posw		: POSITION0;
};

struct PS_OUT {
	float4 color	: SV_TARGET;
};

[RootSignature(RS_01)]
VS_OUT VS_main(VS_IN input) {
	VS_OUT output = (VS_OUT)0;

	output.posw = mul(float4(input.pos, 1.0f), matW);
	output.pos = mul(output.posw, matVP);

	output.color = input.color;
	output.uv = input.uv;

	output.normal = mul(input.normal, (float3x3)matW);

	return output;
}

typedef VS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
	PS_OUT output = (PS_OUT)0;

	output.color = diffuseTex.Sample(sampleState, input.uv);

	float4 lightColor;
	for (int i = 0; i < lightNum; ++i) {
		float3 l = lightPos[i].xyz - input.posw.xyz;
		float3 n = normalize(input.normal);
		float len = length(l);

		l = normalize(l);
		// åıåπÇ∆ñ@ê¸ÇÃì‡êœÇåvéZ
		float d = Lambert(n, l);
		// å∏êä
		float4 att = saturate(1.0f / (attenuation[i].x + attenuation[i].y * len + attenuation[i].z * len * len));

		lightColor += (lightDiffuse[i] * d) * att;
	}

	output.color *= lightColor;

	return output;
}