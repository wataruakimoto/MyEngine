#include "GameScene.h"
#include "3D/Skybox/Skybox.h"
#include "3D/Skybox/SkyboxCommon.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	// Skyboxの初期化
	skyBox = std::make_unique<SkyBox>();
	skyBox->Initialize("Resources/", "");
}

void GameScene::Update() {

	/// === カメラ更新 === ///
	camera->ShowImGui("camera");
	camera->Update();

	// Skyboxの更新
	skyBox->Update();
}

void GameScene::Draw() {

	// スカイボックス共通部の描画設定
	SkyboxCommon::GetInstance()->SettingDrawing();

	// Skyboxの描画
	skyBox->Draw();
}

void GameScene::Finalize() {
}