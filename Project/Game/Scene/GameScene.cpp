#include "GameScene.h"
#include "3D/ModelManager.h"
#include "3D/Skybox/SkyboxCommon.h"
#include "3D/Object3dCommon.h"

void GameScene::Initialize() {

	// カメラの位置
	camera->SetTranslate({ 0.0f, 0.0f, -20.0f });

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModelData("Resources/Sphere", "sphere.obj");

	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("Resources/", "rostock_laage_airport_4k.dds");
	skybox->SetCamera(camera);
	skybox->SetScale({ 100.0f, 100.0f, 100.0f }); // スカイボックスの拡縮

	// モデルの初期化
	model = std::make_unique<Model>();
	model->Initialize("Resources/Sphere", "sphere.obj");
	model->SetEnvironmentMapFilePath(skybox->GetFilePath());

	// オブジェクトの初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
}

void GameScene::Update() {

	// カメラの更新
	camera->Update();

	// Skyboxの更新
	skybox->ShowImGui("Skybox");
	skybox->Update();

	// オブジェクトの更新
	object->ShowImGui("Object");
	object->Update();
}

void GameScene::Draw() {

	// スカイボックス共通部の描画設定
	SkyboxCommon::GetInstance()->SettingDrawing();

	// Skyboxの描画
	skybox->Draw();

	// オブジェクト共通部の描画設定
	Object3dCommon::GetInstance()->SettingDrawing();

	// オブジェクトの描画
	object->Draw();
}

void GameScene::Finalize() {
}