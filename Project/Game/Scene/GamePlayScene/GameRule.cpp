#include "GameRule.h"

#include <algorithm>

/// ================================================== ///
/// 初期化
void GameRule::Initialize() {

	isClear_ = false;

	isGameOver_ = false;

	remainingTime_ = kInitialTime;
}

/// ================================================== ///
/// 更新
void GameRule::Update() {

	// クリア、ゲームオーバーなら更新しない
	if (isClear_ || isGameOver_) return;

	// 残り時間を減らす
	remainingTime_ -= kDeltaTime;

	// 残り時間が0以下になったら
	if (remainingTime_ <= 0.0f) {

		// 残り時間を0にする
		remainingTime_ = 0.0f;

		// ゲームオーバーにする
		isGameOver_ = true;
	}
}

/// ================================================== ///
/// ボーナスタイムの追加
void GameRule::AddBonusTime(float time) {

	// 残り時間に追加
	remainingTime_ = std::min(remainingTime_ + time, kMaxTime);
}

/// ================================================== ///
/// ゴール到達の通知
void GameRule::NotifyGoalReached() {

	// ゲームオーバーではないなら
	if (!isGameOver_) {

		// クリアにする
		isClear_ = true;
	}
}
