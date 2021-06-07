struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSInput
{
    float4 positionSV : SV_POSITION;
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer VSSystemUniforms : register(b0)
{
    row_major matrix sys_ProjectionMatrix;
    row_major matrix sys_ViewMatrix;
    row_major matrix sys_ModelMatrix;
};

PSInput VSMain(in VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), sys_ModelMatrix).xyz;
    output.positionSV = mul(float4(output.position, 1.0f), mul(sys_ViewMatrix, sys_ProjectionMatrix));
    output.uv = input.uv;
    output.normal = input.normal;
    return output;
}

cbuffer PSUserUniforms : register(b0)
{
    float3 u_Color;
    float3 u_Color2;
}
float4 PSMain(in PSInput input) : SV_Target
{
    return float4(u_Color, 1.0f);
}