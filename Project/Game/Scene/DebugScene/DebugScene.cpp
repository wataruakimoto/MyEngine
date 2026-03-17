#include "DebugScene.h"
#include "LineManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "TransitionManager.h"
#include "SceneManager.h"
#include "Input.h"
#include "Transition/FadeTransition.h"
#include "Transition/SlideTransition.h"

#include <imgui.h>

using namespace Engine;

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-20.0f });

	// 線描画マネージャのインスタンス取得
	lineManager = LineManager::GetInstance();
	// 線描画マネージャにデフォルトカメラをセット
	lineManager->SetDefaultCamera(camera.get());

	// フィルターマネージャの初期化
	filterManager = FilterManager::GetInstance();
	filterManager->SetCamera(camera.get());

	// 遷移マネージャの初期化
	transitionManager = TransitionManager::GetInstance();

	// シーンマネージャのインスタンス取得
	sceneManager = SceneManager::GetInstance();

	// 入力の初期化
	input = Input::GetInstance();
}

void DebugScene::Update() {

	// スペースキーがトリガーされたら
	if (input->TriggerKey(VK_SPACE)) {

		// フェードアウト開始
		transitionManager->StartOutTransition(
			std::make_unique<SlideTransition>( Vector2{ 1280.0f, 0.0f }),
			//std::make_unique<FadeTransition>(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }),
			[]() { SceneManager::GetInstance()->ChangeScene("TITLE"); },
			2.0f // 遷移にかける時間 (秒)
			);
	}

	// カメラの更新
	camera->Update();
}

void DebugScene::DrawFiltered() {
}

void DebugScene::DrawUnfiltered() {

	lineManager->DrawSphere({ 0.0f, 0.0f, 0.0f }, 2.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, 16);
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

	// カメラのImGui表示
	camera->ShowImGui("Camera");

	// フィルターマネージャのImGui表示
	filterManager->ShowImGui();
}
