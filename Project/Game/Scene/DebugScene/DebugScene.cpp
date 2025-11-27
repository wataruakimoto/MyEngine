#include "DebugScene.h"
#include "Input.h"

#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-20.0f });

	// パーティクルシステムの初期化
	particleSystem->SetCamera(camera.get());
	particleSystem->CreateParticleGroup("DebugParticle", "Resources/Blue.png", ParticleShape::SHARD);

	// パーティクルの設定
	particleSetting.lifeTime = 0.5f;
	particleSetting.useBillboard = false;
	particleSetting.randomizeScale = true;
	particleSetting.randomScaleMin = { 0.2f, 0.2f, 0.2f };
	particleSetting.randomScaleMax = { 0.3f, 0.3f, 0.3f };
	particleSetting.randomizeRotate = true;
	particleSetting.randomRotateMin = { 0.0f, 0.0f, 0.0f };
	particleSetting.randomRotateMax = { 1.0f, 1.0f, 1.0f };
	particleSetting.randomizeVelocity = true;
	particleSetting.randomVelocityMin = { 2.0f, 0.0f, 0.0f };
	particleSetting.randomVelocityMax = { 4.0f, 0.0f, 0.0f };

	// エミッターの生成
	particleEmitter = std::make_unique<ParticleEmitter>("DebugParticle", emitterTransform, 20, 0.0f, particleSetting);
	particleEmitter->SetUseExplosion(true);
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// スペースキーが押されたらパーティクルを発生させる
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		particleEmitter->Emit();
	}

	// パーティクルシステムの更新
	particleSystem->Update();
}

void DebugScene::Draw() {

	/// === パーティクルの描画準備 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleSystem->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

	// カメラのImGui表示
	camera->ShowImGui("Camera");

	// パーティクルシステムのImGui表示
	particleSystem->ShowImGui("ParticleSystem");

	// エミッターのImGui表示
	particleEmitter->ShowImGui("ParticleEmitter");
}
