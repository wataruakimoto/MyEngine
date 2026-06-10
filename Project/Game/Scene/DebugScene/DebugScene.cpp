#include "DebugScene.h"
#include "SceneManager.h"
#include "Particle/ParticleRenderer.h"
#include "Particle/ParticleManager.h"
#include "Input.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetRotate(Vector3{ 3.14f/12.0f,0.0f,0.0f });
	camera->GetWorldTransform().SetTranslate({ 0.0f,5.0f,-15.0f });

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

	ParticleSetting setting{};
	setting.effectName = "Cylinder";
	setting.textureFileName = "gradationLine.png";
	setting.textureFullPath = "Resources/Textures/Particles/gradationLine.png";
	setting.shape = ParticleShape::CYLINDER;
	setting.useBillboard = false;
	setting.isInfinite = true;
	setting.scale = { 1.0f,0.5f,1.0f };
	setting.rotate = { 0.0f,3.14f/4.0f,0.0f };
	setting.color = { 0.0f,1.0f,1.0f,0.75f };

	particleManager->AddSetting(setting);

	emitter = std::make_unique<ParticleEmitter>("Cylinder", EmitterType::OneShot, 1);
	emitter->Initialize();

	emitterB = std::make_unique<ParticleEmitter>("Cylinder", EmitterType::OneShot, 1);
	emitterB->Initialize();
	emitterB->SetTranslate({ 5.0f,0.0f,0.0f });
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// パーティクルマネージャーの更新
	particleManager->Update();

	emitter->Update();
	emitterB->Update();

	if (Input::GetInstance()->TriggerKey(VK_SPACE)) {
		emitter->Emit();
	}

	if (Input::GetInstance()->TriggerKey(VK_RETURN)) {
		emitterB->Emit();
	}

	if (Input::GetInstance()->TriggerKey('C')) {
		particleManager->ClearInstance(emitter.get());
	}
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
