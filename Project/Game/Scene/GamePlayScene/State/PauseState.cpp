#include "PauseState.h"
#include "GamePlayScene.h"
#include "UI/GuideUI.h"

#include "OffscreenRendering/FilterManager.h"
#include "TransitionManager.h"
#include "Transition/FadeTransition.h"
#include "SceneManager.h"
#include "Input.h"

using namespace Engine;

/// ================================================== ///
/// 初期化
void PauseState::Initialize(GamePlayScene* scene) {

	// インスタンスの取得
	filterManager_ = FilterManager::GetInstance();
	transitionManager_ = TransitionManager::GetInstance();

	// 引数をメンバ変数にセット
	scene_ = scene;

	// ポーズUIの生成・初期化
	pauseUI = std::make_unique<PauseUI>();
	pauseUI->Initialize();

	isSelectedRestart_ = false;

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();
}

/// ================================================== ///
/// 更新
void PauseState::Update() {

	if (isSelectedRestart_) {

		vignetteFilter_->SetIsActive(false);

		SceneManager::GetInstance()->ChangeScene("PLAY");

		return;
	}

	// ポーズUIの更新
	pauseUI->Update();

	// 現在の選択肢を取得
	PauseSelect selected = pauseUI->GetCurrentSelect();

	// 選択肢に応じた処理
	switch (selected) {

	case PauseSelect::Resume:

		scene_->TogglePause();

		isFinished_ = true;

		return;

	case PauseSelect::Restart:

		if (!isSelectedRestart_) {

			// 白フェードの開始
			transitionManager_->StartInTransition(
				std::make_unique<FadeTransition>(Vector3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f),
				[]() {},
				2.0f // 遷移にかける時間 (秒)
			);

			isSelectedRestart_ = true;

			vignetteFilter_->SetIsActive(false);

			scene_->Restart();
		}

		break;

	case PauseSelect::Quit:

		vignetteFilter_->SetIsActive(false);

		SceneManager::GetInstance()->ChangeScene("TITLE");

		return;

	case PauseSelect::None:
		// 何もしない
		break;
	}

	if (Input::GetInstance()->TriggerKey(VK_ESCAPE)) {

		scene_->TogglePause();

		return;
	}
}

/// ================================================== ///
/// 描画
void PauseState::Draw() {

	// ポーズUIの描画
	pauseUI->Draw();
}