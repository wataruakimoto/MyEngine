#include "TransitionManager.h"
#include "BaseTransition.h"

#include <imgui.h>

using namespace Engine;

void TransitionManager::Update() {

	// 遷移がない場合は何もしない
	if (!outTransition_ && !inTransition_) return;

	// 進行度を計算
	float addProgress = deltaTime_ / duration_;

	// 入りの遷移がある場合
	if (outTransition_) {

		// 1 になるまで 進行度を加算する処理
		progress_ = std::min(progress_ + addProgress, 1.0f);

		// 遷移の演出を更新
		outTransition_->Update(progress_);

		// 進行度が 1 になったら
		if (progress_ >= 1.0f) {

			// 入りの遷移をリセット
			outTransition_ = nullptr;

			// コールバック関数を呼び出す
			onTransitionComplete_();
		}

		//
		return;
	}

	// 抜けの遷移がある場合
	if (inTransition_) {

		// 1 になるまで 進行度を加算する処理
		progress_ = std::min(progress_ + addProgress, 1.0f);

		// 遷移の演出を更新
		inTransition_->Update(progress_);

		// 進行度が 1 になったら
		if (progress_ >= 1.0f) {

			// 抜けの遷移をリセット
			inTransition_ = nullptr;

			// コールバック関数を呼び出す
			onTransitionComplete_();
		}

		return;
	}
}

void TransitionManager::Draw() {

	// 入りの遷移演出があるなら
	if (outTransition_) {

		// 描画
		outTransition_->Draw();

		return;
	}

	// 抜けの遷移演出があるなら
	if (inTransition_) {

		// 描画
		inTransition_->Draw();
	}
}

void TransitionManager::Finalize() {

	// インスタンスの解放
	delete instance;
	instance = nullptr;
}

void TransitionManager::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("遷移マネージャ");

	// 進行度を表示
	ImGui::ProgressBar(progress_);

	ImGui::End();

#endif // USE_IMGUI
}

void TransitionManager::StartOutTransition(std::unique_ptr<BaseTransition> transition, std::function<void()> onComplete, float duration) {

	// 引数をメンバ変数に保存
	outTransition_ = std::move(transition);
	duration_ = duration;
	onTransitionComplete_ = onComplete;

	// 遷移の進行度をリセット
	progress_ = 0.0f;
}

void TransitionManager::StartInTransition(std::unique_ptr<BaseTransition> transition, std::function<void()> onComplete, float duration) {

	// 引数をメンバ変数に保存
	inTransition_ = std::move(transition);
	duration_ = duration;
	onTransitionComplete_ = onComplete;

	// 遷移の進行度をリセット
	progress_ = 0.0f;
}

TransitionManager* TransitionManager::instance = nullptr;

TransitionManager* TransitionManager::GetInstance() {
	
	if (instance == nullptr) {

		instance = new TransitionManager;
	}

	return instance;
}
