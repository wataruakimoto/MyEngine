#include "PlayState.h"
#include "GamePlayScene.h"

#include "OffscreenRendering/FilterManager.h"
#include "Easing.h"
#include "WinApp.h"

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

		//TODO: 遷移の仕組みを決めてから
	}
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

	// 3Dレティクルのポインタを取得
	reticle3D_ = scene_->GetReticle3D();

	// 2Dレティクルのポインタを取得
	reticle2D_ = scene_->GetReticle2D();

	// ロックオンのポインタを取得
	lockOn_ = scene_->GetLockOn();

	// ノルマUIのポインタを取得
	normaUI_ = scene_->GetNormaUI();

	// ガイドUIのポインタを取得
	guideUI_ = scene_->GetGuideUI();

	// 敵発生データの読み込み
	scene_->LoadEnemyPopData();
}

void PlayState::Update() {

	// プレイヤー更新
	player_->Update();

	// 弾の更新
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {

		bullet->Update();
	}

	// 敵キャラの更新
	for (std::unique_ptr<Enemy>& enemy : enemies_) {

		// プレイヤーを敵にセット
		enemy->SetPlayer(player_.get());

		// 敵更新
		enemy->Update();
	}

	// 敵の弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	// ゴールの更新
	goal_->Update();

	// 3Dレティクルの更新
	reticle3D_->Update();

	// 2Dレティクルの更新
	reticle2D_->Update();

	// ロックオンの更新
	lockOn_->Update();

	// ノルマUIに目標値を設定
	normaUI_->SetTargetValue(goal_->GetNormaCount());
	// ノルマUIに現在値を設定
	normaUI_->SetCurrentValue(killCount_);

	// ノルマUIの更新
	normaUI_->Update();

	// ガイドUIの更新
	guideUI_->Update();

	// 敵発生コマンドの更新
	scene_->UpdateEnemyPopCommands();
}

///-------------------------------------------/// 
/// リザルト状態
///-------------------------------------------///

void ResultState::Initialize(GamePlayScene* scene) {
}

void ResultState::Update() {
}
