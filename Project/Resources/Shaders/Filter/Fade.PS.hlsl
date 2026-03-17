#include "FullScreen.hlsli"

// フェードの設定
struct Config {
    
    float4 color; // フェードの色
};

ConstantBuffer<Config> gConfig : register(b0);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    // 元の画像の色を取得
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 元の色とフェードの色を合成
    output.color.rgb = lerp(textureColor.rgb, gConfig.color.rgb, gConfig.color.a);
    output.color.a = textureColor.a; // アルファは元の画像のまま
    
    return output;
}