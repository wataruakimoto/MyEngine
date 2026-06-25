#pragma once

#include "IPlayerState.h"
#include "Player/PlayerCommand.h"
#include "WorldTransform.h"
#include "Vector2.h"

#include <numbers>

/// <summary>
/// マニュアル操作状態のコンテキスト
/// </summary>
struct ManualStateContext {
	Engine::WorldTransform* worldTransform = nullptr; // ワールド変換
	float* moveSpeed = nullptr;						  // 移動の速さ
	bool* isLockOnMode = nullptr;					  // ロックオンモードフラグ
};

/// <summary>
/// マニュアル操作状態
/// </summary>
class PlayerManualState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Initiaize(const ManualStateContext& context);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 状態に入ったときの処理
	/// </summary>
	void Enter() override;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	void Exit() override;

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 射撃処理
	/// </summary>
	/// <param name="context">コンテキスト</param>
	void Fire(PlayerContext context);

	/// <summary>
	/// 射撃アニメーション更新
	/// </summary>
	void FireAnimationUpdate();

	/// <summary>
	/// バレルロール
	/// </summary>
	void Rolling();

	/// <summary>
	/// レティクルに向かって移動
	/// </summary>
	void MoveToReticle();

	/// <summary>
	/// 位置の制限
	/// </summary>
	void ClampPosition();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	ManualStateContext context_;

	/// ===== コマンド ===== ///

	std::unique_ptr<NormalShotCommand> normalShotCommand_ = nullptr; // 通常射撃コマンド

	std::unique_ptr<LockOnAimCommand> lockOnAimCommand_ = nullptr; // ロックオン照準コマンド

	std::unique_ptr<LockOnShotCommand> lockOnShotCommand_ = nullptr; // ロックオン射撃コマンド

	std::unique_ptr<BarrelRollCommand> barrelRollCommand_ = nullptr; // バレルロールコマンド

	/// ==========  ========== ///

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	float pressTimer_ = 0.0f;
	bool isLockOnMode_ = false;
	const float kLockOnDuration_ = 0.5f; // ロックオンモード切り替えのための押下時間 (秒)

	float moveSpeedManual = 0.5f;

	// 移動範囲の制限
	const Engine::Vector2 kMoveMin = { -25.0f, 0.0f }; // X,Yの最小値 左下
	const Engine::Vector2 kMoveMax = { 25.0f, 25.0f }; // X,Yの最大値 右上

	/// ===== 射撃アニメーション用 ===== ///

	Engine::Vector3 defaultScale_ = { 1.0f, 1.0f, 1.0f };
	Engine::Vector3 fireScale_ = { 0.8f, 0.8f, 0.8f };

	float fireTimer_ = 0.0f; // カウントダウンタイマー
	float fireAnimationTimer_ = 0.0f; // 射撃アニメーション用カウントダウンタイマー
	const float kFireDuration_ = 0.4f; // 射撃間隔 (秒)
	const float kFireAnimationDuration_ = 0.2f; // 射撃アニメーション時間 (秒)

	bool isFiring_ = false;

	/// ===== バレルロール用 ===== ///

	bool isRolling_ = false; // ロール中フラグ

	float rollTimer_ = 0.0f;    // バレルロール用カウントアップタイマー
	const float rollDuration_ = 0.5f; // ロールにかかる時間（秒）

	float rollCooldownTimer_ = 0.0f; // バレルロールのクールダウン用カウントダウンタイマー
	const float kRollCooldownDuration_ = 0.5f; // クールダウン時間 (秒)

	int rollDirection_ = 0;     // 回転方向 (-1:左, 1:右)

	const float kMaxRollAngle_ = 2.0f * std::numbers::pi_v<float>; // 最大傾き角度 (ラジアン)

	const float kMaxRollMove_ = 10.0f; // 最大移動距離

	float preEaseT_ = 0.0f;

	/// ========== 被弾時用 ========== ///

	// 無敵時間のタイマー（0より大きければ無敵）
	float invincibleTimer_ = 0.0f;

	// ダメージを受けた時の無敵時間の設定 (フレーム数)
	const float kInvincibleTime = 60.0f;
};

