#include "GamePlayScene.h"
#include "Input.h"
#include "Vector3.h"
#include "OffscreenRendering/FilterManager.h"
#include "Sprite/SpriteRenderer.h"
#include "Object/Object3dRenderer.h"
#include "Particle/ParticleRenderer.h"
#include "LineManager.h"

#include "GameObjectManager.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "LevelBuilder.h"
#include "State/IntroState.h"
#include "State/PlayState.h"
#include "State/EndingState.h"
#include "State/PauseState.h"
#include "Player/Player.h"

using namespace Engine;

void GamePlayScene::Initialize() {

	// インスタンス取得
	spriteRenderer_ = SpriteRenderer::GetInstance();
	object3dRenderer_ = Object3dRenderer::GetInstance();
	particleRenderer_ = ParticleRenderer::GetInstance();
	lineManager_ = LineManager::GetInstance();

	// カメラマネージャーの生成
	cameraManager_ = std::make_unique<CameraManager>();
	// カメラマネージャーの初期化
	cameraManager_->Initialize();
	// カメラのファークリップ距離を設定
	cameraManager_->GetCamera()->SetFarClip(950.0f);

	// カメラの設定
	object3dRenderer_->SetDefaultCamera(cameraManager_->GetCamera());
	FilterManager::GetInstance()->SetCamera(cameraManager_->GetCamera());
	particleManager_->SetCamera(cameraManager_->GetCamera());
	lineManager_->SetDefaultCamera(cameraManager_->GetCamera());

	// 衝突マネージャの初期化
	collisionManager_ = std::make_unique<Engine::CollisionManager>();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

	// ゲームオブジェクトマネージャーのインスタンスの取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// オブジェクトマネージャーの初期化
	gameObjectManager_->Initialize();
	// オブジェクトマネージャーにシーンのポインタを渡す
	gameObjectManager_->SetGamePlayScene(this);

	// プレイヤーの生成&初期化
	std::unique_ptr<Player> player = std::make_unique<Player>();
	player->SetCamera(cameraManager_->GetCamera());
	player->Initialize();
	player->SetGamePlayScene(this);
	player->SetMoveSpeedAuto(6.0f);
	// オブジェクトマネージャーにプレイヤーを登録
	gameObjectManager_->SetPlayer(std::move(player));

	// 追従カメラコントローラーの生成
	auto followCameraController = std::make_unique<FollowCameraController>();
	// 追従カメラコントローラーの初期化
	followCameraController->Initialize();
	// 追従カメラコントローラーにプレイヤーのポインタを渡す
	followCameraController->SetPlayer(gameObjectManager_->GetPlayer());
	// 追従カメラコントローラーをカメラマネージャーに登録
	cameraManager_->AddCameraController("FollowCamera", std::move(followCameraController));
	// カメラマネージャーのアクティブカメラを追従カメラに設定
	cameraManager_->SwitchCameraController("FollowCamera");

	/// ========== レベルロード ========== ///

	// レベルローダーの生成
	levelLoader_ = std::make_unique<LevelLoader>();
	// レベルローダーの初期化
	levelLoader_->LoadLevel(kLevelDataFileName_);

	// レベルビルダーの生成
	LevelBuilder levelBuilder;
	// レベルデータからレベルを構築
	levelBuilder.BuildLevel(levelLoader_->GetLevelData());

	// 初期状態をイントロに設定
	ChangeState(std::make_unique<IntroState>());

	// ゲームルールの生成
	gameRule_ = std::make_unique<GameRule>();
	// ゲームルールの初期化
	gameRule_->Initialize();
}

void GamePlayScene::Update() {

	// オブジェクトマネージャーの更新
	gameObjectManager_->Update();

	// カメラマネージャーの更新
	cameraManager_->Update();

	// オリジンシフトの確認と実行
	CheckOriginShift();

	// 状態の更新
	state_->Update();

	// 状態の処理が終了していたら
	if (state_->IsFinished()) {

		if (dynamic_cast<IntroState*>(state_.get())) {
			// イントロ状態からプレイ状態へ変更
			ChangeState(std::make_unique<PlayState>());
		}
		else if (dynamic_cast<PlayState*>(state_.get())) {
			// プレイ状態からリザルト状態へ変更
			ChangeState(std::make_unique<EndingState>());
		}
		else if (dynamic_cast<EndingState*>(state_.get())) {
			// エンディング状態は何もしない (シーン移行はフェード終了時に行う)
		}
	}

	// ゲームルールの更新
	gameRule_->Update();
}

void GamePlayScene::DrawFiltered() {

	/// === 3Dオブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingOpaque();

	lightManager_->Draw();

	//TODO: 全ての3Dオブジェクト個々の描画

	// オブジェクトマネージャーの描画
	gameObjectManager_->Draw();

	/// === 半透明オブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingAlpha();

	gameObjectManager_->DrawAlpha();

	/// === パーティクルの描画準備 === ///
	particleRenderer_->SettingDrawing();

	// パーティクルシステムの描画
	particleManager_->Draw();
}

void GamePlayScene::DrawUnfiltered() {

	/// === UIの描画準備 === ///
	spriteRenderer_->SettingDrawing();

	// 状態の描画
	state_->Draw();
}

void GamePlayScene::Finalize() {

	// ゲームオブジェクトマネージャーの終了処理
	gameObjectManager_->Finalize();
}

void GamePlayScene::ShowImGui() {

	Input::GetInstance()->ShowImgui();

	// オブジェクトマネージャーのImGui表示
	gameObjectManager_->ShowImGui();

	lightManager_->ShowImGui();
}

void GamePlayScene::CheckAllCollisions() {

	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// オブジェクトマネージャーの全オブジェクトの衝突をチェック
	gameObjectManager_->RegisterAllColliders(collisionManager_.get());

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void GamePlayScene::ChangeState(std::unique_ptr<IPlayState> newState) {

	// 状態を変更
	state_ = std::move(newState);

	// 新しい状態の初期化
	state_->Initialize(this);
}

void GamePlayScene::OnPlayerDamaged(uint16_t currentHP) {

	if (auto playState = dynamic_cast<PlayState*>(state_.get())) {

		playState->OnPlayerDamaged(currentHP);
	}
}

/// ================================================== ///
/// 敵を倒したときの処理
void GamePlayScene::OnEnemyDefeated() {

	// 現在のカメラマンコントローラーを取得
	ICameraController* currentCameraController = cameraManager_->GetCurrentController();

	// 受け取ったカメラが追従カメラコントローラーだったら
	if (auto followCameraController = dynamic_cast<FollowCameraController*>(currentCameraController)) {
		
		// カメラをシェイクさせる
		followCameraController->StartShake(2.0f, 0.2f);
	}

	// ボーナスタイムを追加
	gameRule_->AddBonusTime(5.0f);
}

/// ================================================== ///
/// ゴールに到達したときの処理
void GamePlayScene::OnGoalReached() {

	// 状態がプレイ状態のとき
	if (auto playState = dynamic_cast<PlayState*>(state_.get())) {

		// ゴール到達の処理
		playState->SetGoalReached(true);
	}
}

void GamePlayScene::TogglePause() {

	if (!pauseState_) {

		// ポーズ状態を保存
		pauseState_ = std::move(state_);

		// ポーズ状態へ変更
		ChangeState(std::make_unique<PauseState>());
	}
	else {

		// ポーズ解除
		state_ = std::move(pauseState_);

		// ポーズ状態ポインタをクリア
		pauseState_ = nullptr;
	}
}

void GamePlayScene::Restart() {

	/// ===== オブジェクトのクリア ===== ///

	particleManager_->Clear();

	/// ========== レベルのリセット ========== ///

	// レベルビルダーの生成
	LevelBuilder levelBuilder;
	// レベルデータからレベルを構築
	levelBuilder.BuildLevel(levelLoader_->GetLevelData());

	/// ===== 状態の更新 ===== ///

	// ステートを破棄
	state_.reset();
	pauseState_.reset();

	// Introから再スタート
	ChangeState(std::make_unique<IntroState>());
}

void GamePlayScene::CheckOriginShift() {

	float playerZ = gameObjectManager_->GetPlayer()->GetWorldTransform().GetWorldPosition().z;

	// プレイヤーのZ座標がループ距離を超えたら
	if (playerZ >= kLoopDistance) {

		float shiftZ = kLoopDistance - 100;

		// ワールドを手前にずらす
		gameObjectManager_->ShiftWorld(shiftZ);
		cameraManager_->ShiftWorld(shiftZ);
	}
}
