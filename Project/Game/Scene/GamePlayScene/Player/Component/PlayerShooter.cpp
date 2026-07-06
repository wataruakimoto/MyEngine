#include "PlayerShooter.h"
#include "GameObjectManager.h"
#include "Reticle/Reticle.h"
#include "../Bullet.h"

#include "input.h"
#include "MathVector.h"
#include "Easing.h"

using namespace Engine;
using namespace MathVector;
using namespace Easing;

/// ================================================== ///
/// 初期化
void PlayerShooter::Initialize(WorldTransform* playerWorldTransform, Vector3* defaultScale, Reticle* reticle) {

	// オブジェクトマネージャーのインスタンスを取得
	objManager_ = GameObjectManager::GetInstance();

	// 入力のインスタンスを取得
	input_ = Input::GetInstance();

	// プレイヤーのワールド変換を保存
	playerWorldTransform_ = playerWorldTransform;

	// デフォルトの大きさを保存
	defaultScale_ = *defaultScale;

	// レティクルを保存
	reticle_ = reticle;
}

/// ================================================== ///
/// 更新
void PlayerShooter::Update() {

	/// ========== 射撃処理 ========== ///

	// タイマーが0以上のとき
	if (fireTimer_ > 0.0f) {

		// タイマーを減算
		fireTimer_ -= 1.0f / 60.0f;
	}

	// 左クリックが押されているとき
	if (input_->PushMouseButton(MouseButton::Left)) {

		// タイマーが0以下のとき
		if (fireTimer_ <= 0.0f) {

			// 射撃
			Fire();
		}
	}

	/// ========== 射撃アニメーション ========== ///

	// 射撃アニメーション中のとき
	if (isFireAnimating_) {

		FireAnimation();
	}
}

/// ================================================== ///
/// 射撃
void PlayerShooter::Fire() {

	// 弾の生成&初期化
	std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>();
	bullet->Initialize();

	// プレイヤーのワールド座標を取得
	Vector3 playerPos_ = playerWorldTransform_->GetWorldPosition();

	// 弾の位初期置をプレイヤーの位置に設定
	bullet->GetWorldTransform().SetTranslate(playerPos_);

	// レティクルのワールド座標を取得
	Vector3 reticlePos_ = reticle_->GetWorldTransform().GetWorldPosition();

	// 方向ベクトルを計算
	Vector3 direction = reticlePos_ - playerPos_;

	// 正規化
	direction = Normalize(direction);

	// 弾の方向を設定
	bullet->SetDirection(direction);

	// ゲームプレイシーンの弾をリストに登録
	objManager_->AddPlayerBullet(std::move(bullet));

	// タイマーをリセット
	fireTimer_ = kFireInterval_;

	// 射撃アニメーションを開始
	isFireAnimating_ = true;
	fireAnimationProgress_ = 0.0f;
	playerWorldTransform_->SetScale(kFireScale_);
}

/// ================================================== ///
/// 射撃アニメーション
void PlayerShooter::FireAnimation() {

	// アニメーションの進行度を加算
	fireAnimationProgress_ += 1.0f / 60.0f;

	// 進行度を計算
	float t = fireAnimationProgress_ / kFireAnimationDuration_;

	// 縮小から元の大きさに戻すアニメーション
	Vector3 scale = Lerp(kFireScale_, defaultScale_, EaseOutCubic(t));

	// 大きさを設定
	playerWorldTransform_->SetScale(scale);

	// 進行度が間隔に達している場合
	if (fireAnimationProgress_ >= kFireAnimationDuration_) {

		// アニメーション終了
		isFireAnimating_ = false;

		// 大きさを元に戻す
		playerWorldTransform_->SetScale(defaultScale_);
	}
}
