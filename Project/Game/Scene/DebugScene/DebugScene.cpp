#include "DebugScene.h"
#include "SceneManager.h"
#include "Particle/ParticleRenderer.h"
#include "Particle/ParticleManager.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	worldTransform.Initialize();

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetParent(&worldTransform);
	camera->GetWorldTransform().SetRotate({ 0.2f,0.0f,0.0f });
	camera->GetWorldTransform().SetTranslate({ 0.0f,2.5f,-8.0f });

	// シーンマネージャのインスタンス取得
	sceneManager = SceneManager::GetInstance();

	// パーティクルレンダラーのインスタンス取得
	particleRenderer = ParticleRenderer::GetInstance();

	// パーティクルマネージャのインスタンス取得
	particleManager = ParticleManager::GetInstance();
	// パーティクルマネージャにカメラをセット
	particleManager->SetCamera(camera.get());

	// パーティクルエミッターの生成
	emitter_ = std::make_unique<ParticleEmitter>("PlayerMove", EmitterType::Interval, 1);
	// パーティクルエミッターの初期化
	emitter_->Initialize();
	// パーティクルエミッターに位置をセット
	emitter_->GetWorldTransform().SetParent(&worldTransform);
	// 発生頻度をセット
	emitter_->SetFrequency(0.1f);
}

void DebugScene::Update() {

	// ワールド変換に位置を加算
	//worldTransform.AddTranslate({ 0.0f, 0.0f, 0.1f });

	// ワールド変換の更新
	worldTransform.Update();

	// カメラの更新
	camera->Update();

	// パーティクルマネージャの更新
	particleManager->Update();

	// パーティクルエミッターの更新
	emitter_->Update();
}

void DebugScene::DrawFiltered() {

	/// === パーティクル描画 === ///
	particleRenderer->SettingDrawing();

	// パーティクルマネージャの描画
	particleManager->Draw();
}

void DebugScene::DrawUnfiltered() {
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("デバッグシーン");

	camera->ShowImGuiTree();

	worldTransform.ShowImGui();

	ImGui::End();

	emitter_->ShowImGui();

#endif // USE_IMGUI
}
