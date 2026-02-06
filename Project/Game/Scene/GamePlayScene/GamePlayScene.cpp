#include "GamePlayScene.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteCommon.h"
#include "Object/Object3dCommon.h"
#include "Skybox/SkyboxCommon.h"
#include "Vector3.h"
#include "SceneManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "CameraControll/RailCamera/RailCameraController.h"
#include "Easing.h"
#include "WinApp.h"

#include <imgui.h>

using namespace Engine;
using namespace Easing;

void GamePlayScene::Initialize() {

	// カメラの生成&初期化
	camera_ = std::make_unique<Engine::Camera>();
	camera_->Initialize();
	camera_->SetFarClip(950.0f); // ファークリップを950に設定

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->Initialize();
	cameraController_->SetCamera(camera_.get());

	// カメラの設定
	SkyboxCommon::GetInstance()->SetDefaultCamera(camera_.get());
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_.get());
	FilterManager::GetInstance()->SetCamera(camera_.get());

	particleManager_->SetCamera(camera_.get());

	// 衝突マネージャの初期化
	collisionManager_ = std::make_unique<CollisionManager>();

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

	// スカイボックスの生成
	skyBox_ = std::make_unique<SkyBoxGame>();
	skyBox_->Initialize();
	// カメラを設定
	skyBox_->SetCamera(camera_.get());
	// プレイヤーを設定
	skyBox_->SetPlayer(player_.get());

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

	// 敵発生データの読み込み
	LoadEnemyPopData();

	// 初期状態をイントロに設定
	ChangeState(std::make_unique<IntroState>());
}

void GamePlayScene::Update() {

	// カメラコントローラの更新
	cameraController_->Update();

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
	Object3dCommon::GetInstance()->SettingDrawingOpaque();

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

	// プレイヤー描画
	player_->Draw();

	/// === 半透明オブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawingAlpha();

	// ゴールの描画
	goal_->Draw();

	/// === パーティクルの描画準備 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleManager_->Draw();
}

void GamePlayScene::DrawUnfiltered() {

	/// === UIの描画準備 === ///
	SpriteCommon::GetInstance()->SettingDrawing();

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

	floor_->ShowImGui();

	cylinder_->ShowImGui();

	skyBox_->ShowImGui();

	ruleUI_->ShowImGui();

	normaUI_->ShowImGui();

	resultUI_->ShowImGui();

	guideUI_->ShowImGui();

	whiteFade_->ShowImGui();

	FilterManager::GetInstance()->ShowImGui();

	particleManager_->ShowImGui();

	goal_->ShowImGui();
}

void GamePlayScene::CheckAllCollisions() {

	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに追加
	collisionManager_->AddCollider(player_.get());

	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager_->AddCollider(enemy.get());
	}

	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		collisionManager_->AddCollider(bullet.get());
	}

	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		collisionManager_->AddCollider(bullet.get());
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

void GamePlayScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/Datas/gameData/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GamePlayScene::UpdateEnemyPopCommands() {

	// 待機処理
	if (isWait_ == true) {

		standbyTimer_--;

		if (standbyTimer_ <= 0) {
			// 待機完了
			isWait_ = false;
		}

		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {

			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 生成する敵の種類
			int type = 0; // デフォルトは0(Normal)
			getline(line_stream, word, ',');
			type = std::atoi(word.c_str()); // 数字に変換

			// 敵の生成&初期化
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			enemy->SetEnemyType(static_cast<EnemyType>(type));
			enemy->Initialize();
			enemy->GetWorldTransform().SetTranslate({ x, y, z });
			enemy->SetGamePlayScene(this);

			// 敵をリストに追加
			enemies_.push_back(std::move(enemy));

		} // WAITコマンド
		else if (word.find("WAIT") == 0) {

			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			isWait_ = true;
			standbyTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
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

	/// ===== 進行度のリセット ===== ///

	killCount_ = 0;
	normaUI_->SetCurrentValue(0);

	// 敵発生データの読み込み
	LoadEnemyPopData();

	/// ===== 初期化 ===== ///

	cameraController_->Initialize();

	player_->Initialize();
	player_->GetWorldTransform().SetTranslate({ 0.0f, 5.0f, 0.0f });
	player_->SetMoveSpeedAuto(6.0f);

	goal_->Initialize();

	ruleUI_->Initialize();

	/// ===== フェードのリセット ===== ///

	whiteFade_->StartFadeAnimation(WhiteFade::FadeType::Out);

	/// ===== 状態の更新 ===== ///

	// ステートを破棄
	state_.reset();
	pauseState_.reset();

	// Introから再スタート
	ChangeState(std::make_unique<IntroState>());
}

void GamePlayScene::CheckOriginShift() {

	float playerZ = player_->GetWorldTransform().GetWorldPosition().z;

	// プレイヤーのZ座標がループ距離を超えたら
	if (playerZ >= kLoopDistance) {

		// ワールドを手前にずらす
		ShiftWorld(kLoopDistance);
	}
}

void GamePlayScene::ShiftWorld(float shiftZ) {

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
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// ゴールを手前にずらす
	goal_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	goal_->GetGateWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
}
