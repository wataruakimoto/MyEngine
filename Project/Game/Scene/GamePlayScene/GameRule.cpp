#include "GameRule.h"
#include "GamePlayScene.h"

/// ================================================== ///
/// 初期化
void GameRule::Initialize() {

	isClear_ = false;

	isGameOver_ = false;

	remainingTime_ = initialTime_;
}

/// ================================================== ///
/// 更新
void GameRule::Update() {

	// クリア、ゲームオーバーなら更新しない
	if (isClear_ || isGameOver_) return;

	// 残り時間を減らす
	remainingTime_ -= deltaTime_;
}

/// ================================================== ///
/// ボーナスタイムの追加
void GameRule::AddBonusTime(float time) {

	// 残り時間に追加
	remainingTime_ += time;

	// 残り時間が最大時間を超えたら
	if (remainingTime_ > maxTime_) {

		// 残り時間を最大時間に設定
		remainingTime_ = maxTime_;
	}
}
