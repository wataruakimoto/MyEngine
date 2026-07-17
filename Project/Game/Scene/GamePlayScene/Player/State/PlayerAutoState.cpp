#include "PlayerAutoState.h"
#include "Particle/ParticleEmitter.h"

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

	// 移動トレイルの発生制御
	if (context_.moveEmitter) {

		// 速度が0より大きいなら
		if (*context_.moveSpeed > 0.0f) {

			// 速度に応じた間隔で発生させる
			context_.moveEmitter->SetFrequency(kBaseEmitFrequency_ / *context_.moveSpeed);
			context_.moveEmitter->SetEmitting(true);
		}
		// 速度が0以下なら
		else {

			// パーティクルを出さない
			context_.moveEmitter->SetEmitting(false);
		}
	}
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
