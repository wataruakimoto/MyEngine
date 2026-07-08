#define NOMINMAX

#include "PlayerMover.h"
#include "Reticle/Reticle.h"

#include "MathVector.h"

using namespace Engine;
using namespace MathVector;

/// ================================================== ///
/// 初期化
void PlayerMover::Initialize(WorldTransform* playerWorldTransform, Reticle* reticle, float* moveSpeed) {

	// プレイヤーのワールド変換を保存
	playerWorldTransform_ = playerWorldTransform;

	// レティクルを保存
	reticle_ = reticle;

	// 移動の速さを保存
	moveSpeed_ = moveSpeed;
}

/// ================================================== ///
/// 更新
void PlayerMover::Update() {

	// 速度をリセット
	velocity_ = { 0.0f, 0.0f, 0.0f };

	// レティクルに向かって移動
	MoveToReticle();

	// 速度を加算
	playerWorldTransform_->AddTranslate(velocity_);

	// 移動範囲の制限
	ClampPosition();
}

/// ================================================== ///
/// レティクルに向かって移動
void PlayerMover::MoveToReticle() {
	
	// レティクルのワールド座標を取得
	Vector3 reticlePos_ = reticle_->GetWorldTransform().GetWorldPosition();

	/// プレイヤーのワールド座標を取得
	Vector3 playerPos_ = playerWorldTransform_->GetWorldPosition();

	// レティクルの方向ベクトルを計算
	Vector3 direction = Normalize(reticlePos_ - playerPos_);

	/// ===== 回転の処理 ===== ///

	// 横軸の長さを求める
	float xzLength = Length(direction.x, direction.z);

	// ヨー(Y軸回りの回転)を求める
	float yaw = atan2f(direction.x, direction.z);

	// ピッチ(X軸回りの回転)を求める
	float pitch = atan2f(-direction.y, xzLength);

	// 回転を取得
	Vector3 currentRotate = playerWorldTransform_->GetRotate();

	// 回転させる
	currentRotate.x = pitch;
	currentRotate.y = yaw;

	// 回転を設定
	playerWorldTransform_->SetRotate(currentRotate);

	/// ===== 移動の処理 ===== ///

	// 向きと速さから速度を計算
	velocity_ += direction * (*moveSpeed_);
}

/// ================================================== ///
/// 移動範囲の制限
void PlayerMover::ClampPosition() {

	// 大きさを取得
	Vector3 scale = playerWorldTransform_->GetScale();

	// 加算後の座標を取得
	Vector3 position = playerWorldTransform_->GetTranslate();

	// 地面の制限
	position.y = std::max(position.y, kMinHeight_ + scale.y);

	// XY平面上の原点からの距離を計算
	float distance = Length(position.x, position.y);

	// 最大距離を計算
	float maxDistance = kMaxRadius_ - scale.x;

	// 最大距離を超えている場合
	if (distance > maxDistance) {

		// 最大距離に収まるように比率を計算
		float ratio = maxDistance / distance;

		position.x *= ratio;
		position.y *= ratio;
	}

	// 座標を設定
	playerWorldTransform_->SetTranslate(position);
}
