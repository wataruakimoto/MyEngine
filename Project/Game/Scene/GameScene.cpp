#include "GameScene.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "3d/Object3dCommon.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleSystem.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include <numbers>

void GameScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->SetRotate({ 0.2f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	AudioManager::GetInstance()->SoundLoadWave("Resources/fanfare.wav");

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
	ParticleSystem::GetInstance()->CreateParticleGroup("circle2", "Resources/circle2.png", ParticleType::PLANE);
	ParticleSystem::GetInstance()->CreateParticleGroup("ring", "Resources/gradationLine.png", ParticleType::RING);
	ParticleSystem::GetInstance()->CreateParticleGroup("cylinder", "Resources/gradationLine.png", ParticleType::CYLINDER);

	// Transformの設定
	EmitterTransform1.translate = { 0.0f,2.0f,0.0f };
	EmitterTransform2.translate = { -2.5f,1.0f,0.0f };
	EmitterTransform3.translate = { 2.5f,0.0f,0.0f };

	// パーティクルの設定
	particleSetting1.randomizeScale = true;
	particleSetting1.randomScaleMin = { 0.05f,0.4f,1.0f };
	particleSetting1.randomScaleMax = { 0.05f,1.5f,1.0f };
	particleSetting1.randomizeRotate = true;
	particleSetting1.randomRotateMin = { 0.0f,0.0f,-std::numbers::pi_v<float> };
	particleSetting1.randomRotateMax = { 0.0f,0.0f,std::numbers::pi_v<float> };

	particleSetting2.useLifeTime = false;
	particleSetting2.useBillboard = true;

	particleSetting3.color = { 0.0f,0.0f,1.0f,1.0f };
	particleSetting3.useLifeTime = false;
	particleSetting3.useBillboard = false;

	// エミッタ生成
	particleEmitter1 = std::make_unique <ParticleEmitter>("circle2", EmitterTransform1, 3, 0.5f, particleSetting1);
	particleEmitter2 = std::make_unique <ParticleEmitter>("ring", EmitterTransform2, 1, 1.0f, particleSetting2);
	particleEmitter3 = std::make_unique <ParticleEmitter>("cylinder", EmitterTransform3, 1, 1.0f, particleSetting3);

	// パーティクルシステムの初期化
	ParticleSystem::GetInstance()->CreateParticleGroup("explosion", "Resources/gradationLine.png", ParticleType::EXPLOSION);
	ParticleSystem::GetInstance()->CreateParticleGroup("flash", "Resources/gradation.png", ParticleType::FLASH);
	// 座標データの設定
	explosionEmitterTransform.translate = { 0.0f, 0.0f, 0.0f };
	flashEmitterTransform.translate = { 0.0f, 0.0f, 0.0f };
	// パーティクルの設定
	explosionParticleSetting.useBillboard = true;
	flashParticleSetting.useBillboard = true;
	// パーティクルエミッタの生成
	explosionEmitter = std::make_unique <ParticleEmitter>("explosion", explosionEmitterTransform, 1, 0.1f, explosionParticleSetting);
	flashEmitter = std::make_unique <ParticleEmitter>("flash", flashEmitterTransform, 1, 0.1f, flashParticleSetting);
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

	particleEmitter1->ShowImGui("circle2");
	particleEmitter1->Emit();

	particleEmitter2->ShowImGui("ring");

	particleEmitter3->ShowImGui("cylinder");

	explosionEmitter->ShowImGui("explosion");

	flashEmitter->ShowImGui("flash");

	ParticleSystem::GetInstance()->ShowImGui("particleSystem");

	// パーティクルシステムの更新
	ParticleSystem::GetInstance()->Update();
}

void GameScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画
	monsterBall->Draw();

	terrain->Draw();

	/// === パーティクルの描画準備 === ///
	ParticleCommon::GetInstance()->SettingDrawing();

	// パーティクルシステムの描画
	ParticleSystem::GetInstance()->Draw();
}

void GameScene::Finalize() {
}