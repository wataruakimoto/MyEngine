#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

struct Config {
    
    float threshold; // マスクの閾値
};

ConstantBuffer<Config> gConfig : register(b0);

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    // maskの値が闘値以下の場合はdiscardして抜く
    if (mask <= gConfig.threshold) {
        discard;
    }
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    return output;
}