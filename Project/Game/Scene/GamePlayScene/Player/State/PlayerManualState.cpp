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
}

/// ================================================== ///
/// 更新
void PlayerManualState::Update() {

	// レティクルの更新
	reticle_->Update();
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
