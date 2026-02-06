#include "GameClearScene.h"

using namespace Engine;

void GameClearScene::Initialize() {

	// スプライトの生成&初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("GameClear.png");
}

void GameClearScene::Update() {

	// スプライトの更新
	sprite_->Update();

	// スペースキーでシーン切り替え
	if(input_->TriggerKey(VK_SPACE)) {
		sceneManager_->ChangeScene("TITLE");
	}
}

void GameClearScene::DrawFiltered() {
}

void GameClearScene::DrawUnfiltered() {

	// スプライトコモンの描画準備
	spriteCommon_->SettingDrawing();

	// スプライトの描画
	sprite_->Draw();
}

void GameClearScene::Finalize() {
}

void GameClearScene::ShowImGui() {
}
