#include "GameOverScene.h"
#include "WinApp.h"
#include "Object/Object3dRenderer.h"
#include "OffscreenRendering/FilterManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "OffscreenRendering/Filters/FogFilter.h"
#include "OffscreenRendering/Filters/RadialBlurFilter.h"
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

	// ラジアルブラーをフィルターマネージャから受け取っとく
	radialBlurFilter_ = filterManager_->GetRadialBlurFilter();
	radialBlurFilter_->SetIsActive(false); // ラジアルブラーは最初は無効化

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

	// プレイヤーの生成&初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetPlayerState(PlayerState::AutoPilot); // オートモードに設定
	player_->SetCamera(camera_.get());

	playerMoveSpeed_ = 0.0f;
	player_->SetMoveSpeedAuto(playerMoveSpeed_);

	// キャストし追従カメラの方を呼び出す
	dynamic_cast<FollowCameraController*>(cameraController_.get())->SetPlayer(player_.get());
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

	// 状態の更新
	if (stateRequest_) {

		// 状態変更
		gameOverFlowState_ = stateRequest_.value();

		// 状態の初期化
		switch (gameOverFlowState_) {

		case GameOverScene::GameOverFlowState::WaitInput:

			WaitInputInitialize();

			break;

		case GameOverScene::GameOverFlowState::SpeedUp:

			SpeedUpInitialize();

			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 各状態の更新
	switch (gameOverFlowState_) {

	case GameOverScene::GameOverFlowState::WaitInput:

		WaitInputUpdate();

		break;

	case GameOverScene::GameOverFlowState::SpeedUp:

		SpeedUpUpdate();

		break;
	}

	// プレイヤー更新
	player_->Update();
}

void GameOverScene::DrawFiltered() {

	// オブジェクトレンダラーの描画設定
	object3dRenderer_->SettingDrawingOpaque();

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();

	// プレイヤーの描画
	player_->Draw();
}

void GameOverScene::DrawUnfiltered() {
}

void GameOverScene::Finalize() {
}

void GameOverScene::ShowImGui() {

	cameraController_->ShowImGui();

	filterManager_->ShowImGui();
}

void GameOverScene::WaitInputInitialize() {
}

void GameOverScene::WaitInputUpdate() {

	// スペースキーでシーン切り替え
	if (input_->TriggerKey(VK_SPACE)) {

		// 状態を加速に変更
		stateRequest_ = GameOverFlowState::SpeedUp;
	}
}

void GameOverScene::SpeedUpInitialize() {

	// ラジアルブラーをつける
	radialBlurFilter_->SetIsActive(true);

	// ブラーの色はシアン
	radialBlurFilter_->SetGlowColor({ 0.3f, 0.7f, 1.0f });

	// ブラーの横幅をリセット
	blurStrength_ = 0.0f;
}

void GameOverScene::SpeedUpUpdate() {

	// ブラーの中心を計算
	blurCenter_.x = player_->GetScreenPos().x / WinApp::kClientWidth;
	blurCenter_.y = player_->GetScreenPos().y / WinApp::kClientHeight;

	// ブラーの中心を設定
	radialBlurFilter_->SetCenter(blurCenter_);

	// ブラーを徐々に強くする
	if (blurStrength_ < kMaxBlurStrength) {

		blurStrength_ += 0.0005f;
	}

	// プレイヤーの移動速度を徐々に上げる
	playerMoveSpeed_ += 0.02f;

	// プレイヤーの移動速度を設定
	player_->SetMoveSpeedAuto(playerMoveSpeed_);

	// ブラーの強さが最大値付近になったら
	if (blurStrength_ >= kMaxBlurStrength) {

		// シーンをゲームプレイシーンに切り替える
		sceneManager_->ChangeScene("PLAY");
	}
}
