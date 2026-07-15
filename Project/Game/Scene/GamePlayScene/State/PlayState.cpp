#include "PlayState.h"
#include "GamePlayScene.h"
#include "GameObjectManager.h"
#include "GameRule.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"

#include "OffscreenRendering/FilterManager.h"
#include "Input.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

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
void PlayState::OnPlayerDamaged(uint16_t currentHP) {

	// ダメージ時の一時ビネットを開始
	isDamageVignetteActive_ = true;
	damageVignetteTimer_ = 0;

	// ビネットフィルターを有効化
	if (vignetteFilter_) {
		vignetteFilter_->SetIsActive(true);
	}
}

void PlayState::UpdateVignetteEffect() {

	if (!vignetteFilter_ || !player_) {
		return;
	}

	//uint16_t currentHP = player_->GetHP();
	uint32_t currentHP = 100; // デフォルト値
	PlayerState playerState = player_->GetState();

	//// プレイヤーが死亡状態の場合は常時赤いビネットを表示
	//if (playerState == PlayerState::Dead) {
	//
	//	// ビネットフィルターの設定
	//	vignetteFilter_->SetIsActive(true);
	//	vignetteFilter_->SetColor({ 0.8f, 0.0f, 0.0f, 1.0f });
	//	vignetteFilter_->SetIntensity(0.7f);
	//	vignetteFilter_->SetScale(18.0f);
	//	vignetteFilter_->SetRange(1.0f);
	//
	//	// ダメージ時の一時ビネットはリセット
	//	isDamageVignetteActive_ = false;
	//	return;
	//}

	// HPが1の場合は常時赤いビネットを表示
	if (currentHP == 1) {

		// ビネットフィルターの設定
		vignetteFilter_->SetIsActive(true);
		vignetteFilter_->SetColor({ 0.8f, 0.0f, 0.0f, 1.0f });
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
		vignetteFilter_->SetColor({ 1.0f, 0.0f, 0.0f, fadeOut });
		vignetteFilter_->SetIntensity(0.6f);
		vignetteFilter_->SetScale(20.0f);
		vignetteFilter_->SetRange(1.0f);

		// 継続時間が終了したら元に戻す
		if (damageVignetteTimer_ >= kDamageVignetteDuration_) {
			isDamageVignetteActive_ = false;
			vignetteFilter_->SetIsActive(false);

			// デフォルト値に戻す
			vignetteFilter_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
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