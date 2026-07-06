#include "PlayerBarrelRoll.h"

#include "input.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

/// ================================================== ///
/// 初期化
void PlayerBarrelRoll::Initialize(WorldTransform* playerWorldTransform) {

	// 入力のインスタンスを取得
	input_ = Input::GetInstance();

	// プレイヤーのワールド変換を保存
	playerWorldTransform_ = playerWorldTransform;
}

/// ================================================== ///
/// 更新
void PlayerBarrelRoll::Update() {

	/// ========== クールダウン処理 ========== ///

	// タイマーが0以上のとき
	if (cooldownTimer_ > 0.0f) {

		// タイマーを減算
		cooldownTimer_ -= 1.0f / 60.0f;
	}

	/// ========== ロール開始判定 ========== ///

	bool isRollLeft = input_->TriggerKey('A'); // 左ロール
	bool isRollRight = input_->TriggerKey('D'); // 右ロール

	// まだロール中でないかつ、タイマーが0以下のとき
	if (!isRolling_ && cooldownTimer_ <= 0.0f) {

		// 左ロールまたは右ロールが押されたとき
		if (isRollLeft || isRollRight) {

			// ロール中フラグを立てる
			isRolling_ = true;

			// タイマーをリセット
			rollTimer_ = 0.0f;

			preEaseT_ = 0.0f;

			// 右ロールではないなら
			if (!isRollRight) {

				// 回転方向を左に設定
				rollDirection_ = -1;
			}
			// 左ロールではないなら
			else if (!isRollLeft) {

				// 回転方向を右に設定
				rollDirection_ = 1;
			}
		}
	}

	/// ========== ロールの更新 ========== ///

	// ロール中のとき
	if (isRolling_) {

		// ロールの更新
		RollUpdate();
	}
}

/// ================================================== ///
/// ロールの更新
void PlayerBarrelRoll::RollUpdate() {

	// ロール進行タイマーを加算
	rollTimer_ += 1.0f / 60.0f;

	// 進行度を計算
	float t = rollTimer_ / kRollDuration_;

	// イージング関数の値を計算
	float easeT = EaseOutCubic(t);

	/// ========== 回転処理 ========== ///

	// 回転角度を計算 回転方向 * -1.0f * 1回転にかかる角度 * イージング関数の値
	float angle = rollDirection_ * -1.0f * kRollAngle_ * easeT;

	// 現在のプレイヤーの向きを取得
	Vector3 currentRotation = playerWorldTransform_->GetRotate();

	// Z軸のみ回転を更新
	currentRotation.z = angle;

	// 回転を更新
	playerWorldTransform_->SetRotate(currentRotation);

	/// ========== 横移動処理 ========== ///

	// 前フレームとのイージング値の差分を計算
	float delta = easeT - preEaseT_;

	// 横移動量を計算 回転方向 * ロール時の横移動距離 * 差分
	float moveAmount = rollDirection_ * kRollMoveDistance_ * delta;

	// プレイヤーのワールド変換に横移動を加算
	playerWorldTransform_->AddTranslate({ moveAmount, 0.0f, 0.0f });

	// 前フレームのイージング値を更新
	preEaseT_ = easeT;

	/// ========== ロール終了判定 ========== ///

	// ロール進行タイマーがロールにかかる時間以上のとき
	if (rollTimer_ >= kRollDuration_) {

		// ロール中フラグを下ろす
		isRolling_ = false;

		// タイマーをリセット
		cooldownTimer_ = kCooldownDuration_;

		// 現在のプレイヤーの向きを取得
		Vector3 currentRotation = playerWorldTransform_->GetRotate();

		// Z軸のみ回転をリセット
		currentRotation.z = 0.0f;

		// 回転を更新
		playerWorldTransform_->SetRotate(currentRotation);
	}
}
