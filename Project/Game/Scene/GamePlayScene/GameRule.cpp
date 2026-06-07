#include "GameRule.h"

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
