#include "GameScene.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "3d/Object3dCommon.h"
#include "3d/ParticleManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera = new Camera();
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	AudioManager::GetInstance()->SoundLoadWave("resources/fanfare.wav");

	// モデルの生成・初期化
	modelMonsterBall = new Model();
	modelMonsterBall->Initialize("resources/sphere", "sphere.obj");

	// 3Dオブジェクトの生成・初期化
	monsterBall = new Object3d();
	monsterBall->Initialize();
	monsterBall->SetModel(modelMonsterBall);
	monsterBall->SetCamera(camera);

	modelTerrain = new Model();
	modelTerrain->Initialize("resources/terrain", "terrain.obj");

	terrain = new Object3d();
	terrain->Initialize();
	terrain->SetModel(modelTerrain);
	terrain->SetCamera(camera);

	modelPlane = new Model();
	modelPlane->Initialize("resources/plane", "plane.obj");

	// パーティクルの生成・初期化
	particle = new Particle();
	particle->Initialize();
	particle->SetModel(modelPlane);
	particle->SetCamera(camera);
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

	particle->ShowImGui("particle");
	particle->Update();
}

void GameScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画
	monsterBall->Draw();

	terrain->Draw();

	/// === パーティクルの描画準備 === ///
	ParticleManager::GetInstance()->SettingCommonDrawing();

	// パーティクルの描画
	particle->Draw();
}

void GameScene::Finalize() {

	// 3Dオブジェクトの解放
	delete monsterBall;

	delete terrain;

	delete particle;

	// モデルの解放
	delete modelMonsterBall;

	delete modelTerrain;

	delete modelPlane;
}