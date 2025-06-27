#include "GameScene.h"
#include "3D/Object3dCommon.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	// レベルローダーの生成
	//levelLoader = std::make_unique<Loader>();
	// レベルデータの読み込み
	//levelLoader->LoadLevel("Level.json");
	// オブジェクトの配置
	//levelLoader->PlaceObject();
}

void GameScene::Update() {

	/// === カメラ更新 === ///
	camera->ShowImGui("camera");
	camera->Update();

	// レベルローダー更新
	//levelLoader->Update();
}

void GameScene::Draw() {

	/// ===== ここから3D描画 ===== ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	// レベルローダー描画
	//levelLoader->Draw();
}

void GameScene::Finalize() {
}