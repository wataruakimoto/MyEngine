#include "EndingState.h"
#include "GamePlayScene.h"
#include "GameObjectManager.h"
#include "GameRule.h"

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

	// オブジェクトマネージャのインスタンスを取得
	gameObjectManager_ = GameObjectManager::GetInstance();

	// ゲームルールの借りポインタを取得
	gameRule_ = scene_->GetGameRule();

	// プレイヤーのポインタを取得
	player_ = gameObjectManager_->GetPlayer();
	// プレイヤーモードをオートパイロットに変更
	player_->SetPlayerState(PlayerState::AutoPilot);

	// ゴールのポインタを取得
	goal_ = gameObjectManager_->GetGoal();

	// リザルトUIの生成
	resultUI_ = std::make_unique<ResultUI>();
	// リザルトUIの初期化
	resultUI_->Initialize();

	isAnimationFinished_ = false;

	isFadeStarted_ = false;

	// クリアだったら
	if (gameRule_->IsClear()) {

		// プレイヤーの速度を0.5にする
		player_->SetMoveSpeedAuto(0.5f);

		// リザルトUIにクリアアニメーションを開始させる
		resultUI_->StartAnimation(ResultType::Clear);

		// 次のシーンをゲームクリアシーンに設定
		nextScene_ = "CLEAR";
	}
	// ゲームオーバーだったら
	else if (gameRule_->IsGameOver()) {

		// プレイヤーの速度を0.5にする
		player_->SetMoveSpeedAuto(0.5f);

		// リザルトUIにゲームオーバーアニメーションを開始させる
		resultUI_->StartAnimation(ResultType::GameOver);
		
		// 次のシーンをゲームオーバーシーンに設定
		nextScene_ = "OVER";
	}
	// それ以外だったら
	else {

		// プレイヤーの速度を0.5にする
		player_->SetMoveSpeedAuto(0.5f);

		// リザルトUIにゲームオーバーアニメーションを開始させる
		resultUI_->StartAnimation(ResultType::GameOver);

		// 次のシーンをゲームオーバーシーンに設定
		nextScene_ = "OVER";
	}

	// フィルターマネージャのインスタンスを取得
	filterManager_ = FilterManager::GetInstance();

	// ビネットフィルターのポインタを取得
	vignetteFilter_ = filterManager_->GetVignetteFilter();

	// 遷移マネージャのインスタンスを取得
	transitionManager_ = TransitionManager::GetInstance();
}

/// ================================================== ///
/// 更新
void EndingState::Update() {

	/// ===== UI表示の処理 ===== ///

	// アニメーションが完了していなかったら
	if (!isAnimationFinished_) {

		// リザルトUIの更新
		resultUI_->Update();

		if (resultUI_->IsAnimationFinished()) {
			isAnimationFinished_ = true;
		}
	}

	/// ===== フェードの処理 ===== ///

	// アニメーションが完了していて、フェードが開始されていなかったら
	if (isAnimationFinished_ && !isFadeStarted_) {

		// フェードを開始する
		StartFadeIn();

		// フェード開始フラグを立てる
		isFadeStarted_ = true;
	}

	// パーティクルマネージャの更新
	ParticleManager::GetInstance()->Update();
}

/// ================================================== ///
/// 描画
void EndingState::Draw() {

	//resultUI_->Draw();
}

/// ================================================== ///
/// フェードインを開始する
void EndingState::StartFadeIn() {

	// 遷移先シーン名をコピーしてキャプチャ
	const std::string nextScene = nextScene_;

	vignetteFilter_->SetIsActive(false);

	// 白でフェードイン→完了後にシーン切り替え
	transitionManager_->StartInTransition(
		std::make_unique<FadeTransition>(Vector3{ 1.0f, 1.0f, 1.0f }, 0.0f, 1.0f),
		[nextScene]() {
			SceneManager::GetInstance()->ChangeScene(nextScene);
		},
		2.0f
	);
}
