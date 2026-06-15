#include "PlayerAutoState.h"

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

	// Z方向にのみの移動
	context_.worldTransform.AddTranslate({ 0.0f, 0.0f, moveSpeed_ });
}

/// ================================================== ///
/// 加速処理
void PlayerAutoState::Accelerate() {
}

/// ================================================== ///
/// 減速処理
void PlayerAutoState::Decelerate() {
}
