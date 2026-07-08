#define NOMINMAX

#include "PlayerManualState.h"

#include "MathVector.h"

#include <algorithm>

using namespace Engine;

/// ================================================== ///
/// 初期化
void PlayerManualState::Initialize(const ManualStateContext& context) {

	// コンテキストを保存
	context_ = context;

	// レティクルの生成
	reticle_ = std::make_unique<Reticle>();
	// レティクルの初期化
	reticle_->Initialize();
	// レティクルのカメラ設定
	reticle_->SetCamera(context_.camera);

	/// ========== コンポーネント ========== ///

	// 移動コンポーネントの生成
	mover_ = std::make_unique<PlayerMover>();
	// 移動コンポーネントの初期化
	mover_->Initialize(context_.worldTransform, reticle_.get(), &moveSpeed_);

	// 射撃コンポーネントの生成
	shooter_ = std::make_unique<PlayerShooter>();
	// 射撃コンポーネントの初期化
	shooter_->Initialize(context_.worldTransform, &context_.defaultScale, reticle_.get());

	// バレルロールコンポーネントの生成
	barrelRoll_ = std::make_unique<PlayerBarrelRoll>();
	// バレルロールコンポーネントの初期化
	barrelRoll_->Initialize(context_.worldTransform);
}

/// ================================================== ///
/// 更新
void PlayerManualState::Update() {

	/// ========== 移動速度の更新 ========== ///

	// 移動速度が最大値より小さい場合
	if (moveSpeed_ < kMaxMoveSpeed) {

		// 移動速度を回復させる
		moveSpeed_ = std::min(moveSpeed_ + kMoveSpeedRecoveryRate, kMaxMoveSpeed);
	}

	/// ========== 無敵・点滅の更新 ========== ///

	// 無敵タイマーが0より大きい場合
	if (invincibleTimer_ > 0.0f) {

		// 無敵タイマーを減算
		invincibleTimer_ -= 1.0f / 60.0f;

		// 点滅タイマーを加算
		blinkTimer_ += 1.0f / 60.0f;

		// 点滅タイマーが間隔より大きい場合
		if (blinkTimer_ >= kBlinkInterval) {

			// 点滅状態を反転
			isVisible_ = !isVisible_;

			// 点滅タイマーをリセット
			blinkTimer_ = 0.0f;
		}

		// 無敵タイマーが0以下になった場合
		if (invincibleTimer_ <= 0.0f) {

			// 無敵タイマーを0に設定
			invincibleTimer_ = 0.0f;

			// 点滅タイマーをリセット
			blinkTimer_ = 0.0f;

			// 点滅状態をリセット
			isVisible_ = true;
		}
	}

	/// ========== 各種コンポーネントの更新 ========== ///

	// レティクルの更新
	reticle_->Update();

	// 移動コンポーネントの更新
	mover_->Update();

	// 射撃コンポーネントの更新
	shooter_->Update();

	// バレルロールコンポーネントの更新
	barrelRoll_->Update();
}

/// ================================================== ///
/// UIの描画
void PlayerManualState::DrawUI() {

	// レティクルの描画
	reticle_->Draw2D();
}

/// ================================================== ///
/// 状態に入ったときの処理
void PlayerManualState::Enter() {

	// 遅い状態からのスタート
	moveSpeed_ = kHitMoveSpeed;

	// 無敵タイマーを0に設定
	invincibleTimer_ = 0.0f;

	// 点滅タイマーをリセット
	blinkTimer_ = 0.0f;

	// 点滅状態をリセット
	isVisible_ = true;
}

/// ================================================== ///
/// 状態から出るときの処理
void PlayerManualState::Exit() {

	// 点滅状態をリセット
	isVisible_ = true;
}

/// ================================================== ///
/// 被弾時の処理
void PlayerManualState::OnHit() {

	// 無敵タイマーが0より大きい場合は処理を行わない
	if (invincibleTimer_ > 0.0f) return;

	// 移動速度をヒット時の速度に設定
	moveSpeed_ = kHitMoveSpeed;

	// 無敵タイマーを設定
	invincibleTimer_ = kInvincibleDuration;

	// 点滅タイマーをリセット
	blinkTimer_ = 0.0f;
}
