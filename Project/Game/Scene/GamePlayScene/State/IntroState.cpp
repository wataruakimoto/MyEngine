#include "IntroState.h"
#include "GamePlayScene.h"
#include "GameObjectManager.h"

#include "WinApp.h"
#include "OffscreenRendering/FilterManager.h"
#include "Easing.h"
#include "Input.h"

using namespace Engine;
using namespace Easing;

/// ================================================== ///
/// 初期化
void IntroState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// オブジェクトマネージャのインスタンスを取得
	gameObjectManager_ = GameObjectManager::GetInstance();

	// プレイヤーのポインタを取得
	player_ = gameObjectManager_->GetPlayer();

	// 減速開始時の速度を保存
	initialSpeed_ = player_->GetMoveSpeedAuto();

	// プレイヤーモードをオートパイロットに変更
	player_->SetPlayerState(PlayerState::AutoPilot);

	// ゴールのポインタを取得
	goal_ = gameObjectManager_->GetGoal();

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

	// ルールUIの生成
	ruleUI_ = std::make_unique<RuleUI>();
	// ルールUIの初期化
	ruleUI_->Initialize();
}

/// ================================================== ///
/// 更新
void IntroState::Update() {

	// 減速が終わったあとだけ、スペースキー長押しで早送りを受け付ける
	int advanceCount = 1;
	if (isDecelerateFinished_ && Input::GetInstance()->PushKey(VK_SPACE)) {

		advanceCount = kFastForwardSpeed_;
	}

	// 早送り分だけ繰り返し進める
	for (int i = 0; i < advanceCount; ++i) {

		/// ===== 減速の処理 ===== ///

		// タイマーの更新
		if (decelerationTimer_ < kDecelerationDuration) {

			decelerationTimer_ += 1.0f / 60.0f;
		}
		else {

			decelerationTimer_ = kDecelerationDuration;
			isDecelerateFinished_ = true;
		}

		// 減速の処理
		Deceleration();

		// ゴールの更新
		goal_->Update();

		/// ===== UI表示の処理 ===== ///

		if (isDecelerateFinished_ && !isAnimationStarted_) {

			ruleUI_->StartBounceAnimation();

			isAnimationStarted_ = true;
		}

		ruleUI_->Update();

		// パーティクルマネージャの更新
		ParticleManager::GetInstance()->Update();

		if (ruleUI_->IsAnimationFinished()) {

			isFinished_ = true;

			// 終了したのでこれ以上進めない
			break;
		}
	}
}

/// ================================================== ///
/// 描画
void IntroState::Draw() {

	// ルールUIの表示
	ruleUI_->Draw();
}

/// ================================================== ///
/// 減速の処理
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
