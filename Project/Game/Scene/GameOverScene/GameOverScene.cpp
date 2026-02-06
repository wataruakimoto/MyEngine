#include "GameOverScene.h"

using namespace Engine;

void GameOverScene::Initialize() {

	// スプライトの生成&初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("GameOver.png");
}

void GameOverScene::Update() {

	// スプライトの更新
	sprite_->Update();

	// スペースキーでシーン切り替え
	if (input_->TriggerKey(VK_SPACE)) {
		sceneManager_->ChangeScene("TITLE");
	}
}

void GameOverScene::DrawFiltered() {
}

void GameOverScene::DrawUnfiltered() {

	// スプライトコモンの描画準備
	spriteCommon_->SettingDrawing();

	// スプライトの描画
	sprite_->Draw();
}

void GameOverScene::Finalize() {
}

void GameOverScene::ShowImGui() {
}
