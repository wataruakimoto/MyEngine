#include "Skybox.hlsli"

struct PixelShaderOutput {
    
    float4 color : SV_TARGET0;
};

struct Material {
    
    float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);

TextureCube<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    output.color = gMaterial.color * textureColor;
    
    return output;
}