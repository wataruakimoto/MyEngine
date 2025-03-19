#include "Particle.hlsli"

struct Particle
{
    float4x4 WVP;
    float4x4 world;
    float4 color;
};

StructuredBuffer<Particle> gParticle : register(t0);
	
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gParticle[instanceId].WVP);
    output.texcoord = input.texcoord;
    output.color = gParticle[instanceId].color;
    return output;
}