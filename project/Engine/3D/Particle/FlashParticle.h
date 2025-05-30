#pragma once
#include "BaseParticleType.h"
#include <random>

class FlashParticle : public BaseParticleType {

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

	/// <summary>
	/// ランダムで頂点データを生成
	/// </summary>
	void GenerateSpike();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// スパイクの設定
	const uint32_t kNumSpike = 12; // スパイクの数
	const float kMinLength = 0.1f; // 最小の長さ
	const float kMaxLength = 1.0f; // 最大の長さ
	const float angleStep = 2.0f * std::numbers::pi_v<float> / float(kNumSpike); // 分割あたりのラジアン 2π/分割数

	// 最初の右の座標
	Vector2 firstRight = {};

	// 一個前の左側の座標
	Vector2 preLeft = {};

	// 乱数生成器
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
};
