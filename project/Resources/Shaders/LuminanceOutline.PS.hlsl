#include "FullScreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

static const float2 kIndex3x3[3][3] = {
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

static const float kPrewittHorizontalKernel[3][3] = {
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};

static const float kPrewittVerticalKernel[3][3] = {
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }
};

float Luminance(float3 v) {
    
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
     // uvStepSizeの算出
    int width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(float(width)), rcp(float(height)));
    
    // 縦横それぞれの畳み込みの結果を格納する
    float2 difference = float2(0.0f, 0.0f);
    
    //3x3ループ
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            
            // 現在のtexcoordを算出
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            
            // 色に1/9掛けて足す
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            
            // 色を輝度に変換して、畳み込みを行う
            float luminance = Luminance(fetchColor);
            
            difference.x += luminance * kPrewittHorizontalKernel[x][y];
            difference.y += luminance * kPrewittVerticalKernel[x][y];
        }
    }
    
    // 変化の長さを重みとして合成。重みの決定方法も色々考えられる。例えばdifference.xだけ使えば横方向のエッジが検出される
    float weight = length(difference);
    
    // 差が小さすぎてわかりづらいので適当に６倍
    weight = saturate(weight * 6.0f); // 0~1に収める
    
    // weightが大きいほど暗く表示するようにしている。最もシンプルな合成方法
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    
    return output;
}