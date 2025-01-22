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
    float4 color; // 色
    float3 direction; // 向き
    float intensity; // 輝度
};

struct PointLight
{
    float4 color; // 色
    float3 position; // 位置
    float intensity; // 輝度
    float radius; // 光の届く最大距離
    float decay; // 減衰率
};

struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

ConstantBuffer<PointLight> gPointLight : register(b2);

ConstantBuffer<Camera> gCamera : register(b3);

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
    else if (gMaterial.lightingMode == 4)
    { // Blinn-Phong Reflection Model
        
        // 拡散反射の計算
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // 鏡面反射の計算
        float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        // 拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        // 鏡面反射
        float3 specular = float3(1.0f, 1.0f, 1.0f) * gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow;
        
        // 拡散反射と鏡面反射の合成
        output.color.rgb = diffuse + specular;
        // アルファ値はテクスチャをそのまま
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else if (gMaterial.lightingMode == 5)
    { // PointLight
        
        float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        
        // 拡散反射の計算
        float NdotL = dot(normalize(input.normal), -pointLightDirection);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // 鏡面反射の計算
        float3 toEye = normalize(gCamera.worldPosition - pointLightDirection);
        float3 halfVector = normalize(-pointLightDirection + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        float distance = length(gPointLight.position - input.worldPosition); // ポイントライトへの距離
        float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay); // 指数によるコントロール
        
        // 拡散反射
        float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        // 鏡面反射
        float3 specular = float3(1.0f, 1.0f, 1.0f) * gPointLight.color.rgb * gPointLight.intensity * specularPow * factor;
        
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