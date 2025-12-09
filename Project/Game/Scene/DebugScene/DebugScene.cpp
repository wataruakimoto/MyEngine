#include "DebugScene.h"
#include "Input.h"

#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-20.0f });

	// パーティクルマネージャー初期化
	particleManager->Initialize();
	// カメラのセット
	particleManager->SetCamera(camera.get());

	// エミッターの生成
	particleEmitterRed = std::make_unique<ParticleEmitter>("BulletRed", 1.0f, 5);
	particleEmitterBlue = std::make_unique<ParticleEmitter>("PlayerDeathBlue", 1.0f, 10);

	// エミッターの初期化
	particleEmitterRed->Initialize();
	particleEmitterBlue->Initialize();
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// エミッターの更新
	particleEmitterRed->Update();
	particleEmitterBlue->Update();

	// パーティクルシステムの更新
	particleManager->Update();
}

void DebugScene::Draw() {

	/// === パーティクルの描画準備 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleManager->Draw();
}

void DebugScene::Finalize() {

	// パーティクルマネージャーの終了
	particleManager->Finalize();
}

void DebugScene::ShowImGui() {

	// カメラのImGui表示
	camera->ShowImGui("Camera");

	// パーティクルマネージャーのImGui表示
	particleManager->ShowImGui();
}
