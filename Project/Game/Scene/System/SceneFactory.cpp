#include "SceneFactory.h"
#include "TitleScene.h"
#include "GamePlayScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include "DebugScene.h"

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

	return newScene;
}