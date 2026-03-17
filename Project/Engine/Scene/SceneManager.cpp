#include "SceneManager.h"
#include "TransitionManager.h"
#include "BaseTransition.h"

#include <cassert>
#include <imgui.h>

using namespace Engine;

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance() {

	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Update() {

	// 次のシーン予約が入っていたら
	if (nextScene_) {

		// 現在のシーンを終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}

		// シーンを切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;

		// 次のシーンを初期化
		scene_->Initialize();
	}

	// 実行中のシーンを更新
	scene_->Update();
}

void SceneManager::DrawFiltered() {

	scene_->DrawFiltered();
}

void SceneManager::DrawUnfiltered() {

	scene_->DrawUnfiltered();

	// 遷移の描画
	TransitionManager::GetInstance()->Draw();
}

void SceneManager::Finalize() {

	// 実行していたシーンを終了&解放
	scene_->Finalize();
	delete scene_;

	delete instance;
	instance = nullptr;
}

void SceneManager::ShowImGui() {

	scene_->ShowImGui();

	sceneFactory_->ShowImGui();
}

void SceneManager::ChangeScene(const std::string& sceneName, std::unique_ptr<BaseTransition> transition, float duration) {

	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// 遷移の指定がなければ
	if (!transition) {

		// 次のシーンを生成してすぐ切り替える
		nextScene_ = sceneFactory_->CreateScene(sceneName);
	}
	// 遷移の指定があれば
	else {

		TransitionManager::GetInstance()->StartInTransition(
			std::move(transition),										   // 遷移の仕方
			[&]() { nextScene_ = sceneFactory_->CreateScene(sceneName); }, // 遷移完了時の処理
			duration													   // 遷移にかける時間
		);
	}
}