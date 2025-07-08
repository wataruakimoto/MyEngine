#include "GameScene.h"
#include "3D/Skybox/SkyboxCommon.h"

void GameScene::Initialize() {

	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("Resources/", "rostock_laage_airport_4k.dds");
	skybox->SetCamera(camera);
	skybox->SetScale({ 100.0f, 100.0f, 100.0f }); // スカイボックスの拡縮
}

void GameScene::Update() {

	// カメラの更新
	camera->Update();

	// Skyboxの更新
	skybox->ShowImGui("Skybox");
	skybox->Update();
}

void GameScene::Draw() {

	// スカイボックス共通部の描画設定
	SkyboxCommon::GetInstance()->SettingDrawing();

	// Skyboxの描画
	skybox->Draw();
}

void GameScene::Finalize() {
}