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

static const float2 kIndex5x5[5][5] = {
    { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
    { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
    { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
    { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
    { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } }
};

static const float PI = 3.14159265f;

float gauss(float x, float y, float sigma) {
    
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    
    float denominator = 2.0f * PI * sigma * sigma;
    
    return exp(exponent) * rcp(denominator);
}

PixelShaderOutput main(VertexShaderOutput input) {
    
    PixelShaderOutput output;
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    // uvStepSizeの算出
    int width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(float(width)), rcp(float(height)));
    
    // kernelを求める。weightは後で行う
    float weight = 0.0f;
    float kernel3x3[3][3];
    float kernel5x5[5][5];
    
    // 3x3ループ
    //for (int x = 0; x < 3; ++x) {
    //    for (int y = 0; y < 3; ++y) {
    //        
    //        kernel3x3[x][y] = gauss(kIndex3x3[x][y].x, kIndex3x3[x][y].y, 2.0f); // 2.0fは標準偏差
    //        weight += kernel3x3[x][y];
    //        
    //        // 現在のtexcoordを算出
    //        float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
    //       
    //        // 色に1/9掛けて足す
    //        float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
    //        output.color.rgb += fetchColor * kernel3x3[x][y];
    //    }
    //}
    
    // 5x5ループ
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            
            kernel5x5[x][y] = gauss(kIndex5x5[x][y].x, kIndex5x5[x][y].y, 2.0f); // 2.0fは標準偏差
            weight += kernel5x5[x][y];
            
            // 現在のtexcoordを算出
            float2 texcoord = input.texcoord + kIndex5x5[x][y] * uvStepSize;
           
            // 色に1/9掛けて足す
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * kernel5x5[x][y];
        }
    }
    
    // 畳み込み後の値を正規化する。本来gauss関数は全体を合計すると(積分)1になるように設計されている。
    // しかし、無限の範囲は足せないので、kernel値の合計であるweightは1に満たない。
    // なので、合計が1になるように逆数を掛けて全体を底上げして調整する。
    output.color.rgb *= rcp(weight);
    
    return output;
}