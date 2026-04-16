#include "GameClearScene.h"
#include "Object/Object3dRenderer.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "OffscreenRendering/FilterManager.h"
#include "OffscreenRendering/Filters/FogFilter.h"
#include "TransitionManager.h"
#include "Transition/FadeTransition.h"
#include "SceneManager.h"
#include "input.h"
#include "Sprite/SpriteRenderer.h"

using namespace Engine;

void GameClearScene::Initialize() {

	// インスタンスの取得
	object3dRenderer_ = Object3dRenderer::GetInstance();
	filterManager_ = FilterManager::GetInstance();
	transitionManager = TransitionManager::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	input_ = Input::GetInstance();
	spriteRenderer_ = SpriteRenderer::GetInstance();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

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
	cameraController_->SetCamera(camera_.get());
	cameraController_->Initialize();

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

	playerMoveSpeed_ = 1.0f;
	player_->SetMoveSpeedAuto(playerMoveSpeed_);

	// キャストし追従カメラの方を呼び出す
	dynamic_cast<FollowCameraController*>(cameraController_.get())->SetPlayer(player_.get());

	// テキストスプライトの生成&初期化
	text_ = std::make_unique<Engine::Sprite>();
	text_->Initialize("GameClear.png");

	// ガイドスプライトの生成&初期化
	guide_ = std::make_unique<Engine::Sprite>();
	guide_->Initialize("SpaceToTitle.png");
	guide_->SetPosition({ 640.0f, 600.0f });
	guide_->SetAnchorPoint({ 0.5f, 0.5f });

	// 最初の状態をリクエスト
	stateRequest_ = GameClearFlowState::FadeOut;
}

void GameClearScene::Update() {

	// カメラコントローラの更新
	cameraController_->Update();

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(camera_->GetWorldTransform().GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(camera_->GetWorldTransform().GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

	// プレイヤー更新
	player_->Update();

	// テキストスプライトの更新
	text_->Update();

	// ガイドスプライトの更新
	guide_->Update();

	// 状態遷移
	if (stateRequest_) {

		// 状態変更
		gameClearFlowState_ = stateRequest_.value();

		// 状態の初期化
		switch (gameClearFlowState_) {

		case GameClearFlowState::FadeOut:

			FadeOutInitialize();

			break;

		case GameClearFlowState::WaitInput:

			WaitInputInitialize();

			break;

		case GameClearFlowState::FadeIn:

			FadeInInitialize();

			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 各状態の更新
	switch (gameClearFlowState_) {

	case GameClearFlowState::FadeIn:

		FadeInUpdate();

		break;

	case GameClearFlowState::WaitInput:

		WaitInputUpdate();

		break;

	case GameClearFlowState::FadeOut:

		FadeOutUpdate();

		break;
	}
}

void GameClearScene::DrawFiltered() {

	// オブジェクトレンダラーの描画設定
	object3dRenderer_->SettingDrawingOpaque();

	// ライトの描画
	lightManager_->Draw();

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();

	// プレイヤーの描画
	player_->Draw();
}

void GameClearScene::DrawUnfiltered() {

	// スプライトの描画設定
	spriteRenderer_->SettingDrawing();

	// スプライトの描画
	text_->Draw();

	// ガイドスプライトの描画
	guide_->Draw();
}

void GameClearScene::Finalize() {
}

void GameClearScene::ShowImGui() {
}

void GameClearScene::FadeOutInitialize() {

	// フェードアウト開始
	transitionManager->StartOutTransition(
		std::make_unique<FadeTransition>(Vector3{ 1.0f, 1.0f, 1.0f }, 1.0f, 0.0f),
		[this]() { stateRequest_ = GameClearFlowState::WaitInput; },
		2.0f // 遷移にかける時間 (秒)
	);
}

void GameClearScene::FadeOutUpdate() {
}

void GameClearScene::WaitInputInitialize() {
}

void GameClearScene::WaitInputUpdate() {

	// スペースを押したとき
	if (input_->TriggerKey(VK_SPACE)) {

		// 次のシーンをタイトルに設定
		nextScene_ = "TITLE";

		// 状態を加速に変更
		stateRequest_ = GameClearFlowState::FadeIn;
	}
}

void GameClearScene::FadeInInitialize() {

	// シーン名を固定
	const std::string nextScene = nextScene_;

	// フェードイン開始
	transitionManager->StartInTransition(
		std::make_unique<FadeTransition>(Vector3{ 0.0f, 0.0f, 0.0f }, 0.0f, 1.0f),
		[nextScene]() { SceneManager::GetInstance()->ChangeScene(nextScene); },
		2.0f // 遷移にかける時間 (秒)
	);
}

void GameClearScene::FadeInUpdate() {
}
