#include "GameScene.h"
#include "3D/Object3dCommon.h"
#include "3D/ModelManager.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	// レベルローダーの生成
	levelLoader = std::make_unique<Loader>();
	// レベルデータの読み込み
	levelLoader->LoadLevel("Level.json");
	// オブジェクトの配置
	levelLoader->PlaceObject();

	// モデルの生成
	model = std::make_unique<Model>();
	// モデル読み込み
	ModelManager::GetInstance()->LoadModelData("Resources/Terrain", "Terrain.obj");
	// モデル初期化
	model->Initialize("Resources/Terrain", "Terrain.obj");
	
	// オブジェクトの生成
	object = std::make_unique<Object3d>();
	// オブジェクト初期化
	object->Initialize();
	// オブジェクトのモデル設定
	object->SetModel(model.get());
}

void GameScene::Update() {

	/// === カメラ更新 === ///
	camera->ShowImGui("camera");
	camera->Update();

	// レベルローダー更新
	levelLoader->ShowImGui();
	levelLoader->Update();

	// オブジェクトの更新
	object->ShowImGui("terrain");
	object->Update();
}

void GameScene::Draw() {

	/// ===== ここから3D描画 ===== ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	// レベルローダー描画
	levelLoader->Draw();

	// オブジェクト描画
	object->Draw();
}

void GameScene::Finalize() {
}