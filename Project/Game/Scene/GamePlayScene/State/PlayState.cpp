#include "PlayState.h"
#include "GamePlayScene.h"
#include "GameObjectManager.h"
#include "GameRule.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"

#include "OffscreenRendering/FilterManager.h"
#include "Input.h"

using namespace Engine;

/// ================================================== ///
/// 初期化
void PlayState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// オブジェクトマネージャのインスタンスを取得
	gameObjectManager_ = GameObjectManager::GetInstance();

	// プレイヤーのポインタを取得
	player_ = gameObjectManager_->GetPlayer();

	// プレイヤーモードをゲームプレイに変更
	player_->SetPlayerState(PlayerState::Manual);

	// ゴールのポインタを取得
	goal_ = gameObjectManager_->GetGoal();

	// ガイドUIの生成
	guideUI_ = std::make_unique<GuideUI>();
	// ガイドUIの初期化
	guideUI_->Initialize();

	// タイマーUIの生成
	timerUI_ = std::make_unique<TimerUI>();
	// タイマーUIの初期化
	timerUI_->Initialize();

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();
}

/// ================================================== ///
/// 更新
void PlayState::Update() {

	/// ===== ポーズ処理 ===== ///
	if (Input::GetInstance()->TriggerKey(VK_ESCAPE)) {
		scene_->TogglePause();
		return;
	}

	/// ===== 各種オブジェクトの更新 ===== ///

	// ガイドUIの更新
	guideUI_->Update();

	// ゲームルールから経過時間を取得
	timerUI_->SetRemainingTime(scene_->GetGameRule()->GetRemainingTime());

	// タイマーUIの更新
	timerUI_->Update();

	// パーティクルマネージャの更新
	ParticleManager::GetInstance()->Update();

	/// ===== 衝突判定の処理 ===== ///

	// 衝突判定と応答
	scene_->CheckAllCollisions();

	// プレイヤーのデスフラグを取得
	//bool isPlayerDead = player_->IsDead();
	bool isPlayerDead = false;

	// ゴールに到達していたら
	if (isGoalReached_) {

		isFinished_ = true;
	}
	// 残り時間が0になっていたら (ゲームオーバー)
	else if (scene_->GetGameRule()->IsGameOver()) {

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

/// ================================================== ///
/// 描画
void PlayState::Draw() {

	// プレイヤーのUIの描画
	player_->DrawUI();

	// ガイドUIの描画
	guideUI_->Draw();

	// タイマーUIの描画
	timerUI_->Draw();
}

/// ================================================== ///
/// プレイヤーがダメージを受けたときの処理
void PlayState::OnPlayerDamaged() {

	// 被弾ビネットを開始 (1回だけ光らせる)
	isDamageVignetteActive_ = true;
	damageVignetteTimer_ = 0.0f;
}

void PlayState::UpdateVignetteEffect() {

	if (!vignetteFilter_) {
		return;
	}

	// 被弾時のビネット (1回だけ光らせる、優先)
	if (isDamageVignetteActive_) {

		// 経過時間を更新
		damageVignetteTimer_ += 1.0f / 60.0f;

		// ビネットフィルターの設定
		vignetteFilter_->SetIsActive(true);
		vignetteFilter_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		vignetteFilter_->SetIntensity(0.6f);
		vignetteFilter_->SetScale(20.0f);
		vignetteFilter_->SetRange(1.0f);

		// 持続時間が終了したら終了
		if (damageVignetteTimer_ >= kDamageVignetteDuration_) {

			isDamageVignetteActive_ = false;
			vignetteFilter_->SetIsActive(false);
		}

		return;
	}

	// 残り時間が少ないときの点滅
	float remainingTime = scene_->GetGameRule()->GetRemainingTime();
	if (remainingTime <= kLowTimeThreshold_) {

		// 点滅間隔ごとにON/OFFを切り替え
		lowTimeBlinkTimer_ += 1.0f / 60.0f;
		if (lowTimeBlinkTimer_ >= kLowTimeBlinkInterval_) {

			lowTimeBlinkState_ = !lowTimeBlinkState_;
			lowTimeBlinkTimer_ = 0.0f;
		}

		// ビネットフィルターの設定
		vignetteFilter_->SetIsActive(lowTimeBlinkState_);
		vignetteFilter_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		vignetteFilter_->SetIntensity(0.5f);
		vignetteFilter_->SetScale(16.0f);
		vignetteFilter_->SetRange(1.0f);
	}
	else {

		// 点滅状態をリセットして非表示
		vignetteFilter_->SetIsActive(false);
		lowTimeBlinkState_ = false;
		lowTimeBlinkTimer_ = 0.0f;
	}
}