// Vertex shader
struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOutput VSMain(uint id : SV_VertexID)
{
    VSOutput output;
    output.uv = float2((id << 1) & 2, id & 2);
    output.pos = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
    return output;
}

// Pixel (fragment) shader
float4 PSMain(VSOutput input) : SV_TARGET
{
    // Create a linear gradient based on the x-coordinate
    float3 color = lerp(float3(1.0, 0.0, 0.0), float3(0.0, 0.0, 1.0), input.uv.x);
    return float4(color, 1.0);
}
