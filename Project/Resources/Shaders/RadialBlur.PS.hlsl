#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

struct Config {
    
    float3 glowColor; // グローの色
    float blurStrength; // ぼかしの強さ
    float2 center; // 中心点
};

ConstantBuffer<Config> gConfig : register(b0);

static const int kNumSamples = 10; // サンプリング数。多いほど滑らかだが重い

PixelShaderOutput main(VertexShaderOutput input) {
    
    // 中心から現在のuvに対しての方向を計算
    // 普段方向といえば、単位ベクトルだが、ここではあえて正規化せず、遠いほどより遠くをサンプリングする
    float2 direction = input.texcoord - gConfig.center;
    
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    
    for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex) {
        
        // サンプリング位置を指数関数的に分布させる
        float factor = pow(float(sampleIndex) / (kNumSamples - 1), 2.0f); // 外側ほど速く見える
        
        // 現在のuvから先ほど計算した方向にサンプリング点を進めながらサンプリングしていく
        float2 texcoord = input.texcoord + direction * gConfig.blurStrength * factor;
        
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
    }
    
    // 平均化する
    outputColor.rgb *= rcp(float(kNumSamples));
    
    // 輝度を上げる
    float boostGlow = saturate(gConfig.blurStrength * 3.0f);
    
    outputColor.rgb += gConfig.glowColor * boostGlow;
    
    PixelShaderOutput output;
 
    output.color.rgb = outputColor;
    output.color.a = 1.0f; // アルファ値は1.0に設定
    
    return output;
}