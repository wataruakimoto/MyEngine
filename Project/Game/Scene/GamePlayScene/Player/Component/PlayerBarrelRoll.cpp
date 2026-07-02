#include "PlayerBarrelRoll.h"

#include "input.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

/// ================================================== ///
/// 初期化
void PlayerBarrelRoll::Initialize() {

	// 入力のインスタンスを取得
	input_ = Input::GetInstance();
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

			preEaseValue_ = 0.0f;

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

}
