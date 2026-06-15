#include "GameOverScene.h"
#include "WinApp.h"
#include "Object/Object3dRenderer.h"
#include "OffscreenRendering/FilterManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "OffscreenRendering/Filters/FogFilter.h"
#include "OffscreenRendering/Filters/RadialBlurFilter.h"
#include "Transition/FadeTransition.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "Sprite/SpriteRenderer.h"
#include "Particle/ParticleRenderer.h"

using namespace Engine;

void GameOverScene::Initialize() {

	// インスタンスの取得
	object3dRenderer_ = Object3dRenderer::GetInstance();
	filterManager_ = FilterManager::GetInstance();
	input_ = Input::GetInstance();
	sceneManager_ = SceneManager::GetInstance();
	transitionManager = TransitionManager::GetInstance();
	spriteRenderer_ = SpriteRenderer::GetInstance();
	particleRenderer_ = ParticleRenderer::GetInstance();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

	// フォグをフィルターマネージャから受け取っとく
	fogFilter_ = filterManager_->GetFogFilter();
	fogFilter_->SetStartDistance(500.0f); // フォグ開始距離を500に設定
	fogFilter_->SetIsActive(true);    // フォグを有効化

	// ラジアルブラーをフィルターマネージャから受け取っとく
	radialBlurFilter_ = filterManager_->GetRadialBlurFilter();
	radialBlurFilter_->SetIsActive(false); // ラジアルブラーは最初は無効化

	// カメラマネージャーの生成
	cameraManager_ = std::make_unique<CameraManager>();
	// カメラマネージャーの初期化
	cameraManager_->Initialize();
	// カメラのファークリップ距離を設定
	cameraManager_->GetCamera()->SetFarClip(950.0f);

	// カメラの設定
	object3dRenderer_->SetDefaultCamera(cameraManager_->GetCamera());
	filterManager_->SetCamera(cameraManager_->GetCamera());
	particleManager_->SetCamera(cameraManager_->GetCamera());

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
	player_->SetCamera(cameraManager_->GetCamera());

	playerMoveSpeed_ = 0.0f;
	player_->SetMoveSpeedAuto(playerMoveSpeed_);


	// 追従カメラコントローラーの生成
	auto followCameraController = std::make_unique<FollowCameraController>();
	// 追従カメラコントローラーの初期化
	followCameraController->Initialize();
	// 追従カメラコントローラーにプレイヤーのポインタを渡す
	followCameraController->SetPlayer(player_.get());
	// 追従カメラコントローラーをカメラマネージャーに登録
	cameraManager_->AddCameraController("FollowCamera", std::move(followCameraController));
	// カメラマネージャーのアクティブカメラを追従カメラに設定
	cameraManager_->SwitchCameraController("FollowCamera");

	// テキストスプライトの生成&初期化
	text_ = std::make_unique<Engine::Sprite>();
	text_->Initialize("GameOver.png");

	// ガイドスプライトの生成&初期化
	guide_ = std::make_unique<Engine::Sprite>();
	guide_->Initialize("SpaceToTitle.png");
	guide_->SetPosition({ 640.0f, 600.0f });
	guide_->SetAnchorPoint({ 0.5f, 0.5f });

	// 最初の状態をリクエスト
	stateRequest_ = GameOverFlowState::FadeOut;
}

void GameOverScene::Update() {

	// カメラマネージャーの更新
	cameraManager_->Update();

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(cameraManager_->GetCamera()->GetWorldTransform().GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(cameraManager_->GetCamera()->GetWorldTransform().GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

	// 状態の更新
	if (stateRequest_) {

		// 状態変更
		gameOverFlowState_ = stateRequest_.value();

		// 状態の初期化
		switch (gameOverFlowState_) {

		case GameOverFlowState::FadeOut:

			FadeOutInitialize();

			break;

		case GameOverFlowState::WaitInput:

			WaitInputInitialize();

			break;

		case GameOverFlowState::SpeedUp:

			SpeedUpInitialize();

			break;

		case GameOverFlowState::FadeIn:

			FadeInInitialize();

			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 各状態の更新
	switch (gameOverFlowState_) {

	case GameOverFlowState::FadeIn:

		FadeInUpdate();

		break;

	case GameOverScene::GameOverFlowState::WaitInput:

		WaitInputUpdate();

		break;

	case GameOverScene::GameOverFlowState::SpeedUp:

		SpeedUpUpdate();

		break;

	case GameOverFlowState::FadeOut:

		FadeOutUpdate();

		break;
	}

	// プレイヤー更新
	player_->Update();

	// テキストスプライトの更新
	text_->Update();

	// ガイドスプライトの更新
	guide_->Update();

	// パーティクルマネージャの更新
	particleManager_->Update();
}

void GameOverScene::DrawFiltered() {

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

	/// === パーティクルの描画準備 === ///
	particleRenderer_->SettingDrawing();

	// パーティクルシステムの描画
	particleManager_->Draw();
}

void GameOverScene::DrawUnfiltered() {

	// スプライトの描画設定
	spriteRenderer_->SettingDrawing();

	// スプライトの描画
	text_->Draw();

	// ガイドスプライトの描画
	guide_->Draw();
}

void GameOverScene::Finalize() {
}

void GameOverScene::ShowImGui() {
}

void GameOverScene::FadeOutInitialize() {

	// フェードアウト開始
	transitionManager->StartOutTransition(
		std::make_unique<FadeTransition>(Vector3{ 0.0f, 0.0f, 0.0f }, 1.0f, 0.0f),
		[this]() { stateRequest_ = GameOverFlowState::WaitInput; },
		2.0f // 遷移にかける時間 (秒)
	);
}

void GameOverScene::FadeOutUpdate() {
}

void GameOverScene::WaitInputInitialize() {
}

void GameOverScene::WaitInputUpdate() {

	// スペースを押したとき
	//if (input_->TriggerKey(VK_SPACE)) {
	//
	//	// 白色で設定
	//	fadeColor_ = { 1.0f,1.0f,1.0f };
	//
	//	// 次のシーンをプレイに設定
	//	nextScene_ = "PLAY";
	//
	//	// 状態を加速に変更
	//	stateRequest_ = GameOverFlowState::SpeedUp;
	//}

	// バックスペースを押したとき
	if (input_->TriggerKey(VK_SPACE)) {

		// 黒色で設定
		fadeColor_ = { 0.0f,0.0f,0.0f };

		// 次のシーンをタイトルに設定
		nextScene_ = "TITLE";

		// 状態をフェードインに変更
		stateRequest_ = GameOverFlowState::FadeIn;
	}
}

void GameOverScene::SpeedUpInitialize() {

	// ラジアルブラーをつける
	//radialBlurFilter_->SetIsActive(true);

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

		stateRequest_ = GameOverFlowState::FadeIn;
	}
}

void GameOverScene::FadeInInitialize() {

	// シーン名を固定
	const std::string nextScene = nextScene_;

	// フェードイン開始
	transitionManager->StartInTransition(
		std::make_unique<FadeTransition>(fadeColor_, 0.0f, 1.0f),
		[nextScene]() { SceneManager::GetInstance()->ChangeScene(nextScene); },
		2.0f // 遷移にかける時間 (秒)
	);
}

void GameOverScene::FadeInUpdate() {
}
