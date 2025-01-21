#include "object3d.hlsli"

struct Material
{
    float4 color;
    int lightingMode;
    float4x4 uvTransform;
    float shininess;
};

struct DirectionalLight
{
    float4 color; // !< ライトの色
    float3 direction; // !< ライトの向き
    float intensity; // !< 輝度
};

struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float4 transformedUV = mul(float4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    PixelShaderOutput output;
    
    if (gMaterial.lightingMode == 1)
    { // Lambertian Reflection
        
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
        
    }
    else if (gMaterial.lightingMode == 2)
    { // Harf Lambert
        
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else if (gMaterial.lightingMode == 3)
    { // Phong Reflection Model
        
        // 拡散反射の計算
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // 鏡面反射の計算
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
        float RdotE = dot(toEye, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        // 拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        // 鏡面反射
        float3 specular = float3(1.0f, 1.0f, 1.0f) * gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        
        // 拡散反射と鏡面反射の合成
        output.color.rgb = diffuse + specular;
        // アルファ値はテクスチャをそのまま
        output.color.a = gMaterial.color.a * textureColor.a;
        
    }
    else
    { // Lightingしない場合
        
        output.color = gMaterial.color * textureColor;
    }
    
    return output;
}