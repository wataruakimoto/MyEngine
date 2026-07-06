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
	mover_->Initialize(context_.worldTransform, reticle_.get(), context_.moveSpeed);

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
}

/// ================================================== ///
/// 状態から出るときの処理
void PlayerManualState::Exit() {
}

/// ================================================== ///
/// 被弾時の処理
void PlayerManualState::OnHit() {
}
