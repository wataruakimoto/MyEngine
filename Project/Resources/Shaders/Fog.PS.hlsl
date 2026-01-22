#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gDepthTexture : register(t1);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

// フォグの設定
struct Config {
    float4 color; // フォグの色
    float start;  // フォグの開始距離
    float end;    // フォグの終了距離
};

ConstantBuffer<Config> gConfig : register(b0);

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    
    // テクスチャから色を取得
    float4 baseColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 深度値を取得 (0.0f = 手前, 1.0f = 奥)
    float depth = gDepthTexture.Sample(gSampler, input.texcoord);
    
    // 深度によるフォグのかかり具合
    float depthFactor = smoothstep(gConfig.start, gConfig.end, depth);
    
    // アルファ値を考慮したフォグの強さ
    float fogFactor = depthFactor * gConfig.color.a;
    
    // 色を混ぜる
    output.color.rgb = lerp(baseColor.rgb, gConfig.color.rgb, fogFactor);
    output.color.a = baseColor.a; // アルファ値は元のまま
    
    return output;
}