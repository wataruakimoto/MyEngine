#include "Skybox.hlsli"

struct VertexShaderInput {
    
    float4 position : POSITION0;
};

struct Transformation {
    
    float4x4 WVP;
};

ConstantBuffer<Transformation> gTransformation : register(b0);

VertexShaderOutput main(VertexShaderInput input) {
    
    VertexShaderOutput output;
    
    output.position = mul(input.position, gTransformation.WVP).xyww;
    
    output.texcoord = input.position.xyz;
    
    return output;
}