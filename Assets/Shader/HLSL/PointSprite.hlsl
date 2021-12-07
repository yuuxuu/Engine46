//*****************************************************************************
//!	@file   PointSprite.hlsl
//!	@brief
//!	@note
//!	@author 2020/10/31 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD0;
};

struct PS_OUT {
    float4 color    : SV_TARGET0;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.0f);

    output.color = input.color;

    return output;
}

typedef VS_OUT GS_OUT;

[maxvertexcount(4)]
void GS_main(point VS_OUT input[1], inout TriangleStream<GS_OUT> TriStream) {
    GS_OUT output = (GS_OUT)0;

    float size = 1.0f;

    float4x4 mat = matW;
    mat._41 = input[0].pos.x;
    mat._42 = input[0].pos.y;
    mat._43 = input[0].pos.z;

    {
        float4 pos = float4(-size, size, 0.0f, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0, 0);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(size, size, 0.0f, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1, 0);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(-size, -size, 0.0f, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0, 1);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(size, -size, 0.0f, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1, 1);

        TriStream.Append(output);
    }

    TriStream.RestartStrip();
}

typedef GS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    output.color = diffuseTex.Sample(sampleState, input.uv) * input.color * 2;// *material.diffuse* material.emissive;

    if (output.color.a <= 0.3f) discard;

    return output;
}
