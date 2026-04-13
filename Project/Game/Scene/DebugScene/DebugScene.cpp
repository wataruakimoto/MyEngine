#include "DebugScene.h"
#include "LineManager.h"
#include "SceneManager.h"
#include "Object/Object3dRenderer.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetTranslate({ 0.0f,0.0f,-10.0f });

	// 線描画マネージャのインスタンス取得
	lineManager = LineManager::GetInstance();
	// 線描画マネージャにデフォルトカメラをセット
	lineManager->SetDefaultCamera(camera.get());

	// シーンマネージャのインスタンス取得
	sceneManager = SceneManager::GetInstance();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

	// インスタンス取得
	object3dRenderer_ = Object3dRenderer::GetInstance();
	object3dRenderer_->SetDefaultCamera(camera.get());

	// モデルの生成
	model_ = std::make_unique<Model>();
	model_->Initialize("Sphere", "Sphere.obj");

	// オブジェクトの生成
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// オブジェクトの更新
	object_->Update();
}

void DebugScene::DrawFiltered() {

	/// === 3Dオブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingOpaque();

	lightManager_->Draw();

	// オブジェクトの描画
	object_->Draw();
}

void DebugScene::DrawUnfiltered() {

	lineManager->DrawGrid({ 0.0f, 0.0f, 0.0f }, 15.0f, 15, { 1.0f, 1.0f, 1.0f, 1.0f });
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("デバッグシーン");

	camera->ShowImGuiTree();

	object_->ShowImGui();

	model_->ShowImGui();

	ImGui::End();

	lightManager_->ShowImGui();

#endif // USE_IMGUI
}
