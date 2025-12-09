#pragma once

#include "WorldTransform.h"

#include <string>

/// ===== パーティクルエミッター ===== ///
class ParticleEmitter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="effectName">エフェクト名</param>
	ParticleEmitter(std::string	effectName, float frequency, uint32_t count) :
		effectName(effectName),
		frequency(frequency),
		count(count) {
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void Emit();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name="translate">位置</param>
	void SetTranslate(const Vector3& translate) { worldTransform.SetTranslate(translate); }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// エフェクト名
	std::string effectName;

	// 発生頻度 (秒間)
	float frequency = 0.0f;

	// 発生用タイマー
	float timer = 0.0f;

	// デルタタイム
	const float deltaTime = 1.0f / 60.0f;

	// 1回あたりの発生数
	uint32_t count = 10;

	// ワールド変換情報
	WorldTransform worldTransform;
};

