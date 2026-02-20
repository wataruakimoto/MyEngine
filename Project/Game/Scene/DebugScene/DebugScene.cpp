#include "DebugScene.h"
#include "Input.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-20.0f });

	// 線描画レンダラーの初期化
	lineRenderer = LineRenderer::GetInstance();
	lineRenderer->Initialize();

	// 線描画マネージャの初期化
	lineManager = LineManager::GetInstance();
	lineManager->Initialize();
	lineManager->SetDefaultCamera(camera.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// 線描画マネージャのクリア
	lineManager->Clear();
}

void DebugScene::DrawFiltered() {
}

void DebugScene::DrawUnfiltered() {

	// 線描画の設定
	lineRenderer->SettingDrawing();

	lineManager->DrawSphere({ 0.0f, 0.0f, 0.0f }, 2.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, 16);

	// 線描画
	lineManager->Render();
}

void DebugScene::Finalize() {

	// 線描画マネージャの終了
	lineManager->Finalize();
	// 線描画レンダラーの終了
	lineRenderer->Finalize();
}

void DebugScene::ShowImGui() {

	// カメラのImGui表示
	camera->ShowImGui("Camera");
}
