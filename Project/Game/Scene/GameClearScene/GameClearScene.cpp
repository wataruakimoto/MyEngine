#include "GameClearScene.h"

void GameClearScene::Initialize() {

	// テクスチャ読み込み
	textureManager_->LoadTexture("Resources/GameClear.png");

	// スプライトの生成&初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("Resources/GameClear.png");
}

void GameClearScene::Update() {

	// スプライトの更新
	sprite_->Update();

	// スペースキーでシーン切り替え
	if(input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene("TITLE");
	}
}

void GameClearScene::Draw() {

	// スプライトコモンの描画準備
	spriteCommon_->SettingDrawing();

	// スプライトの描画
	sprite_->Draw();
}

void GameClearScene::Finalize() {
}

void GameClearScene::ShowImGui() {
}
