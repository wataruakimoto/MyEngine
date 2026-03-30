#include "FullScreen.hlsli"

struct Config {
    
    float4 color; // 色 RGB：色 A：強度
    float intensity; // 強さ
    float scale; // スケール
    float range; // 範囲
};

ConstantBuffer<Config> gConfig : register(b0);

Texture2D<float4> gTexture : register(t0);

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    PixelShaderOutput output;
    
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    // 中心からの距離を計算 (-1.0 ~ 1.0 の範囲に正規化)
    float2 centered = input.texcoord * 2.0f - 1.0f;
    
    // rangeで距離をスケーリング (range が大きいほど広範囲に効果)
    centered *= gConfig.range;
    
    // UV座標に変換し直す
    float2 scaledUV = saturate((centered + 1.0f) * 0.5f);
    
    // ビネット計算
    float2 correct = scaledUV * (1.0f - scaledUV.yx);
    float vignette = correct.x * correct.y * gConfig.scale;
    
    // Intensityでビネット効果の強さを調整
    vignette = saturate(pow(vignette, gConfig.intensity));
    
    // 中心1.0→周辺0.0 を、周辺強度 0.0→1.0 に反転
    float edgeFactor = 1.0f - vignette;
    
    float blend = saturate(edgeFactor * gConfig.color.a);
    
    // 係数として乗算 + 色を混ぜる
    // vignetteが1.0(中心)なら元の色、0.0(周辺)ならビネット色
    output.color.rgb = lerp(output.color.rgb, gConfig.color.rgb, blend);
    
    return output;
}