#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

struct Config {
    
    float threshold; // マスクの閾値
    float3 edgeColor; // エッジの色
    float detectionRange; // エッジの検出範囲
};

ConstantBuffer<Config> gConfig : register(b0);

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    // maskの値が闘値以下の場合はdiscardして抜く
    if (mask <= gConfig.threshold) {
        discard;
    }
    
    // 閾値に範囲を加算
    float upperThreshold = gConfig.threshold + gConfig.detectionRange;
    
    // Edgeっぽさを算出
    float edge = 1.0f - smoothstep(gConfig.threshold, upperThreshold, mask); // 0.0f ~ 1.0fの値を返す
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * gConfig.edgeColor;
    
    return output;
}