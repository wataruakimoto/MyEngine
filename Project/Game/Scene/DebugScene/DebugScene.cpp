#include "DebugScene.h"
#include "SceneManager.h"
#include "Object/Object3dRenderer.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetRotate({ 0.39f,0.0f,0.0f });
	camera->GetWorldTransform().SetTranslate({ 0.0f,5.0f,-10.0f });

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
	model_->Initialize("Sphere", "sphere.obj");

	// オブジェクトの生成
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());

	// モデルの生成
	modelT_ = std::make_unique<Model>();
	modelT_->Initialize("Terrain", "terrain.obj");

	// オブジェクトの生成
	objectT_ = std::make_unique<Object3d>();
	objectT_->Initialize();
	objectT_->SetModel(modelT_.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// オブジェクトの更新
	object_->Update();

	objectT_->Update();
}

void DebugScene::DrawFiltered() {

	/// === 3Dオブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingOpaque();

	lightManager_->Draw();

	// オブジェクトの描画
	object_->Draw();

	objectT_->Draw();
}

void DebugScene::DrawUnfiltered() {
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("デバッグシーン");

	camera->ShowImGuiTree();

	if (ImGui::TreeNode("モンボ")) {

		object_->ShowImGui();

		model_->ShowImGui();

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("地面")) {

		objectT_->ShowImGui();

		modelT_->ShowImGui();

		ImGui::TreePop();
	}

	ImGui::End();

	lightManager_->ShowImGui();

#endif // USE_IMGUI
}
