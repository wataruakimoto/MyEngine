#pragma once

#include "ParticleRendererBase.h"

#include <numbers>

class CylinderRenderer : public ParticleRendererBase {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize() override;

	void Update() override;

	void Draw(uint16_t instanceCount, uint16_t instanceSrvIndex, const std::string& texturePath) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	void GenerateVertexData() override;

	void GenerateIndexData() override;

	void GenerateMaterialData() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// シリンダーの設定
	const uint32_t kCylinderDivide = 32; // 円の分割数
	const float kTopRadius = 1.0f; // 上側の半径
	const float kBottomRadius = 1.0f; // 下側の半径
	const float kHeight = 3.0f; // 高さ
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kCylinderDivide); // 分割あたりのラジアン 2π/分割数

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	// 1秒間のUVの移動量
	float kUVSpeed = 0.1f; // 10秒で1周
};

