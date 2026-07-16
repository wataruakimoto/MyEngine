#include "PlayerFallState.h"
#include "Particle/ParticleEmitter.h"

#include <cmath>

using namespace Engine;

/// ================================================== ///
/// 初期化
void PlayerFallState::Initialize(const FallStateContext& context) {

	// コンテキストを保存
	context_ = context;
}

/// ================================================== ///
/// 更新
void PlayerFallState::Update() {

	// 経過時間を進める
	fallTimer_ += 1.0f / 60.0f;

	// 回転速度の加算
	fallRotateVelocity_.x += kRollAcceleration_;
	fallRotateVelocity_.z += kRollAcceleration_ * 0.5f;

	// 回転の更新
	context_.worldTransform->AddRotate(fallRotateVelocity_);

	// 落下速度の加算
	fallVelocity_.y += kFallAcceleration_;

	// 最大落下速度を超えないようにする
	if (fallVelocity_.y < kMaxFallSpeed_) {

		fallVelocity_.y = kMaxFallSpeed_;
	}

	// 横揺れの計算
	float swayX = sinf(fallTimer_ * kSwayFrequency_) * kSwayAmplitude_;
	float swayZ = cosf(fallTimer_ * kSwayFrequency_ * 0.5f) * kSwayAmplitude_;

	// 座標の更新
	context_.worldTransform->AddTranslate({ swayX * 0.05f, fallVelocity_.y, swayZ * 0.05f });

	// プレイヤーのスケール分を考慮した地面の高さ
	float groundY = kGroundHeight_ + context_.scale.y;

	// 地面に到達したら
	if (context_.worldTransform->GetWorldPosition().y <= groundY) {

		// Y座標を地面の高さに揃える
		Vector3 position = context_.worldTransform->GetWorldPosition();
		position.y = groundY;
		context_.worldTransform->SetTranslate(position);

		isGroundHit_ = true;

		// まだ着地パーティクルを発生させていなければ
		if (!isParticleEmitted_) {

			// 着地点にエミッターを移動させて発生させる
			if (context_.deathEmitterBlue) {
				context_.deathEmitterBlue->SetTranslate(position);
				// SetTranslateだけではワールド行列に反映されないため、Emit前に明示的に更新する
				context_.deathEmitterBlue->GetWorldTransform().Update();
				context_.deathEmitterBlue->Emit();
			}

			isParticleEmitted_ = true;
		}
	}
}

/// ================================================== ///
/// 状態に入ったときの処理
void PlayerFallState::Enter() {

	// タイマーをリセット
	fallTimer_ = 0.0f;

	// 落下速度・回転速度を設定
	fallVelocity_ = { 0.0f, kFallStartSpeed_, 0.0f };
	fallRotateVelocity_ = { kRollSpeed_, 0.0f, kRollSpeed_ * 0.5f };

	// フラグをリセット
	isGroundHit_ = false;
	isParticleEmitted_ = false;
}

/// ================================================== ///
/// 状態から出るときの処理
void PlayerFallState::Exit() {
}
