#include "GameOverScene.h"
#include "Object/Object3dRenderer.h"
#include "OffscreenRendering/FilterManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "OffscreenRendering/Filters/FogFilter.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"

using namespace Engine;

void GameOverScene::Initialize() {

	// インスタンスの取得
	object3dRenderer_ = Object3dRenderer::GetInstance();
	filterManager_ = FilterManager::GetInstance();
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();

	// フォグをフィルターマネージャから受け取っとく
	fogFilter_ = filterManager_->GetFogFilter();
	fogFilter_->SetStartDistance(500.0f); // フォグ開始距離を500に設定
	fogFilter_->SetIsActive(true);    // フォグを有効化

	// カメラの生成&初期化
	camera_ = std::make_unique<Engine::Camera>();
	camera_->Initialize();
	camera_->SetFarClip(950.0f); // ファークリップを950に設定

	// カメラの設定
	object3dRenderer_->SetDefaultCamera(camera_.get());
	filterManager_->SetCamera(camera_.get());

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->Initialize();
	cameraController_->SetCamera(camera_.get());

	// フロアを生成
	floor_ = std::make_unique<Floor>();
	floor_->Initialize();

	// シリンダーの生成
	cylinder_ = std::make_unique<Cylinder>();
	cylinder_->Initialize();
}

void GameOverScene::Update() {

	// カメラコントローラの更新
	cameraController_->Update();

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(camera_->GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(camera_->GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

	// スペースキーでシーン切り替え
	if (input_->TriggerKey(VK_SPACE)) {
		sceneManager_->ChangeScene("TITLE");
	}
}

void GameOverScene::DrawFiltered() {

	// オブジェクトレンダラーの描画設定
	object3dRenderer_->SettingDrawingOpaque();

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();
}

void GameOverScene::DrawUnfiltered() {
}

void GameOverScene::Finalize() {
}

void GameOverScene::ShowImGui() {

	cameraController_->ShowImGui();

	filterManager_->ShowImGui();
}
