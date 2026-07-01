#include "PlayerAutoState.h"

#include <algorithm>

using namespace Engine;

/// ================================================== ///
/// 初期化
void PlayerAutoState::Initialize(const AutoStateContext& context) {

	// コンテキストを保存
	context_ = context;
}

/// ================================================== ///
/// 更新
void PlayerAutoState::Update() {

	// 加速フラグが経っているなら
	if (context_.isAccelerating) {

		// 加速処理
		Accelerate();
	}
	// 減速フラグが経っているなら
	else if (context_.isDecelerating) {

		// 減速処理
		Decelerate();
	}

	// Z方向にのみの移動
	context_.worldTransform->AddTranslate({0.0f, 0.0f, *context_.moveSpeed});
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
	*context_.moveSpeed = (std::min)(*context_.moveSpeed + acceleration_, kMaxMoveSpeed);
}

/// ================================================== ///
/// 減速処理
void PlayerAutoState::Decelerate() {

	// 徐々に減速
	// 0に達するまで減速
	*context_.moveSpeed = (std::max)(*context_.moveSpeed - acceleration_, kMinMoveSpeed);
}
