#include "PlayerAutoState.h"

#include <algorithm>

/// ================================================== ///
/// 初期化
void PlayerAutoState::Initialize() {
}

/// ================================================== ///
/// 更新
void PlayerAutoState::Update() {

	// 加速フラグが経っているなら
	if (isAccelerating_) {

		// 加速処理
		Accelerate();
	}
	// 減速フラグが経っているなら
	else if (isDecelerating_) {

		// 減速処理
		Decelerate();
	}

	velocity_ = { 0.0f, 0.0f, moveSpeed_ };

	// Z方向にのみの移動
	context_.worldTransform.AddTranslate(velocity_);
}

/// ================================================== ///
/// 状態に入ったときの処理
void PlayerAutoState::Enter() {
}

/// ================================================== ///
/// 状態から出るときの処理
void PlayerAutoState::Exit() {
}

/// ================================================== ///
/// 加速処理
void PlayerAutoState::Accelerate() {

	// 徐々に加速
	// 最大値に達するまで加速
	moveSpeed_ = (std::min)(moveSpeed_ + 0.01f, maxMoveSpeed_);
}

/// ================================================== ///
/// 減速処理
void PlayerAutoState::Decelerate() {

	// 徐々に減速
	// 0に達するまで減速
	moveSpeed_ = (std::max)(moveSpeed_ - 0.01f, minMoveSpeed_);
}
