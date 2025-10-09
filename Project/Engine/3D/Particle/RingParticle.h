#pragma once

#include "BaseParticleType.h"

#include <numbers>

/// === リング用のパーティクル === ///
class RingParticle : public BaseParticleType {

///-------------------------------------------///
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ParticleGroup* group) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 頂点データ生成
	/// </summary>
	void GenerateVertexData() override;

	/// <summary>
	/// 参照データ生成
	/// </summary>
	void GenerateIndexData() override;

	/// <summary>
	/// マテリアルデータ生成
	/// </summary>
	void GenerateMaterialData() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// リングの設定
	const uint32_t kRingDivide = 32; // 円の分割数
	const float kOuterRadius = 1.0f; // 外側の半径
	const float kInnerRadius = 0.2f; // 内側の半径
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide); // 分割あたりのラジアン 2π/分割数

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	// 1秒間のUVの移動量
	float kUVSpeed = 0.1f; // 10秒で1周
};
