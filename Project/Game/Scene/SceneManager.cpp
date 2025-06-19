#include "SceneManager.h"
#include <cassert>

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

		// シーンマネージャーをセット
		scene_->SetSceneManager(this);

		// カメラをセット
		scene_->SetCamera(camera_);

		// 次のシーンを初期化
		scene_->Initialize();
	}

	// 実行中のシーンを更新
	scene_->Update();
}

void SceneManager::Draw() {

	scene_->Draw();
}

void SceneManager::Finalize() {

	// 実行していたシーンを終了&解放
	scene_->Finalize();
	delete scene_;

	delete instance;
	instance = nullptr;
}

void SceneManager::ChangeScene(const std::string& sceneName) {

	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	// ファクトリーから次のシーンを生成する
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}