#include "PlayState.h"
#include "GamePlayScene.h"
#include "OffscreenRendering/FilterManager.h"
#include "Scene/SceneManager.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "Easing.h"
#include "WinApp.h"
#include "Input.h"

#include <algorithm>

using namespace Easing;

///-------------------------------------------/// 
/// イントロ状態
///-------------------------------------------///

void IntroState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// プレイヤーのポインタを取得
	player_ = scene_->GetPlayer();

	// 減速開始時の速度を保存
	initialSpeed_ = player_->GetMoveSpeedAuto();

	// プレイヤーモードをオートパイロットに変更
	player_->SetPlayerState(PlayerState::AutoPilot);

	// ゴールのポインタを取得
	goal_ = scene_->GetGoal();

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ラジアルブラーのポインタを取得
	radialBlurFilter_ = filterManager_->GetRadialBlurFilter();

	// 減速開始時のブラー強度を保存
	initialBlurStrength_ = radialBlurFilter_->GetBlurStrength();

	// 減速タイマーをセット
	decelerationTimer_ = 0.0f;

	// 減速完了フラグをリセット
	isDecelerateFinished_ = false;

	// ルールUIのポインタを取得
	ruleUI_ = scene_->GetRuleUI();

	// 白フェードのポインタを取得
	whiteFade_ = scene_->GetWhiteFade();
}

void IntroState::Update() {

	/// ===== 減速の処理 ===== ///

	// タイマーの更新
	if (decelerationTimer_ < kDecelerationDuration) {

		decelerationTimer_ += 1.0f / 60.0f;
	}
	else {

		decelerationTimer_ = kDecelerationDuration;
		isDecelerateFinished_ = true;
	}

	Deceleration();

	// ゴールの更新
	goal_->Update();

	/// ===== UI表示の処理 ===== ///

	if (isDecelerateFinished_ && !isAnimationStarted_) {

		ruleUI_->StartBounceAnimation();

		isAnimationStarted_ = true;
	}

	ShowUI();
}

void IntroState::Deceleration() {

	// タイマーの進行度を計算
	float t = std::clamp(decelerationTimer_ / kDecelerationDuration, 0.0f, 1.0f); // タイマーの進行度(0 → 1)

	// プレイヤーの速度を補間して計算
	float playerSpeed = Lerp(initialSpeed_, 0.0f, t);

	// プレイヤーの速度を設定
	player_->SetMoveSpeedAuto(playerSpeed);

	// プレイヤーの更新
	player_->Update();

	// ブラーの強度を補間して計算
	float blurStrength = Lerp(initialBlurStrength_, 0.0f, t);

	// ブラーの強度を設定
	radialBlurFilter_->SetBlurStrength(blurStrength);

	// ブラーの中心を計算
	blurCenter_.x = player_->GetScreenPos().x / WinApp::kClientWidth;
	blurCenter_.y = player_->GetScreenPos().y / WinApp::kClientHeight;

	// ブラーの中心を設定
	radialBlurFilter_->SetCenter(blurCenter_);

	// ブラーの強さが0以下だったら
	if (blurStrength <= 0.0f) {

		radialBlurFilter_->SetIsActive(false);
	}
}

void IntroState::ShowUI() {

	// ルールUIの更新
	ruleUI_->Update();

	if (ruleUI_->IsAnimationFinished()) {

		isFinished_ = true;
	}

	// 白フェードの更新
	whiteFade_->Update();
}

///-------------------------------------------/// 
/// プレイ状態
///-------------------------------------------///

void PlayState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// プレイヤーのポインタを取得
	player_ = scene_->GetPlayer();

	// プレイヤーモードをゲームプレイに変更
	player_->SetPlayerState(PlayerState::Manual);

	// ゴールのポインタを取得
	goal_ = scene_->GetGoal();

	// ノルマUIのポインタを取得
	normaUI_ = scene_->GetNormaUI();

	// ガイドUIのポインタを取得
	guideUI_ = scene_->GetGuideUI();

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();
}

void PlayState::Update() {

	/// ===== ポーズ処理 ===== ///
	if(Input::GetInstance()->TriggerKey(VK_ESCAPE)) {
		scene_->TogglePause();
		return;
	}

	/// ===== 各種オブジェクトの更新 ===== ///

	// プレイヤー更新
	player_->Update();

	// リストで管理しているオブジェクトの更新
	scene_->UpdateListObjects();

	// ゴールの更新
	goal_->Update();

	// ノルマUIに目標値を設定
	normaUI_->SetTargetValue(goal_->GetNormaCount());
	// ノルマUIに現在値を設定
	normaUI_->SetCurrentValue(scene_->GetKillCount());

	// ノルマUIの更新
	normaUI_->Update();

	// ガイドUIの更新
	guideUI_->Update();

	// 敵発生コマンドの更新
	scene_->UpdateEnemyPopCommands();

	// パーティクルマネージャの更新
	ParticleManager::GetInstance()->Update();

	/// ===== 衝突判定の処理 ===== ///

	// 衝突判定と応答
	scene_->CheckAllCollisions();

	// ゴールとプレイヤーの衝突判定
	goal_->CheckGateCollision(scene_->GetKillCount());

	// ゴールライン到達の判定
	bool isReachedGoalLine = player_->GetWorldTransform().GetWorldPosition().z >= goal_->GetWorldTransform().GetTranslate().z;

	// プレイヤーのデスフラグを取得
	bool isPlayerDead = player_->IsDead();

	// ゴールラインに到達していたら
	if (isReachedGoalLine) {

		isFinished_ = true;
	}
	// プレイヤーがデスフラグが立っていたら
	else if (isPlayerDead) {

		isFinished_ = true;
	}

	/// ===== エフェクトの更新 ===== ///

	// ビネットエフェクトの更新
	UpdateVignetteEffect();
}

void PlayState::DrawUI() {

	player_->DrawUI();

	// ノルマUIの描画
	normaUI_->Draw();

	// ガイドUIの描画
	guideUI_->Draw();
}

void PlayState::OnPlayerDamaged(uint16_t currentHP) {

	// ダメージ時の一時ビネットを開始
	isDamageVignetteActive_ = true;
	damageVignetteTimer_ = 0;

	// ビネットフィルターを有効化
	if (vignetteFilter_) {
		vignetteFilter_->SetIsActive(true);
	}
}

void PlayState::OnEnemyDefeated() {

	// カメラシェイクを開始
	if (cameraController_) {
		dynamic_cast<FollowCameraController*>(cameraController_)->StartShake(0.5f, 0.1f);
	}
}

void PlayState::UpdateVignetteEffect() {

	if (!vignetteFilter_ || !player_) {
		return;
	}

	uint16_t currentHP = player_->GetHP();
	PlayerState playerState = player_->GetState();

	// プレイヤーが死亡状態の場合は常時赤いビネットを表示
	if (playerState == PlayerState::Dead) {

		// ビネットフィルターの設定
		vignetteFilter_->SetIsActive(true);
		vignetteFilter_->SetColor({ 0.8f, 0.0f, 0.0f });
		vignetteFilter_->SetIntensity(0.7f);
		vignetteFilter_->SetScale(18.0f);
		vignetteFilter_->SetRange(1.0f);

		// ダメージ時の一時ビネットはリセット
		isDamageVignetteActive_ = false;
		return;
	}

	// HPが1の場合は常時赤いビネットを表示
	if (currentHP == 1) {

		// ビネットフィルターの設定
		vignetteFilter_->SetIsActive(true);
		vignetteFilter_->SetColor({ 0.8f, 0.0f, 0.0f });
		vignetteFilter_->SetIntensity(0.7f);
		vignetteFilter_->SetScale(18.0f);
		vignetteFilter_->SetRange(1.0f);

		// ダメージ時の一時ビネットはリセット
		isDamageVignetteActive_ = false;
		return;
	}

	// ダメージ時の一時ビネット処理
	if (isDamageVignetteActive_) {

		// タイマー更新
		damageVignetteTimer_ += 1.0f / 60.0f; // 60FPS換算

		// 線形補間で徐々にフェードアウト
		float t = damageVignetteTimer_ / kDamageVignetteDuration_;
		float easeT = EaseOutQuad(t); // イージング適用
		float fadeOut = Lerp(1.0f, 0.0f, easeT); // 1から0へ線形補間

		// ビネットフィルターの設定
		vignetteFilter_->SetColor({ 1.0f * fadeOut, 0.0f, 0.0f });
		vignetteFilter_->SetIntensity(0.6f);
		vignetteFilter_->SetScale(20.0f);
		vignetteFilter_->SetRange(1.0f);

		// 継続時間が終了したら元に戻す
		if (damageVignetteTimer_ >= kDamageVignetteDuration_) {
			isDamageVignetteActive_ = false;
			vignetteFilter_->SetIsActive(false);

			// デフォルト値に戻す
			vignetteFilter_->SetColor({ 0.0f, 0.0f, 0.0f });
			vignetteFilter_->SetIntensity(0.8f);
			vignetteFilter_->SetScale(16.0f);
			vignetteFilter_->SetRange(1.0f);
		}
	}
	// HP2以上でダメージビネットも無効な場合はビネットを非表示
	else if (currentHP >= 2) {

		vignetteFilter_->SetIsActive(false);
	}
}

///-------------------------------------------/// 
/// エンディング状態
///-------------------------------------------///

void EndingState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// プレイヤーのポインタを取得
	player_ = scene_->GetPlayer();

	// プレイヤーモードをオートパイロットに変更
	player_->SetPlayerState(PlayerState::AutoPilot);

	// ゴールのポインタを取得
	goal_ = scene_->GetGoal();

	// リザルトUIのポインタを取得
	resultUI_ = scene_->GetResultUI();

	// 白フェードのポインタを取得
	whiteFade_ = scene_->GetWhiteFade();

	// フェード時間を設定
	whiteFade_->SetFadeDuration(3.0f);

	// 黒フェードのポインタを取得
	blackFade_ = scene_->GetBlackFade();

	// フェード時間を設定
	blackFade_->SetFadeDuration(3.0f);

	isAnimationFinished_ = false;

	isFadeStarted_ = false;

	isPlayerDead_ = player_->IsDead();

	isClear_ = scene_->GetKillCount() >= goal_->GetNormaCount();

	// プレイヤーが死んでたら
	if (isPlayerDead_) {

		// プレイヤーの速度を0にする
		player_->SetMoveSpeedAuto(0.0f);

		// 黒フェードの開始
		blackFade_->StartFadeAnimation(BlackFade::FadeType::In);

		// フェード開始
		isFadeStarted_ = true;

		// アニメーション完了フラグを立てる
		isAnimationFinished_ = true;
	}
	// プレイヤーが生きてたら
	else {

		// クリアしていたら
		if (isClear_) {

			// プレイヤーの速度を0.5にする
			player_->SetMoveSpeedAuto(0.5f);

			// リザルトUIにクリアアニメーションを開始させる
			resultUI_->StartAnimation(ResultType::Clear);
		}
		// ゲームオーバーなら
		else {

			// プレイヤーの速度を0にする
			player_->SetMoveSpeedAuto(0.0f);

			// リザルトUIにゲームオーバーアニメーションを開始させる
			resultUI_->StartAnimation(ResultType::GameOver);
		}
	}

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();
}

void EndingState::Update() {

	// プレイヤー更新
	player_->Update();

	// ゴールの更新
	goal_->Update();

	/// ===== UI表示の処理 ===== ///

	// プレイヤーが生きていて、アニメーションが完了していなかったら
	if (!isPlayerDead_ && !isAnimationFinished_) {

		// リザルトUIの表示
		ShowUI();
	}

	/// ===== フェードの処理 ===== ///

	// アニメーションが完了していたら
	if (isAnimationFinished_) {

		if (!isFadeStarted_) {

			// フェードインの開始
			whiteFade_->StartFadeAnimation(WhiteFade::FadeType::In);
			blackFade_->StartFadeAnimation(BlackFade::FadeType::In);

			// フェード開始
			isFadeStarted_ = true;
		}

		// フェードインの更新
		FadeIn();
	}
}

void EndingState::ShowUI() {

	// リザルトUIの更新
	resultUI_->Update();

	if (resultUI_->IsAnimationFinished()) {
		isAnimationFinished_ = true;
	}
}

void EndingState::FadeIn() {

	// プレイヤーが生きていて、クリアしていたら
	if (!isPlayerDead_ && isClear_) {

		// 白フェードの更新
		whiteFade_->Update();

		if (whiteFade_->IsFadeFinished()) {

			vignetteFilter_->SetIsActive(false);

			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("CLEAR");
		}
	}
	// ゲームオーバーなら
	else {

		// 黒フェードの更新
		blackFade_->Update();

		if (blackFade_->IsFadeFinished()) {

			vignetteFilter_->SetIsActive(false);

			// シーン切り替え
			SceneManager::GetInstance()->ChangeScene("OVER");
		}
	}
}

void PauseState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// ポーズUIの生成・初期化
	pauseUI = std::make_unique<PauseUI>();
	pauseUI->Initialize();

	// ノルマUIのポインタを取得
	normaUI_ = scene_->GetNormaUI();

	// ガイドUIのポインタを取得
	guideUI_ = scene_->GetGuideUI();

	// 白フェードのポインタを取得
	whiteFade_ = scene_->GetWhiteFade();

	isSelectedRestart_ = false;

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();
}

void PauseState::Update() {

	guideUI_->Update();

	if (isSelectedRestart_) {

		// 白フェードの更新
		whiteFade_->Update();

		if (whiteFade_->IsFadeFinished()) {

			vignetteFilter_->SetIsActive(false);

			SceneManager::GetInstance()->ChangeScene("PLAY");
		}

		return;
	}

	// ポーズUIの更新
	pauseUI->Update();

	// 現在の選択肢を取得
	PauseSelect selected = pauseUI->GetCurrentSelect();

	// 選択肢に応じた処理
	switch (selected) {

	case PauseSelect::Resume:

		scene_->TogglePause();

		isFinished_ = true;

		return;

	case PauseSelect::Restart:
		
		if (!isSelectedRestart_) {

			// 白フェードの開始
			whiteFade_->StartFadeAnimation(WhiteFade::FadeType::In);
			isSelectedRestart_ = true;
		}

		break;

	case PauseSelect::Quit:

		vignetteFilter_->SetIsActive(false);

		SceneManager::GetInstance()->ChangeScene("TITLE");
		
		return;

	case PauseSelect::None:
		// 何もしない
		break;
	}

	if(Input::GetInstance()->TriggerKey(VK_ESCAPE)){

		scene_->TogglePause();
		
		return;
	}
}

void PauseState::DrawPauseUI() {

	// ポーズUIの描画
	pauseUI->Draw();

	// ノルマUIの描画
	normaUI_->Draw();

	// ガイドUIの描画
	guideUI_->Draw();
}
