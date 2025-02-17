#include "Particle.hlsli"

struct PerticleForGPU
{
    float4x4 WVP;
    float4x4 world;
};

StructuredBuffer<PerticleForGPU> gParticle : register(t0);
	
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
    output.color = input.color;
    return output;
}