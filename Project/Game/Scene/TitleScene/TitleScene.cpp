#include "TitleScene.h"

void TitleScene::Initialize() {
}

void TitleScene::Update() {

	// スペースキーを押したらゲームシーンへ
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene("PLAY");
	};
}

void TitleScene::Draw() {
}

void TitleScene::Finalize() {
}

void TitleScene::ShowImGui() {
}
