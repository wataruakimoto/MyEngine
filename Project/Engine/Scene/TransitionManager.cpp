#include "TransitionManager.h"
#include "BaseTransition.h"

using namespace Engine;

void TransitionManager::Update() {

	// 遷移が進行中でなければ更新しない
	if (state_ == TransitionState::None || !currentTransition_) return;

	// 進行度を計算
	float addProgress = deltaTime_ / duration_;

	switch (state_) {

	case TransitionManager::TransitionState::None:

		// 何もしない

		break;

	case TransitionManager::TransitionState::Out:

		// 遷移の進行度を加算
		progress_ += addProgress;

		// 遷移が完了したら
		if (progress_ >= 1.0f) {

			// 進行度を1に固定
			progress_ = 1.0f;

			// コールバック関数があれば呼び出す
			if (onTransitionComplete_) {

				onTransitionComplete_();
			}
		}

		break;

	case TransitionManager::TransitionState::In:

		// 遷移の進行度を減算
		progress_ -= addProgress;

		// 遷移が完了したら
		if (progress_ <= 0.0f) {

			// 進行度を0に固定
			progress_ = 0.0f;

			// 遷移状態を遷移なしに設定
			state_ = TransitionState::None;

			// 現在の遷移を破棄
			currentTransition_.reset();
		}

		break;
	}

	// 現在の遷移を更新
	currentTransition_->Update(progress_);
}

void TransitionManager::Draw() {

	// 遷移が進行中でなければ更新しない
	if (state_ == TransitionState::None || !currentTransition_) return;

	// 現在の遷移を描画
	currentTransition_->Draw();
}

void TransitionManager::Finalize() {

	// インスタンスの解放
	delete instance;
	instance = nullptr;
}

void TransitionManager::StartTransition(std::unique_ptr<BaseTransition> transition, std::function<void()> onComplete, float duration) {

	// 引数をメンバ変数に保存
	currentTransition_ = std::move(transition);
	duration_ = duration;
	onTransitionComplete_ = onComplete;

	// 遷移の進行度をリセット
	progress_ = 0.0f;

	// 遷移状態を開始に設定
	state_ = TransitionState::Out;
}

TransitionManager* TransitionManager::instance = nullptr;

TransitionManager* TransitionManager::GetInstance() {
	
	if (instance == nullptr) {

		instance = new TransitionManager;
	}

	return instance;
}
