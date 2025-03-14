#include "GameScene.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "3d/Object3dCommon.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	AudioManager::GetInstance()->SoundLoadWave("resources/fanfare.wav");

	// モデルの生成・初期化
	modelMonsterBall = std::make_unique <Model>();
	modelMonsterBall->Initialize("resources/sphere", "sphere.obj");

	// 3Dオブジェクトの生成・初期化
	monsterBall = std::make_unique <Object3d>();
	monsterBall->Initialize();
	monsterBall->SetModel(modelMonsterBall.get());
	monsterBall->SetCamera(camera.get());

	modelTerrain = std::make_unique <Model>();
	modelTerrain->Initialize("resources/terrain", "terrain.obj");

	terrain = std::make_unique <Object3d>();
	terrain->Initialize();
	terrain->SetModel(modelTerrain.get());
	terrain->SetCamera(camera.get());
}

void GameScene::Update() {

	// サウンド更新
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	
		AudioManager::GetInstance()->SoundPlayWave();
	}

	/// === カメラ更新 === ///
	camera->ShowImGui("camera");
	camera->Update();

	/// === 3Dオブジェクト更新 === ///

	monsterBall->ShowImGui("monsterBall");
	monsterBall->Update();

	terrain->ShowImGui("terrain");
	terrain->Update();
}

void GameScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画
	monsterBall->Draw();

	terrain->Draw();
}

void GameScene::Finalize() {
}