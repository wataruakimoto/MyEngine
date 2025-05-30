#include "GameScene.h"
#include "input/Input.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleSystem.h"

#include <numbers>

void GameScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->SetRotate({ 0.0f, 0.0f, 0.0f });
	camera->SetTranslate({ 0.0f, 0.0f, -20.0f });

	// パーティクルシステムの初期化
	ParticleSystem::GetInstance()->SetCamera(camera.get());

	// パーティクルグループの生成
	ParticleSystem::GetInstance()->CreateParticleGroup("explosion", "Resources/gradationLine.png", ParticleType::EXPLOSION);

	ParticleSystem::GetInstance()->CreateParticleGroup("flash", "Resources/white.png", ParticleType::FLASH2);

	ParticleSystem::GetInstance()->CreateParticleGroup("smoke", "Resources/circle.png", ParticleType::PLANE);

	// 座標データの設定
	explosionEmitterTransform.translate = { 0.0f, 0.0f, 0.0f };

	flashEmitterTransform.translate = { 0.0f, 0.0f, 0.0f };

	smokeEmitterTransform.translate = { 0.0f, 0.0f, 0.0f };

	// パーティクルの設定
	explosionParticleSetting.transform.scale = { 2.0f, 2.0f, 1.0f };
	explosionParticleSetting.lifeTime = 0.5f;

	flashParticleSetting.randomizeScale = true;
	flashParticleSetting.randomScaleMin = { 0.2f,0.5f,1.0f };
	flashParticleSetting.randomScaleMax = { 0.2f,4.0f,1.0f };
	flashParticleSetting.randomizeRotate = true;
	flashParticleSetting.randomRotateMin = { 0.0f,0.0f,-std::numbers::pi_v<float> };
	flashParticleSetting.randomRotateMax = { 0.0f,0.0f,std::numbers::pi_v<float> };

	smokeParticleSetting.lifeTime = 1.5f;
	smokeParticleSetting.color = { 0.5f, 0.5f, 0.5f, 1.0f };
	smokeParticleSetting.transform.scale = { 4.0f, 4.0f, 0.0f };
	smokeParticleSetting.randomizeTranslate = true;
	smokeParticleSetting.randomTranslateMin = { -1.0f, -1.0f, 0.0f };
	smokeParticleSetting.randomTranslateMax = { 1.0f, 1.0f, 0.0f };
	smokeParticleSetting.randomizeVelocity = true;
	smokeParticleSetting.randomVelocityMin = { -0.5f, -0.5f, 0.0f };
	smokeParticleSetting.randomVelocityMax = { 0.5f, 0.5f, 0.0f };

	// パーティクルエミッタの生成
	explosionEmitter = std::make_unique <ParticleEmitter>("explosion", explosionEmitterTransform, 1, 2.0f, explosionParticleSetting);

	flashEmitter = std::make_unique <ParticleEmitter>("flash", flashEmitterTransform, 4, 2.0f, flashParticleSetting);

	smokeEmitter = std::make_unique <ParticleEmitter>("smoke", smokeEmitterTransform, 10, 2.0f, smokeParticleSetting);
}

void GameScene::Update() {

	// パーティクルを発生させる
	explosionEmitter->Emit();
	flashEmitter->Emit();
	smokeEmitter->Emit();

	/// === カメラ更新 === ///
	camera->ShowImGui("camera");
	camera->Update();

	explosionEmitter->ShowImGui("explosion");

	flashEmitter->ShowImGui("flash");

	smokeEmitter->ShowImGui("smoke");

	ParticleSystem::GetInstance()->ShowImGui("particleSystem");

	// パーティクルシステムの更新
	ParticleSystem::GetInstance()->Update();
}

void GameScene::Draw() {

	/// === パーティクルの描画準備 === ///
	ParticleCommon::GetInstance()->SettingDrawing();

	// パーティクルシステムの描画
	ParticleSystem::GetInstance()->Draw();
}

void GameScene::Finalize() {
}