#include "SceneFactory.h"
#include "TitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "DebugScene.h"

#include <imgui.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {

	// 次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") {

		newScene = new TitleScene();
	}
	else if (sceneName == "PLAY") {

		newScene = new GamePlayScene();
	}
	else if (sceneName == "CLEAR") {

		newScene = new GameClearScene();
	}
	else if (sceneName == "OVER") {

		newScene = new GameOverScene();
	}
	else if (sceneName == "DEBUG") {

		newScene = new DebugScene();
	}

	// 現在のシーン名を更新
	currentSceneName_ = sceneName;

	return newScene;
}

void SceneFactory::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("SceneFactory");
	
	// 現在のシーン名表示
	ImGui::Text("Current Scene: %s", currentSceneName_.c_str());

	// ボタンでシーン切り替え
	if (ImGui::Button("Title Scene")) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	if (ImGui::Button("Game Play Scene")) {
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}
	if (ImGui::Button("Game Clear Scene")) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
	if (ImGui::Button("Game Over Scene")) {
		SceneManager::GetInstance()->ChangeScene("OVER");
	}
	if (ImGui::Button("Debug Scene")) {
		SceneManager::GetInstance()->ChangeScene("DEBUG");
	}

	ImGui::End();

#endif // _DEBUG
}
