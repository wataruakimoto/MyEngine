#include "GamePlayScene.h"
#include "Input.h"
#include "Vector3.h"
#include "OffscreenRendering/FilterManager.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"

#include "Sprite/SpriteRenderer.h"
#include "Object/Object3dRenderer.h"
#include "Particle/ParticleRenderer.h"
#include "LineManager.h"

using namespace Engine;

void GamePlayScene::Initialize() {

	// インスタンス取得
	spriteRenderer_ = SpriteRenderer::GetInstance();
	object3dRenderer_ = Object3dRenderer::GetInstance();
	particleRenderer_ = ParticleRenderer::GetInstance();
	lineManager_ = LineManager::GetInstance();

	// カメラの生成&初期化
	camera_ = std::make_unique<Engine::Camera>();
	camera_->Initialize();
	camera_->SetFarClip(950.0f); // ファークリップを950に設定

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->SetCamera(camera_.get());
	cameraController_->Initialize();

	// カメラの設定
	object3dRenderer_->SetDefaultCamera(camera_.get());
	FilterManager::GetInstance()->SetCamera(camera_.get());
	particleManager_->SetCamera(camera_.get());
	lineManager_->SetDefaultCamera(camera_.get());

	// 衝突マネージャの初期化
	collisionManager_ = std::make_unique<Engine::CollisionManager>();

	// ライトマネージャの初期化
	lightManager_ = std::make_unique<Engine::LightManager>();
	lightManager_->Initialize();

	// プレイヤーの生成&初期化
	player_ = std::make_unique<Player>();
	player_->SetCamera(camera_.get());
	player_->Initialize();
	player_->SetGamePlayScene(this);
	player_->SetMoveSpeedAuto(6.0f);

	// キャストし追従カメラの方を呼び出す
	dynamic_cast<FollowCameraController*>(cameraController_.get())->SetPlayer(player_.get());

	// フロアを生成
	floor_ = std::make_unique<Floor>();
	floor_->Initialize();

	// シリンダーの生成
	cylinder_ = std::make_unique<Cylinder>();
	cylinder_->Initialize();

	// ゴールの生成&初期化
	goal_ = std::make_unique<Goal>();
	goal_->Initialize();
	// プレイヤーをゴールに設定
	goal_->SetPlayer(player_.get());

	// ルールUIの生成&初期化
	ruleUI_ = std::make_unique<RuleUI>();
	ruleUI_->Initialize();

	// ノルマUIの生成&初期化
	normaUI_ = std::make_unique<NormaUI>();
	normaUI_->Initialize();
	// ノルマUIに目標値を設定
	normaUI_->SetTargetValue(goal_->GetNormaCount());
	// ノルマUIに現在値を設定
	normaUI_->SetCurrentValue(0);

	// リザルトUIの生成&初期化
	resultUI_ = std::make_unique<ResultUI>();
	resultUI_->Initialize();

	// ガイドUIの生成&初期化
	guideUI_ = std::make_unique<GuideUI>();
	guideUI_->Initialize();

	// 白フェードの初期化
	whiteFade_ = std::make_unique<WhiteFade>();
	whiteFade_->Initialize();
	whiteFade_->StartFadeAnimation(WhiteFade::FadeType::Out);
	whiteFade_->SetFadeDuration(1.5f);

	// 黒フェードの初期化
	blackFade_ = std::make_unique<BlackFade>();
	blackFade_->Initialize();

	/// ========== レベルロード ========== ///

	LoadLevelAndApply();

	// 初期状態をイントロに設定
	ChangeState(std::make_unique<IntroState>());
}

void GamePlayScene::Update() {

	// カメラコントローラの更新
	cameraController_->Update();

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(camera_->GetWorldTransform().GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(camera_->GetWorldTransform().GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

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
}

void GamePlayScene::DrawFiltered() {

	/// === 3Dオブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingOpaque();

	lightManager_->Draw();

	//TODO: 全ての3Dオブジェクト個々の描画

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();

	// 敵描画
	for (std::unique_ptr<Enemy>& enemy : enemies_) {

		enemy->Draw();
	}

	// 敵の弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {

		bullet->Draw();
	}

	// 弾の描画
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {

		bullet->Draw();
	}

	// 障害物の描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {

		obstacle->Draw();
	}

	// プレイヤー描画
	player_->Draw();

	/// === 半透明オブジェクトの描画準備 === ///
	object3dRenderer_->SettingDrawingAlpha();

	// ゴールの描画
	goal_->Draw();

	/// === パーティクルの描画準備 === ///
	particleRenderer_->SettingDrawing();

	// パーティクルシステムの描画
	particleManager_->Draw();
}

void GamePlayScene::DrawUnfiltered() {

	/// === UIの描画準備 === ///
	spriteRenderer_->SettingDrawing();

	// TODO: 全てのスプライト個々の描画

	// ルールUIの描画
	ruleUI_->Draw();

	// リザルトUIの描画
	resultUI_->Draw();

	if (PlayState* playState = dynamic_cast<PlayState*>(state_.get())) {
		
		// プレイ状態ならプレイUIの描画
		playState->DrawUI();
	}

	if (PauseState* pauseState = dynamic_cast<PauseState*>(state_.get())) {

		// ポーズ状態ならポーズUIの描画
		pauseState->DrawPauseUI();
	}

	// 白フェードの描画
	whiteFade_->Draw();

	// 黒フェードの描画
	blackFade_->Draw();
}

void GamePlayScene::Finalize() {

	// 敵の解放
	for (std::unique_ptr<Enemy>& enemy : enemies_) {

		enemy->Finalize();
	}

	// プレイヤーの解放
	player_->Finalize();
}

void GamePlayScene::ShowImGui() {

	Input::GetInstance()->ShowImgui();

	camera_->ShowImGui("Camera");

	cameraController_->ShowImGui();

	player_->ShowImGui();

	for (std::unique_ptr<Enemy>& enemy : enemies_) { enemy->ShowImGui(); }

	for (std::unique_ptr<Bullet>& bullet : playerBullets_) { bullet->ShowImGui(); }

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) { bullet->ShowImGui(); }

	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) { obstacle->ShowImGui(); }

	floor_->ShowImGui();

	cylinder_->ShowImGui();

	ruleUI_->ShowImGui();

	normaUI_->ShowImGui();

	resultUI_->ShowImGui();

	guideUI_->ShowImGui();

	whiteFade_->ShowImGui();

	goal_->ShowImGui();

	lightManager_->ShowImGui();
}

void GamePlayScene::CheckAllCollisions() {

	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに追加
	collisionManager_->AddCollider(player_->GetCollider());

	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy->GetCollider());
	}

	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		collisionManager_->AddCollider(bullet->GetCollider());
	}

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		collisionManager_->AddCollider(bullet->GetCollider());
	}

	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		collisionManager_->AddCollider(obstacle->GetCollider());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}

void GamePlayScene::ChangeState(std::unique_ptr<IPlayState> newState) {

	// 状態を変更
	state_ = std::move(newState);

	// 新しい状態の初期化
	state_->Initialize(this);
}

void GamePlayScene::AddPlayerBullet(std::unique_ptr<Bullet> bullet) {

	// 弾をリストに登録
	playerBullets_.push_back(std::move(bullet));
}

void GamePlayScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet) {

	// 弾をリストに登録
	enemyBullets_.push_back(std::move(bullet));
}

void GamePlayScene::UpdateListObjects() {

	// デスフラグの立った敵を削除
	for (auto ite = enemies_.begin(); ite != enemies_.end(); ) {

		if ((*ite)->IsDead()) {

			// 敵を削除
			ite = enemies_.erase(ite);
		}
		else {

			// プレイヤーを敵にセット
			(*ite)->SetPlayer(player_.get());

			// 敵更新
			(*ite)->Update();

			// 次の敵へ
			++ite;
		}
	}

	// デスフラグが立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });

	// 弾の更新
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {

		bullet->Update();
	}

	// デスフラグが立った敵の弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });

	// 敵の弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	// 障害物の更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {

		obstacle->Update();
	}
}

void GamePlayScene::OnPlayerDamaged(uint16_t currentHP) {

	if (auto playState = dynamic_cast<PlayState*>(state_.get())) {

		playState->OnPlayerDamaged(currentHP);
	}
}

void GamePlayScene::OnEnemyDefeated() {

	if (auto playState = dynamic_cast<PlayState*>(state_.get())) {

		playState->OnEnemyDefeated();
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

	enemies_.clear();
	playerBullets_.clear();
	enemyBullets_.clear();
	obstacles_.clear();

	particleManager_->Clear();

	/// ===== 進行度のリセット ===== ///

	killCount_ = 0;
	normaUI_->SetCurrentValue(0);

	/// ===== 初期化 ===== ///

	cameraController_->Initialize();

	player_->Initialize();
	player_->GetWorldTransform().SetTranslate({ 0.0f, 5.0f, 0.0f });
	player_->SetMoveSpeedAuto(6.0f);

	goal_->Initialize();

	ruleUI_->Initialize();

	/// ========== レベルのリセット ========== ///
	LoadLevelAndApply();

	/// ===== フェードのリセット ===== ///

	whiteFade_->StartFadeAnimation(WhiteFade::FadeType::Out);

	/// ===== 状態の更新 ===== ///

	// ステートを破棄
	state_.reset();
	pauseState_.reset();

	// Introから再スタート
	ChangeState(std::make_unique<IntroState>());
}

void GamePlayScene::LoadLevelAndApply() {

	levelLoader_.LoadLevel(kLevelDataFileName_);
	const GameLevelData& levelData = levelLoader_.GetLevelData();

	// プレイヤー開始位置（※メンバ名は必ず統一）
	player_->GetWorldTransform().SetTranslate(levelData.playerPosition);

	// 敵生成
	SpawnEnemiesFromLevelData(levelData);

	// 障害物生成
	SpawnObstaclesFromLevelData(levelData);
}

void GamePlayScene::SpawnEnemiesFromLevelData(const GameLevelData& levelData) {

	enemies_.clear();

	for (const EnemySpawnData& spawn : levelData.enemySpawnDatas) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetEnemyType(spawn.type);
		enemy->Initialize();
		enemy->GetWorldTransform().SetTranslate(spawn.position);
		enemy->SetGamePlayScene(this);
		enemies_.push_back(std::move(enemy));
	}
}

void GamePlayScene::SpawnObstaclesFromLevelData(const GameLevelData& levelData) {

	obstacles_.clear();

	for (const ObstacleSpawnData& spawn : levelData.obstacleSpawnDatas) {
		std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
		obstacle->Initialize();
		obstacle->GetWorldTransform().SetScale(spawn.scale);
		obstacle->GetWorldTransform().SetTranslate(spawn.position);
		obstacles_.push_back(std::move(obstacle));
	}
}

void GamePlayScene::CheckOriginShift() {

	float playerZ = player_->GetWorldTransform().GetWorldPosition().z;

	// プレイヤーのZ座標がループ距離を超えたら
	if (playerZ >= kLoopDistance) {

		float shiftZ = kLoopDistance - 100;

		// ワールドを手前にずらす
		ShiftWorld(shiftZ);
	}
}

void GamePlayScene::ShiftWorld(float shiftZ) {

	worldShiftZ_ += shiftZ;

	// カメラを手前にずらす
	cameraController_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });

	// プレイヤーを手前にずらす
	player_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });

	// 敵を手前にずらす
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 弾を手前にずらす
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		bullet->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 敵の弾を手前にずらす
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 障害物を手前にずらす
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// ゴールを手前にずらす
	goal_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	goal_->GetGateWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
}
