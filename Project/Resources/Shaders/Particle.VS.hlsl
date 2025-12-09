#include "Particle.hlsli"

struct Instance {
    float4x4 world;
    float4x4 WVP;
    float4 color;
};

StructuredBuffer<Instance> gInstance : register(t0);
	
struct VertexShaderInput {
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input, uint instanceId : SV_InstanceID) {
    VertexShaderOutput output;
    output.position = mul(input.position, gInstance[instanceId].WVP);
    output.texcoord = input.texcoord;
    output.color = gInstance[instanceId].color;
    return output;
}