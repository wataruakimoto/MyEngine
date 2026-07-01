#define NOMINMAX

#include "PlayerManualState.h"
#include "MathVector.h"

#include <algorithm>

using namespace Engine;
using namespace MathVector;

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

	/// ========== 移動処理 ========== ///

	// 速度をリセット
	velocity_ = { 0.0f, 0.0f, 0.0f };

	// レティクルに向かって移動
	MoveToReticle();

	// 速度を加算
	context_.worldTransform->AddTranslate(velocity_);

	// 移動範囲の制限
	ClampPosition();

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

/// ================================================== ///
/// レティクルに向かって移動
void PlayerManualState::MoveToReticle() {

	// レティクルのワールド座標を取得
	Vector3 reticlePos = reticle_->GetWorldTransform().GetWorldPosition();

	// プレイヤーのワールド座標を取得
	Vector3 playerPos = context_.worldTransform->GetWorldPosition();

	// レティクルの方向ベクトルを計算
	Vector3 direction = Normalize(reticlePos - playerPos);

	/// ===== 回転の処理 ===== ///

	// 横軸の長さを求める
	float xzLength = Length(direction.x, direction.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(direction.x, direction.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-direction.y, xzLength);

	// 回転を取得
	Vector3 currentRotate = context_.worldTransform->GetRotate();

	// 回転させる
	currentRotate.x = pitch;
	currentRotate.y = yaw;

	// 回転を設定
	context_.worldTransform->SetRotate(currentRotate);

	/// ===== 移動の処理 ===== ///

	// 向きと速さから速度を計算
	velocity_ += direction * (*context_.moveSpeed);
}

/// ================================================== ///
/// 移動範囲の制限
void PlayerManualState::ClampPosition() {

	// 大きさを取得
	Vector3 scale = context_.worldTransform->GetScale();

	// 加算後の座標を取得
	Vector3 position = context_.worldTransform->GetWorldPosition();

	// 地面の制限
	position.y = std::max(position.y, kMinHeight_ * scale.y);

	// XY平面上の原点からの距離を計算
	float distance = Length(position.x, position.y);

	// 最大距離を計算
	float maxDistance = kMaxRadius_ * scale.x;

	// 最大距離を超えている場合
	if (distance > maxDistance) {
		
		// 最大距離に収まるように比率を計算
		float ratio = maxDistance / distance;

		position.x *= ratio;
		position.y *= ratio;
	}

	// 座標を設定
	context_.worldTransform->SetTranslate(position);
}
