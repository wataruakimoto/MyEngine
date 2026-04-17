#include "object3d.hlsli"

#define MAX_DIR_LIGHTS 4

struct Camera {
    float3 worldPosition;
};

struct Material {
    float4 color; // 色
    float3 specularColor; // 鏡面反射色
    float padding;
    uint diffuseSetting; // 拡散反射の設定 0:使わない 1:Lambert反射 2:HalfLambert反射 
    uint specularSetting; // 鏡面反射の設定 0:使わない 1:Phong反射 2:BlinnPhong反射
    uint useEnvironmentMap; // 環境マップの設定 0:使わない 1:使う
    float shininess; // 明るさ
    float4x4 uvTransform; // UV座標
};

struct DirectionalLight {
    float3 direction; // 向き
    float intensity; // 輝度
    float4 color; // 色
};

struct DirectionalLightList {
    uint activeCount; // 有効なライトの数
    float3 padding;
    DirectionalLight lights[MAX_DIR_LIGHTS];
};

struct PointLight {
    float3 position; // 位置
    float intensity; // 輝度
    float radius; // 光の届く半径
    float decay; // 減衰率
    float4 color; // 色
};

struct SpotLight {
    float3 position; // 位置
    float3 direction; // 向き
    float intensity; // 輝度
    float radius; // 光の届く半径
    float decay; // 減衰率
    float cosAngle; // 余弦
    float cosFalloffStart; // Falloffの開始角度
    float4 color; // 色
};

struct Environment {
    float intensity; // 環境光の強度
};

ConstantBuffer<Camera> gCamera : register(b1);

ConstantBuffer<Material> gMaterial : register(b2);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b3);

//ConstantBuffer<DirectionalLightList> gDirectionalLightList : register(b3);

ConstantBuffer<PointLight> gPointLight : register(b4);

ConstantBuffer<SpotLight> gSpotLight : register(b5);

ConstantBuffer<Environment> gEnvironment : register(b6);

Texture2D<float4> gTexture : register(t0);

TextureCube<float4> gEnvironmentTexture : register(t1);

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    /// ================================================== ///
	/// 平行光源の計算
	/// ================================================== ///
    
    /// ========== 拡散反射の計算 ========== ///
    
    float3 diffuseDirectional = float3(0.0f, 0.0f, 0.0f);
    
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    
    // Lambert反射なら
    if (gMaterial.diffuseSetting == 1) {
        
        float cos = saturate(NdotL);
        
        diffuseDirectional = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }
    // HalfLambert反射なら
    else if (gMaterial.diffuseSetting == 2) {
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        diffuseDirectional = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    }
    
    /// ========== 鏡面反射の計算 ========== ///
    
    float3 specularDirectional = float3(0.0f, 0.0f, 0.0f);
    
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    
    // Phong反射なら
    if (gMaterial.specularSetting == 1) {
        
        float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
        float RdotE = dot(toEye, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        specularDirectional = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
    }
    // BlinnPhong反射なら
    else if (gMaterial.specularSetting == 2) {
        
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        specularDirectional = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
    }
    
    /// ================================================== ///
	/// 点光源の計算
	/// ================================================== ///
    
     /// ========== 拡散反射の計算 ========== ///
    
    float3 diffusePoint = float3(0.0f, 0.0f, 0.0f);
   
    float3 directionToLight = input.worldPosition - gPointLight.position;
    
    float distanceToLight = length(directionToLight);
    
    float NdotLPoint = dot(normalize(input.normal), -normalize(directionToLight));
    
    float factor = pow(saturate(-distanceToLight / gPointLight.radius + 1.0f), gPointLight.decay);
    
    // Lambert反射なら
    if (gMaterial.diffuseSetting == 1) {
        
        float cos = saturate(NdotLPoint);
        
        diffusePoint = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
    }
    // HalfLambert反射なら
    else if (gMaterial.diffuseSetting == 2) {
        
        float cos = pow(NdotLPoint * 0.5f + 0.5f, 2.0f);
        diffusePoint = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
    }
    
    /// ========== 鏡面反射の計算 ========== ///
    
    float3 specularPoint = float3(0.0f, 0.0f, 0.0f);
    
    float3 toEyePoint = normalize(gCamera.worldPosition - input.worldPosition);
    
    // Phong反射なら
    if (gMaterial.specularSetting == 1) {
        
        float3 reflectLight = reflect(normalize(directionToLight), normalize(input.normal));
        float RdotE = dot(toEyePoint, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        specularPoint = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
    }
    // BlinnPhong反射なら
    else if (gMaterial.specularSetting == 2) {
        
        float3 halfVector = normalize()
    }
    
    /// ================================================== ///
	/// 光の合成
	/// ================================================== ///
    
    float3 resultColor = float3(0.0f, 0.0f, 0.0f);
    
    if (gMaterial.diffuseSetting == 0 && gMaterial.specularSetting == 0) {
        
        // そのままを返す
        resultColor = gMaterial.color.rgb * textureColor.rgb;
    }
    else {
        
        // Diffuse と Specular を足す
        resultColor = diffuseDirectional + specularDirectional;
        //resultColor = diffusePoint + specularPoint;
    }
    
    /// ================================================== ///
	/// 最終出力
	/// ================================================== ///
    
    PixelShaderOutput output = { float4(0.0f, 0.0f, 0.0f, 1.0f) };
    
    output.color.rgb = resultColor;
    output.color.a = gMaterial.color.a * textureColor.a;
    
    // 透明なピクセルは破棄
    if (output.color.a == 0.0f) {
        discard;
    }
    
    return output;
}