#include "object3d.hlsli"

struct Camera {
    float3 worldPosition;
};

struct Material {
    float4 color;           // 色
    float3 specularColor;   // 鏡面反射色
    float padding;
    uint diffuseSetting;    // 拡散反射の設定 0:使わない 1:Lambert反射 2:HalfLambert反射 
    uint specularSetting;   // 鏡面反射の設定 0:使わない 1:Phong反射 2:BlinnPhong反射
    uint useEnvironmentMap; // 環境マップの設定 0:使わない 1:使う
    float shininess;        // 明るさ
    float4x4 uvTransform;   // UV座標
};

struct DirectionalLight {
    uint active;      // 有効フラグ 0:無効 1:有効
    float3 color;     // 色
    float3 direction; // 向き
    float intensity;  // 輝度
};

struct PointLight {
    uint active;      // 有効フラグ 0:無効 1:有効
    float3 color;     // 色
    float3 position;  // 位置
    float intensity;  // 輝度
    float range;      // 距離
    float decay;      // 減衰率
};

struct SpotLight {
    uint active;           // 有効フラグ 0:無効 1:有効
    float3 color;          // 色
    float3 position;       // 位置
    float padding;         // パディング
    float3 direction;      // 向き
    float intensity;       // 輝度
    float range;           // 距離
    float decay;           // 減衰率
    float cosAngle;        // 角度
    float cosFalloffStart; // 減衰開始角度
};

struct Environment {
    float intensity; // 環境光の強度
};

ConstantBuffer<Camera> gCamera : register(b1);

ConstantBuffer<Material> gMaterial : register(b2);

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b3);

ConstantBuffer<PointLight> gPointLight : register(b4);

ConstantBuffer<SpotLight> gSpotLight : register(b5);

ConstantBuffer<Environment> gEnvironment : register(b6);

Texture2D<float4> gTexture : register(t0);

TextureCube<float4> gEnvironmentTexture : register(t1);

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

void CalculateDirectionalLight(VertexShaderOutput input, float3 toEye, out float3 diffuse, out float3 specular) {
    
    /// ========== 拡散反射の計算 ========== ///
    
    diffuse = float3(0.0f, 0.0f, 0.0f);
    
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
    
    // Lambert反射なら
    if (gMaterial.diffuseSetting == 1) {
        
        float cos = saturate(NdotL);
        
        diffuse = gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    // HalfLambert反射なら
    else if (gMaterial.diffuseSetting == 2) {
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        diffuse = gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    
    /// ========== 鏡面反射の計算 ========== ///
    
    specular = float3(0.0f, 0.0f, 0.0f);
    
    // Phong反射なら
    if (gMaterial.specularSetting == 1) {
        
        float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
        float RdotE = dot(toEye, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        specular = gDirectionalLight.color * gDirectionalLight.intensity * specularPow;
    }
    // BlinnPhong反射なら
    else if (gMaterial.specularSetting == 2) {
        
        float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        specular = gDirectionalLight.color * gDirectionalLight.intensity * specularPow;
    }
}

void CalculatepointLight(VertexShaderOutput input, float3 toEye, out float3 diffuse, out float3 specular) {
    
    float3 directionToLight = input.worldPosition - gPointLight.position;
    
    float distanceToLight = length(directionToLight);
    
    float factor = pow(saturate(-distanceToLight / gPointLight.range + 1.0f), gPointLight.decay);
    
    /// ========== 拡散反射の計算 ========== ///
    
    diffuse = float3(0.0f, 0.0f, 0.0f);
    
    float NdotL = dot(normalize(input.normal), -normalize(directionToLight));
    
    // Lambert反射なら
    if (gMaterial.diffuseSetting == 1) {
        
        float cos = saturate(NdotL);
        
        diffuse = gPointLight.color * cos * gPointLight.intensity * factor;
    }
    // HalfLambert反射なら
    else if (gMaterial.diffuseSetting == 2) {
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        diffuse = gPointLight.color * cos * gPointLight.intensity * factor;
    }
    
    /// ========== 鏡面反射の計算 ========== ///
    
    specular = float3(0.0f, 0.0f, 0.0f);
    
    // Phong反射なら
    if (gMaterial.specularSetting == 1) {
        
        float3 reflectLight = reflect(normalize(directionToLight), normalize(input.normal));
        float RdotE = dot(toEye, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        specular = gPointLight.color * gPointLight.intensity * specularPow * factor;
    }
    // BlinnPhong反射なら
    else if (gMaterial.specularSetting == 2) {
        
        float3 halfVector = normalize(-directionToLight + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        specular = gPointLight.color * gPointLight.intensity * specularPow * factor;
    }
}

void CalculateSpotLight(VertexShaderOutput input, float3 toEye, out float3 diffuse, out float3 specular) {
    
    float3 directionToSpotLight = input.worldPosition - gSpotLight.position;
    
    float distanceToSpotLight = length(directionToSpotLight);
    
    float spotCosAngle = dot(normalize(directionToSpotLight), gSpotLight.direction);
        
    float attenuationFactor = pow(saturate(-distanceToSpotLight / gSpotLight.range + 1.0f), gSpotLight.decay);
    
    float falloffFactor = saturate((spotCosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
    
    /// ========== 拡散反射の計算 ========== ///
    
    diffuse = float3(0.0f, 0.0f, 0.0f);
    
    float NdotL = dot(normalize(input.normal), -normalize(directionToSpotLight));
    
    // Lambert反射なら
    if (gMaterial.diffuseSetting == 1) {
        
        float cos = saturate(NdotL);
        
        diffuse = gSpotLight.color * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
    }
    // HalfLambert反射なら
    else if (gMaterial.diffuseSetting == 2) {
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        diffuse = gSpotLight.color * cos * gSpotLight.intensity * attenuationFactor * falloffFactor;
    }
    
    /// ========== 鏡面反射の計算 ========== ///
    
    specular = float3(0.0f, 0.0f, 0.0f);
    
    // Phong反射なら
    if (gMaterial.specularSetting == 1) {
        
        float3 reflectLight = reflect(normalize(directionToSpotLight), normalize(input.normal));
        float RdotE = dot(toEye, reflectLight);
        float specularPow = pow(saturate(RdotE), gMaterial.shininess); // 反射強度
        
        specular = gSpotLight.color * gSpotLight.intensity * specularPow * attenuationFactor * falloffFactor;
    }
    // BlinnPhong反射なら
    else if (gMaterial.specularSetting == 2) {
        
        float3 halfVector = normalize(-directionToSpotLight + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
        float specularPow = pow(saturate(NdotH), gMaterial.shininess); // 反射強度
        
        specular = gSpotLight.color * gSpotLight.intensity * specularPow * attenuationFactor * falloffFactor;
    }
}

PixelShaderOutput main(VertexShaderOutput input) {
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
    
    /// ========== 平行光源の計算 ========== ///
    
    float3 diffuseDirectional = float3(0.0f, 0.0f, 0.0f);
    float3 specularDirectional = float3(0.0f, 0.0f, 0.0f);
    
    if (gDirectionalLight.active != 0) {
    
        CalculateDirectionalLight(input, toEye, diffuseDirectional, specularDirectional);
    }
    
    /// ========== 点光源の計算 ========== ///
    
    float3 diffusePoint = float3(0.0f, 0.0f, 0.0f);
    float3 specularPoint = float3(0.0f, 0.0f, 0.0f);
    
    if (gPointLight.active != 0) {
    
        CalculatepointLight(input, toEye, diffusePoint, specularPoint);
    }
    
    /// ========== スポットライトの計算 ========== ///
    
    float3 diffuseSpot = float3(0.0f, 0.0f, 0.0f);
    float3 specularSpot = float3(0.0f, 0.0f, 0.0f);
    
    if (gSpotLight.active != 0) {
    
        CalculateSpotLight(input, toEye, diffuseSpot, specularSpot);
    }
    
    /// ================================================== ///
	/// 光の合成
	/// ================================================== ///
    
    /// ========== 拡散反射を合成 ========== ///
    
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    
    // 反射させないなら
    if (gMaterial.diffuseSetting == 0) {
        
        // そのままを返す
        diffuse = gMaterial.color.rgb * textureColor.rgb;
    }
    // 反射させるなら
    else {
        
        // 合成させる
        diffuse = gMaterial.color.rgb * textureColor.rgb * (diffuseDirectional + diffusePoint + diffuseSpot);
    }
    
    /// ========== 鏡面反射を合成 ========== ///
    
    float3 specular = float3(0.0f, 0.0f, 0.0f);
    
    // 反射させないなら
    if (gMaterial.specularSetting == 0) {
        
        // 何もしない(黒のまま返す)
    }
    // 反射させるなら
    else {
       
        // 合成させる
        specular = gMaterial.specularColor * (specularDirectional + specularPoint + specularSpot);
    }
    
    // 反射を合成
    float3 resultColor = diffuse + specular;
    
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