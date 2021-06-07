struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct PSInput
{
    float4 positionSV : SV_POSITION;
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float visibility : VISIBILITY;
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
    output.color = input.color;
    
    // Calculate fog visibility
    float4 positionRelativeToCamera = mul(mul(float4(input.position, 1.0f), sys_ModelMatrix), sys_ViewMatrix);
    float distance = length(positionRelativeToCamera.xyz);
    output.visibility = exp(-pow(distance * 0.007, 5.5));
    
    return output;
}

Texture2D u_Texture : register(t0);
SamplerState samplerState : register(s0);

float4 PSMain(in PSInput input) : SV_Target
{
    float4 color = u_Texture.Sample(samplerState, input.uv) * input.color.a;
    float4 fogColor = float4(0.4, 0.7, 1.0, 1.0);
    return lerp(fogColor, color, input.visibility);
}