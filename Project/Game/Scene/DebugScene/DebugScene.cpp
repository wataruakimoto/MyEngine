#include "DebugScene.h"
#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });

	// パーティクルシステムの初期化
	particleSystem->SetCamera(camera.get());
	particleSystem->CreateParticleGroup("Red", "Resources/Red.png", ParticleType::CUBE);
	particleSystem->CreateParticleGroup("Blue", "Resources/Blue.png", ParticleType::CUBE);

	// エミッター変換データの初期化
	emitterTransform.translate = { 0.0f,0.0f,0.0f };

	// パーティクルの初期化
	particleSetting.transform.scale = { 0.2f,0.2f,0.2f }; // スケール0.2
	particleSetting.lifeTime = 2.0f;                 // 2秒
	particleSetting.randomizeRotate = true;
	particleSetting.randomizeVelocity = true;

	// エミッターの生成
	particleEmitterRed = std::make_unique<ParticleEmitter>("Red", emitterTransform, 5, 0.0f, particleSetting);
	particleEmitterBlue = std::make_unique<ParticleEmitter>("Blue", emitterTransform, 25, 0.0f, particleSetting);
}

void DebugScene::Update() {

	// スペースキーを押したら
	if(input->TriggerKey(DIK_SPACE)) {

		// パーティクル発生
		particleEmitterRed->Emit();
		particleEmitterBlue->Emit();
	}

	// カメラの更新
	camera->Update();

	// パーティクルシステムの更新
	particleSystem->Update();
}

void DebugScene::Draw() {

	/// === パーティクルの描画前処理 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleSystem->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

	camera->ShowImGui("Camera");

	particleSystem->ShowImGui("ParticleSystem");

	particleEmitterRed->ShowImGui("ParticleEmitterRed");

	particleEmitterBlue->ShowImGui("ParticleEmitterBlue");
}
