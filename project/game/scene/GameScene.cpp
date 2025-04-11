#include "GameScene.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "3d/Object3dCommon.h"
#include "3d/ParticleCommon.h"
#include "3d/ParticleSystem.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

void GameScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	AudioManager::GetInstance()->SoundLoadWave("Resources/fanfare.wav");

	TextureManager::GetInstance()->LoadTexture("Resources/circle.png");

	TextureManager::GetInstance()->LoadTexture("Resources/circle2.png");

	// モデルの生成・初期化
	modelMonsterBall = std::make_unique <Model>();
	modelMonsterBall->Initialize("Resources/sphere", "sphere.gltf");

	// 3Dオブジェクトの生成・初期化
	monsterBall = std::make_unique <Object3d>();
	monsterBall->Initialize();
	monsterBall->SetModel(modelMonsterBall.get());
	monsterBall->SetCamera(camera.get());

	modelTerrain = std::make_unique <Model>();
	modelTerrain->Initialize("Resources/terrain", "terrain.gltf");

	terrain = std::make_unique <Object3d>();
	terrain->Initialize();
	terrain->SetModel(modelTerrain.get());
	terrain->SetCamera(camera.get());

	// パーティクルシステムの初期化
	ParticleSystem::GetInstance()->SetCamera(camera.get());
	ParticleSystem::GetInstance()->CreateParticleGroup("circle", "Resources/circle.png");
	ParticleSystem::GetInstance()->CreateParticleGroup("circle2", "Resources/circle2.png");

	// Transformの設定
	EmitterTransform1.translate = { -2.0f,2.5f,0.0f };
	EmitterTransform2.translate = { 2.0f,2.5f,0.0f };

	// エミッタ生成
	particleEmitter1 = std::make_unique <ParticleEmitter>("circle", EmitterTransform1, 5, 0.5f);
	particleEmitter2 = std::make_unique <ParticleEmitter>("circle2", EmitterTransform2, 5, 0.5f);
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

	particleEmitter1->ShowImGui("particleEmitter");
	particleEmitter1->Emit();

	particleEmitter2->ShowImGui("particleEmitter2");
	particleEmitter2->Emit();

	ParticleSystem::GetInstance()->ShowImGui("particleSystem");
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