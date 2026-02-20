#include "Line.hlsli"

struct TransformationMatrix {
    float4x4 viewProjection;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderInput {
    float4 position : POSITION0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input) {
    
    VertexShaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.viewProjection);
    output.color = input.color;
    return output;
}