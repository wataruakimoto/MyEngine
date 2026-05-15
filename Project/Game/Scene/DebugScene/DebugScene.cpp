#include "DebugScene.h"
#include "SceneManager.h"
#include "Object/Object3dRenderer.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	worldTransform.Initialize();

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->GetWorldTransform().SetRotate(Vector3{ 0.2f,0.0f,0.0f });
	camera->GetWorldTransform().SetTranslate({ 0.0f,2.5f,-8.0f });

	// シーンマネージャのインスタンス取得
	sceneManager = SceneManager::GetInstance();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

	// オブジェクトレンダラーのインスタンス取得
	object3dRenderer = Object3dRenderer::GetInstance();
	// オブジェクトレンダラーにカメラをセット
	object3dRenderer->SetDefaultCamera(camera.get());

	// モデルの生成
	model = std::make_unique<Model>();
	// モデルの初期化
	model->Initialize("Player/player.obj");

	// オブジェクトの生成
	object = std::make_unique<Object3d>();
	// オブジェクトの初期化
	object->Initialize();
	// オブジェクトにワールド変換をセット
	object->GetWorldTransform().SetParent(&worldTransform);
	// オブジェクトにモデルをセット
	object->SetModel(model.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// ワールド変換の更新
	worldTransform.Update();

	// オブジェクトの更新
	object->Update();
}

void DebugScene::DrawFiltered() {

	/// === オブジェクトの描画 === ///
	object3dRenderer->SettingDrawingOpaque();

	// ライトの描画
	lightManager_->Draw();

	// オブジェクトの描画
	object->Draw();
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

	lightManager_->ShowImGui();

	object->ShowImGui();

	model->ShowImGui();

	ImGui::End();

#endif // USE_IMGUI
}
