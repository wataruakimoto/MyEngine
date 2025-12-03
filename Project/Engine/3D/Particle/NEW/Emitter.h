#pragma once

#include "WorldTransform.h"

#include <string>

/// ===== パーティクルエミッター ===== ///
class Emitter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="effectName">エフェクト名</param>
	Emitter(std::string	effectName) : effectName(effectName) {};

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
/// メンバ変数
///-------------------------------------------///
private:

	// エフェクト名
	std::string effectName;

	// 発生頻度
	float frequency = 0.0f;

	// 発生用タイマー
	float timer = 0.0f;

	// 1回あたりの発生数
	int count = 10;

	// ワールド変換情報
	WorldTransform worldTransform;
};

