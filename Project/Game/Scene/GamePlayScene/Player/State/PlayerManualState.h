#pragma once

#include "IPlayerState.h"
#include "WorldTransform.h"
#include "Reticle/Reticle.h"
#include "../Component/PlayerMover.h"
#include "../Component/PlayerShooter.h"
#include "../Component/PlayerBarrelRoll.h"

#include <memory>
#include <numbers>

/// === 前方宣言 === ///

namespace Engine {
	class Camera;
}

/// <summary>
/// マニュアル操作状態のコンテキスト
/// </summary>
struct ManualStateContext {

	Engine::WorldTransform* worldTransform = nullptr; // ワールド変換
	Engine::Vector3 defaultScale = { 1.0f, 1.0f, 1.0f }; // デフォルトの拡縮
	Engine::Camera* camera = nullptr;				  // カメラ
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
	void Initialize(const ManualStateContext& context);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// UIの描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 状態に入ったときの処理
	/// </summary>
	void Enter() override;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	void Exit() override;

	/// <summary>
	/// 被弾時の処理
	/// </summary>
	void OnHit();

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// 描画可能かどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsVisible() const { return isVisible_; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// コンテキスト
	ManualStateContext context_;

	// レティクル
	std::unique_ptr<Reticle> reticle_ = nullptr;

	/// ========== コンポーネント ========== ///

	// 移動コンポーネント
	std::unique_ptr<PlayerMover> mover_ = nullptr;

	// 射撃コンポーネント
	std::unique_ptr<PlayerShooter> shooter_ = nullptr;

	// バレルロールコンポーネント
	std::unique_ptr<PlayerBarrelRoll> barrelRoll_ = nullptr;

	/// ========== 移動速度用 ========== ///

	// 移動速度
	float moveSpeed_ = 0.0f;

	// 最大移動速度
	const float kMaxMoveSpeed = 1.0f;

	// 被弾時の移動速度
	const float kHitMoveSpeed = 0.2f;

	// 毎フレームの移動速度の復帰量
	const float kMoveSpeedRecoveryRate = 0.01f;

	/// ========== 無敵・点滅用 ========== ///

	// 無敵タイマー
	float invincibleTimer_ = 0.0f;

	// 無敵時間 (秒)
	const float kInvincibleDuration = 2.0f;

	// 点滅間隔 (秒)
	const float kBlinkInterval = 0.1f;

	// 点滅タイマー
	float blinkTimer_ = 0.0f;

	// 描画するかどうか
	bool isVisible_ = true;
};

