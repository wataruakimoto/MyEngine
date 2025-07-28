#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

struct Config {
    
    float time; // 時間
};

float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233)) {
    
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

ConstantBuffer<Config> gConfig : register(b0);

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    // 乱数生成
    float random = rand2dTo1d(input.texcoord * gConfig.time);
    
    output.color = gTexture.Sample(gSampler, input.texcoord) * random;
    
    return output;
}