#pragma once

#include "WorldTransform.h"

#include <numbers>

/// === 前方宣言 === ///

namespace Engine {
	class Input;
}

/// <summary>
/// プレイヤーのバレルロールコンポーネント
/// </summary>
class PlayerBarrelRoll {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Engine::WorldTransform* playerWorldTransform);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// ロールの更新
	/// </summary>
	void RollUpdate();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ========== ローリング用 ========== ///

	// ロール中フラグ
	bool isRolling_ = false;
	
	// 回転方向 -1: 左回転, 1: 右回転
	int rollDirection_ = 0;

	// ロール進行タイマー
	float rollTimer_ = 0.0f;

	// ロールにかかる時間 (秒)
	const float kRollDuration_ = 0.5f;

	// ロールにかかる角度 (ラジアン)
	const float kRollAngle_ = 2.0f * std::numbers::pi_v<float>; // 2πラジアン = 360度

	// ロール時の横移動距離
	const float kRollMoveDistance_ = 10.0f;

	// 前フレームのイージング値
	float preEaseT_ = 0.0f;

	/// ========== クールダウン用 ========== ///

	// クールダウンタイマー
	float cooldownTimer_ = 0.0f;

	// クールダウンにかかる時間 (秒)
	const float kCooldownDuration_ = 0.5f;

	/// ========== プレイヤーから受け取ってくる変数 ========== ///

	Engine::WorldTransform* playerWorldTransform_ = nullptr;

	/// ========== ポインタ ========== ///

	Engine::Input* input_ = nullptr;
};