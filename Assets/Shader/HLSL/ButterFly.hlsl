//*****************************************************************************
//!	@file   ButterFly.hlsl
//!	@brief
//!	@note
//!	@author 2020/10/31 çÏê¨ÅFñÿë∫óD
//*****************************************************************************

#include "../ConstantBuffer.hlsli"
#include "../RootSignature.hlsli"
#include "../ShaderUlity.hlsli"

struct VS_IN {
    float3 pos      : POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD; // uv.x:degree uv.y:speed
    float3 normal   : NORMAL;   // normal:velocity; 
};

struct VS_OUT {
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
    float2 uv       : TEXCOORD0;
    float3 normal   : NORMAL;
};

struct PS_OUT {
    float4 color    : SV_TARGET0;
};

[RootSignature(RS_MODEL)]
VS_OUT VS_main(VS_IN input) {
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos, 1.0f);

    output.color = input.color;

    output.uv = input.uv;

    output.normal = input.normal;

    return output;
}

typedef VS_OUT GS_OUT;

[maxvertexcount(8)]
void GS_main(point VS_OUT input[1], inout TriangleStream<GS_OUT> TriStream) {
    GS_OUT output = (GS_OUT)0;

    float2 size = float2(1.0f, 1.0f);
    float radian = DegreeToRadian(input[0].uv.x);
    float posY = sin(radian);

    float4x4 mat = GetLookAtMatrix(float3(0.0f, 0.0f, 0.0f), input[0].normal, float3(0.0f, 1.0f, 0.0f));
    mat._41 = input[0].pos.x;
    mat._42 = input[0].pos.y;
    mat._43 = input[0].pos.z;

    {
        float4 pos = float4(-size.x, posY, size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0.0f, 0.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(0.0f, 0.0f, size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1.0f, 0.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(-size.x, posY, -size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0.0f, 1.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(0.0f, 0.0f, -size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1.0f, 1.0f);

        TriStream.Append(output);
    }
    TriStream.RestartStrip();

    {
        float4 pos = float4(0.0f, 0.0f, size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1.0f, 0.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(size.x, posY, size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0.0f, 0.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(0.0f, 0.0f, -size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(1.0f, 1.0f);

        TriStream.Append(output);
    }

    {
        float4 pos = float4(size.x, posY, -size.y, 1.0f);
        output.pos = mul(pos, mat);
        output.pos = mul(output.pos, matVP);

        output.color = input[0].color;

        output.uv = float2(0.0f, 1.0f);

        TriStream.Append(output);
    }
    TriStream.RestartStrip();
}

typedef GS_OUT PS_IN;

PS_OUT PS_main(PS_IN input) {
    PS_OUT output = (PS_OUT)0;

    output.color = diffuseTex.Sample(sampleState, input.uv) * input.color;

    if (output.color.a <= 0.3f) discard;

    return output;
}
