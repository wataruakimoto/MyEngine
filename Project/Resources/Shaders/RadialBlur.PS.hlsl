#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

static const float2 kCenter = float2(0.5f, 0.5f); // 中心点。ここを基準に放射状にブラーがかかる
static const int kNumSamples = 10; // サンプリング数。多いほど滑らかだが重い
static const float kBlurWidth = 0.01f; // ぼかしの幅。大きいほど大きい

PixelShaderOutput main(VertexShaderOutput input) {
    
    // 中心から現在のuvに対しての方向を計算
    // 普段方向といえば、単位ベクトルだが、ここではあえて正規化せず、遠いほどより遠くをサンプリングする
    float2 direction = input.texcoord - kCenter;
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex) {
        
        // 現在のuvから先ほど計算した方向にサンプリング点を進めながらサンプリングしていく
        float2 texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    // 平均化する
    outputColor.rgb *= rcp(float(kNumSamples));
    
    PixelShaderOutput output;
 
    output.color.rgb = outputColor;
    output.color.a = 1.0f; // アルファ値は1.0に設定
    
    return output;
}