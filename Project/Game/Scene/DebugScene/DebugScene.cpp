#include "DebugScene.h"
#include "SceneManager.h"
#include "Particle/ParticleRenderer.h"
#include "Particle/ParticleManager.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetTranslate({ 0.0f,0.0f,-10.0f });

	// シーンマネージャのインスタンス取得
	sceneManager = SceneManager::GetInstance();

	// パーティクルレンダラーのインスタンス取得
	particleRenderer = ParticleRenderer::GetInstance();
	// デフォルトカメラのセッターにカメラをセット
	particleRenderer->SetDefaultCamera(camera.get());
	
	// パーティクルマネージャーのインスタンス取得
	particleManager = ParticleManager::GetInstance();
	// カメラのセッターにカメラをセット
	particleManager->SetCamera(camera.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// パーティクルマネージャーの更新
	particleManager->Update();
}

void DebugScene::DrawFiltered() {

	/// === パーティクルの描画 === ///
	particleRenderer->SettingDrawing();

	// パーティクルマネージャーの描画
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

	ImGui::End();

#endif // USE_IMGUI
}
