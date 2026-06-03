#include "EndingState.h"
#include "GamePlayScene.h"

#include "OffscreenRendering/FilterManager.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "Transition/FadeTransition.h"

using namespace Engine;

/// ================================================== ///
/// 初期化
void EndingState::Initialize(GamePlayScene* scene) {

	// 引数をメンバ変数にセット
	scene_ = scene;

	// プレイヤーのポインタを取得
	player_ = scene_->GetGameObjectManager()->GetPlayer();

	// プレイヤーモードをオートパイロットに変更
	player_->SetPlayerState(PlayerState::AutoPilot);

	// ゴールのポインタを取得
	goal_ = scene_->GetGameObjectManager()->GetGoal();

	// リザルトUIの生成
	resultUI_ = std::make_unique<ResultUI>();
	// リザルトUIの初期化
	resultUI_->Initialize();

	isAnimationFinished_ = false;

	isFadeStarted_ = false;

	isPlayerDead_ = player_->IsDead();

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

/// ================================================== ///
/// 更新
void EndingState::Update() {

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

	// パーティクルマネージャの更新
	ParticleManager::GetInstance()->Update();
}

/// ================================================== ///
/// UI表示の処理
void EndingState::ShowUI() {

	// リザルトUIの更新
	resultUI_->Update();

	if (resultUI_->IsAnimationFinished()) {
		isAnimationFinished_ = true;
	}
}

/// ================================================== ///
/// フェードインの処理
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