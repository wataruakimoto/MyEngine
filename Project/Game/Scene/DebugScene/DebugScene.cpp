#include "DebugScene.h"
#include "Input.h"
#include "Transition/FadeTransition.h"

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

	// フィルターマネージャの初期化
	filterManager = FilterManager::GetInstance();
	filterManager->SetCamera(camera.get());

	// 遷移マネージャの初期化
	transitionManager = TransitionManager::GetInstance();

	// 入力の初期化
	input = Input::GetInstance();
}

void DebugScene::Update() {

	// スペースキーがトリガーされたら
	if (input->TriggerKey(VK_SPACE)) {

		// フェードアウト開始
		transitionManager->StartTransition(
			std::make_unique<FadeTransition>(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }),
			[]() {
				// 遷移完了後の処理 (ここでは何もしない)
			},
			2.0f // 遷移にかける時間 (秒)
			);
	}

	// カメラの更新
	camera->Update();

	// 線描画マネージャのクリア
	lineManager->Clear();

	// 遷移マネージャの更新
	transitionManager->Update();
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

	// フィルターマネージャのImGui表示
	filterManager->ShowImGui();
}
