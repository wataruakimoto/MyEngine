#include "TitleScene.h"
#include "Skybox/SkyboxCommon.h"
#include "Object/Object3dCommon.h"
#include "Sprite/SpriteCommon.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"

#include <imgui.h>

void TitleScene::Initialize() {

	// カメラの生成&初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->Initialize();
	cameraController_->SetCamera(camera_.get());

	// カメラの設定
	SkyboxCommon::GetInstance()->SetDefaultCamera(camera_.get());
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_.get());

	// プレイヤーの生成&初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetPlayerMode(PlayerMode::Title); // タイトルモードに設定

	// キャストし追従カメラの方を呼び出す
	dynamic_cast<FollowCameraController*>(cameraController_.get())->SetTarget(&player_->GetWorldTransform());

	// フロアを生成
	floor_ = std::make_unique<Floor>();
	floor_->Initialize();

	// シリンダーの生成
	cylinder_ = std::make_unique<Cylinder>();
	cylinder_->Initialize();

	// スカイボックスの生成
	skyBox_ = std::make_unique<SkyBoxGame>();
	skyBox_->Initialize();
	// カメラを設定
	skyBox_->SetCamera(camera_.get());
	// プレイヤーを設定
	skyBox_->SetPlayer(player_.get());

	// 黒画面UIの生成&初期化
	blackScreen_ = std::make_unique<BlackScreen>();
	blackScreen_->Initialize();

	// タイトルUIの生成&初期化
	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Initialize();

	// スタートUIの生成&初期化
	startUI_ = std::make_unique<StartUI>();
	startUI_->Initialize();
}

void TitleScene::Update() {

	// 状態の変更がリクエストされていたら
	if (stateRequest_) {

		// 状態を変更
		titleFlowState_ = stateRequest_.value();

		// 各状態の初期化
		switch (titleFlowState_) {

		case TitleFlowState::Blackout:
			BlackoutInitialize();
			break;

		case TitleFlowState::SlideIn:
			SlideInInitialize();
			break;

		case TitleFlowState::FadeOut:
			FadeOutInitialize();
			break;

		case TitleFlowState::WaitInput:
			WaitInputInitialize();
			break;

		case TitleFlowState::MoveUp:
			MoveUpInitialize();
			break;

		default:
			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 各状態の更新
	switch (titleFlowState_) {

	case TitleFlowState::Blackout:
		BlackoutUpdate();
		break;

	case TitleFlowState::SlideIn:
		SlideInUpdate();
		break;

	case TitleFlowState::FadeOut:
		FadeOutUpdate();
		break;

	case TitleFlowState::WaitInput:
		WaitInputUpdate();
		break;

	case TitleFlowState::MoveUp:
		MoveUpUpdate();
		break;

	default:
		break;
	}

	// カメラコントローラの更新
	cameraController_->Update();

	// プレイヤー更新
	player_->Update();

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(camera_->GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(camera_->GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

	// スカイボックスの更新
	skyBox_->Update();

	// タイトルUIの更新
	titleUI_->Update();

	// スタートUIの更新
	startUI_->Update();

	// 黒画面UIの更新
	blackScreen_->Update();
}

void TitleScene::Draw() {

	/// === スカイボックスの描画準備 === ///
	SkyboxCommon::GetInstance()->SettingDrawing();

	// スカイボックスの描画
	skyBox_->Draw();

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();

	// プレイヤー描画
	player_->Draw();

	/// === 2Dオブジェクトの描画準備(最前面) === ///
	SpriteCommon::GetInstance()->SettingDrawing();

	// スタートUIの描画
	startUI_->Draw();

	// 黒画面UIの描画
	blackScreen_->Draw();

	// タイトルUIの描画
	titleUI_->Draw();
}

void TitleScene::Finalize() {
}

void TitleScene::ShowImGui() {

	// カメラコントローラのImGui表示
	cameraController_->ShowImGui();

	// プレイヤーのImGui表示
	player_->ShowImGui();

	// フロアのImGui表示
	floor_->ShowImGui();

	// シリンダーのImGui表示
	cylinder_->ShowImGui();

	// スカイボックスのImGui表示
	skyBox_->ShowImGui();

	// タイトルUIのImGui表示
	titleUI_->ShowImGui();

	// スタートUIのImGui表示
	startUI_->ShowImGui();

#ifdef _DEBUG

	ImGui::Begin("TitleScene");
	ImGui::Text("TitleFlowState");
	ImGui::Text("%d", static_cast<int>(titleFlowState_));
	ImGui::End();

#endif // _DEBUG
}

void TitleScene::BlackoutInitialize() {

	// 黒画面のアルファ値を1に設定
	blackScreen_->SetAlpha(1.0f);

	// スタートUIを非表示に設定
	startUI_->SetVisible(false);
}

void TitleScene::BlackoutUpdate() {

	// 状態をスライドインに変更
	stateRequest_ = TitleFlowState::SlideIn;
}

void TitleScene::SlideInInitialize() {

	// タイトルUIのスライドインアニメーション開始
	titleUI_->StartSlideInAnimation();

	// スタートUIを非表示に設定
	startUI_->SetVisible(false);
}

void TitleScene::SlideInUpdate() {

	// スライドインが終了したら
	if (titleUI_->IsSlideInFinished()) {

		// 状態をフェードアウトに変更
		stateRequest_ = TitleFlowState::FadeOut;
	}
}

void TitleScene::FadeOutInitialize() {

	// 黒画面のアルファ値を1に設定
	blackScreen_->SetAlpha(1.0f);

	// スタートUIを非表示に設定
	startUI_->SetVisible(false);
}

void TitleScene::FadeOutUpdate() {

	// 黒画面のアルファ値を徐々に減少
	blackScreen_->SubAlpha(0.005f);

	// 黒画面が完全に透明になったら
	if (blackScreen_->GetAlpha() <= 0.0f) {

		// 状態を入力待ちに変更
		stateRequest_ = TitleFlowState::WaitInput;
	}
}

void TitleScene::WaitInputInitialize() {

	// タイマーリセット
	startUI_->ResetBlinkTimer();

	// スタートUIを表示に設定
	startUI_->SetVisible(true);
}

void TitleScene::WaitInputUpdate() {

	// スペースキーを押したら
	if (input_->TriggerKey(DIK_SPACE)) {

		// 状態をUI上に移動に変更
		stateRequest_ = TitleFlowState::MoveUp;
	}
}

void TitleScene::MoveUpInitialize() {

	// タイトルUIの上に移動アニメーション開始
	titleUI_->StartMoveUpAnimation();

	// スタートUIを非表示に設定
	startUI_->SetVisible(false);
}

void TitleScene::MoveUpUpdate() {

	// 上に移動が終了したら
	if (titleUI_->IsMoveUpFinished()) {

		// シーンをゲームプレイシーンに変更
		sceneManager_->ChangeScene("PLAY");
	}
}
