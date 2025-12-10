#include "GamePlayScene.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteCommon.h"
#include "Object/Object3dCommon.h"
#include "Skybox/SkyboxCommon.h"
#include "Vector3.h"
#include "SceneManager.h"
#include "CameraControll/FollowCamera/FollowCameraController.h"
#include "CameraControll/RailCamera/RailCameraController.h"
#include "Easing.h"

#include <imgui.h>

using namespace Easing;

void GamePlayScene::Initialize() {

	// カメラの生成&初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetFarClip(950.0f); // ファークリップを950に設定

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->Initialize();
	cameraController_->SetCamera(camera_.get());

	// カメラの設定
	SkyboxCommon::GetInstance()->SetDefaultCamera(camera_.get());
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_.get());

	particleManager_->SetCamera(camera_.get());

	// 衝突マネージャの初期化
	collisionManager_ = std::make_unique<CollisionManager>();

	// プレイヤーの生成&初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();
	player_->SetGamePlayScene(this);
	player_->SetPlayerState(PlayerState::AutoPilot); // オートパイロットに設定
	player_->SetCamera(camera_.get());

	// キャストし追従カメラの方を呼び出す
	dynamic_cast<FollowCameraController*>(cameraController_.get())->SetTarget(&player_->GetWorldTransform());
	//player->GetWorldTransform().SetParent(&cameraController_->GetWorldTransform());

	// 敵の生成
	LoadEnemyPopData();

	// 3Dレティクルの生成
	reticle3D_ = std::make_unique<Reticle3D>();
	reticle3D_->Initialize();
	// レティクルのカメラ設定
	reticle3D_->SetCamera(camera_.get());

	// プレイヤーにレティクルを設定
	player_->SetReticle3D(reticle3D_.get());

	// 2Dレティクルの生成
	reticle2D_ = std::make_unique<Reticle2D>();
	reticle2D_->Initialize();
	// カメラを2Dレティクルに設定
	reticle2D_->SetCamera(camera_.get());

	// 3Dレティクルに2Dレティクルを設定
	reticle3D_->SetReticle2D(reticle2D_.get());

	// ロックオンの生成
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize();
	// 自機をロックオンに設定
	lockOn_->SetPlayer(player_.get());
	// カメラをロックオンに設定
	lockOn_->SetCamera(camera_.get());
	// 2Dレティクルをロックオンに設定
	lockOn_->SetReticle2D(reticle2D_.get());

	// プレイヤーにロックオンを設定
	player_->SetLockOn(lockOn_.get());

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

	// ラジアルブラーをフィルターマネージャから受け取っとく
	radialBlurFilter_ = filterManager_->GetRadialBlurFilter();

	// ビネットフィルターをフィルターマネージャから受け取っとく
	vignetteFilter_ = filterManager_->GetVignetteFilter();

	// プレイヤーのHPをもらう
	previousHP_ = player_->GetHP();

	// ルールUIの生成&初期化
	ruleUI_ = std::make_unique<RuleUI>();
	ruleUI_->Initialize();

	// ノルマUIの生成&初期化
	normaUI_ = std::make_unique<NormaUI>();
	normaUI_->Initialize();
	// ノルマUIに目標値を設定
	normaUI_->SetTargetValue(kClearNorma_);
	// ノルマUIに現在値を設定
	normaUI_->SetCurrentValue(0);

	// リザルトUIの生成&初期化
	resultUI_ = std::make_unique<ResultUI>();
	resultUI_->Initialize();

	// 白フェードの初期化
	whiteFade_ = std::make_unique<WhiteFade>();
	whiteFade_->Initialize();
	whiteFade_->SetFadeDuration(3.0f); // フェード時間を3秒に設定

	// 黒フェードの初期化
	blackFade_ = std::make_unique<BlackFade>();
	blackFade_->Initialize();
	blackFade_->SetFadeDuration(3.0f); // フェード時間を3秒に設定

	// 状態リクエストに減速を設定
	stateRequest_ = PlayFlowState::SpeedDown;

	// ゴールの生成&初期化
	goal_ = std::make_unique<Goal>();
	goal_->Initialize();

	// ゴールラインを設定
	goalLineZ_ = goal_->GetWorldTransform().GetTranslate().z;
}

void GamePlayScene::Update() {

	// 状態の変更がリクエストされていたら
	if (stateRequest_) {

		// 状態を変更
		playFlowState_ = stateRequest_.value();

		// 各状態を初期化
		switch (playFlowState_) {

		case PlayFlowState::SpeedDown:
			SpeedDownInitialize();
			break;

		case PlayFlowState::ShowUI:
			ShowUIInitialize();
			break;

		case PlayFlowState::Play:
			PlayInitialize();
			break;

		case PlayFlowState::Result:
			ResultInitialize();
			break;

		case PlayFlowState::WhiteFade:
			WhiteFadeInitialize();
			break;

		case PlayFlowState::BlackFade:
			BlackFadeInitialize();
			break;

		default:
			break;
		}

		// リクエストをクリア
		stateRequest_ = std::nullopt;
	}

	// 各状態の更新
	switch (playFlowState_) {

	case PlayFlowState::SpeedDown:
		SpeedDownUpdate();
		break;

	case PlayFlowState::ShowUI:
		ShowUIUpdate();
		break;

	case PlayFlowState::Play:
		PlayUpdate();
		break;

	case PlayFlowState::Result:
		ResultUpdate();
		break;

	case PlayFlowState::WhiteFade:
		WhiteFadeUpdate();
		break;

	case PlayFlowState::BlackFade:
		BlackFadeUpdate();
		break;

	default:
		break;
	}

	// デスフラグの立った敵を削除
	for (auto ite = enemies_.begin(); ite != enemies_.end(); ) {

		if ((*ite)->IsDead()) {

			// 敵を削除
			ite = enemies_.erase(ite);

			// 倒した数をカウントアップ
			killCount++;
		}
		else {

			// 次の敵へ
			++ite;
		}
	}

	// デスフラグが立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });

	// デスフラグが立った敵の弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });

	// カメラコントローラの更新
	cameraController_->Update();

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

	// 3Dレティクルの更新
	reticle3D_->Update();

	// 2Dレティクルの更新
	reticle2D_->Update();

	// ロックオンの更新
	lockOn_->Update();

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

	// ゴールの更新
	goal_->Update();

	// 衝突マネージャの更新
	collisionManager_->Update();

	// 衝突判定と応答
	CheckAllCollisions();

	// パーティクルマネージャの更新
	particleManager_->Update();
}

void GamePlayScene::Draw() {

	/// === スカイボックスの描画準備 === ///
	SkyboxCommon::GetInstance()->SettingDrawing();

	// スカイボックスの描画
	//skyBox_->Draw();

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

	// 衝突マネージャの描画
	collisionManager_->Draw();

	/// === 半透明オブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawingAlpha();

	// ゴールの描画
	goal_->Draw();

	/// === パーティクルの描画準備 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleManager_->Draw();

	/// === UIの描画準備 === ///
	SpriteCommon::GetInstance()->SettingDrawing();

	// TODO: 全てのスプライト個々の描画

	// 2Dレティクルの描画
	reticle2D_->Draw();

	// ロックオンの描画
	lockOn_->Draw();

	// ルールUIの描画
	ruleUI_->Draw();

	// ノルマUIの描画
	normaUI_->Draw();

	// リザルトUIの描画
	resultUI_->Draw();

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

	reticle3D_->ShowImGui();

	reticle2D_->ShowImGui();

	lockOn_->ShowImGui();

	floor_->ShowImGui();

	cylinder_->ShowImGui();

	skyBox_->ShowImGui();

	ruleUI_->ShowImGui();

	normaUI_->ShowImGui();

	resultUI_->ShowImGui();

	whiteFade_->ShowImGui();

	filterManager_->ShowImGui();

	particleManager_->ShowImGui();

	goal_->ShowImGui();

#ifdef USE_IMGUI

	ImGui::Begin("GamePlayScene");
	ImGui::Text("PlayFlowState: %d", static_cast<int>(playFlowState_));
	ImGui::End();

#endif // USE_IMGUI
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

void GamePlayScene::AddPlayerBullet(std::unique_ptr<Bullet> bullet) {

	// 弾をリストに登録
	playerBullets_.push_back(std::move(bullet));
}

void GamePlayScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet) {

	// 弾をリストに登録
	enemyBullets_.push_back(std::move(bullet));
}

void GamePlayScene::OnPlayerDamaged(uint16_t currentHP) {

	// ダメージ時の一時ビネットを開始
	isDamageVignetteActive_ = true;
	damageVignetteTimer_ = 0;

	// ビネットフィルターを有効化
	if (vignetteFilter_) {
		vignetteFilter_->SetIsActive(true);
	}
}

void GamePlayScene::OnEnemyDefeated() {

	// カメラシェイクを開始
	if (cameraController_) {
		dynamic_cast<FollowCameraController*>(cameraController_.get())->StartShake(0.5f, 0.1f);
	}
}

void GamePlayScene::UpdateVignetteEffect() {

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

void GamePlayScene::LoadEnemyPopData() {

	// ファイルを開く
	std::ifstream file;
	file.open("./resources/gameData/enemyPop.csv");
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

			// 敵の生成&初期化
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
			enemy->Initialize();
			enemy->GetWorldTransform().SetTranslate({ x, y, z });
			enemy->SetGamePlayScene(this);

			// ロックオンに敵を登録
			lockOn_->SetEnemy(enemy.get());

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

void GamePlayScene::SpeedDownInitialize() {

	// 移動速度を取得
	playerMoveSpeed_ = 5.0f;

	// プレイヤーの速度を設定
	player_->SetMoveSpeedAuto(playerMoveSpeed_);

	// ブラーの強さを取得
	blurStrength_ = radialBlurFilter_->GetBlurStrength();

	// ブラーの強さを設定
	radialBlurFilter_->SetBlurStrength(blurStrength_);
}

void GamePlayScene::SpeedDownUpdate() {

	// ブラーの中心を計算
	blurCenter_.x = player_->GetScreenPos().x / WinApp::kClientWidth;
	blurCenter_.y = player_->GetScreenPos().y / WinApp::kClientHeight;

	// ブラーの中心を設定
	radialBlurFilter_->SetCenter(blurCenter_);

	// ブラーの強さが0に達していなければ
	if (blurStrength_ > 0.0f) {

		// ブラーの強さを徐々に減らす
		blurStrength_ -= 0.001f;
	}
	else {

		// 0を下回らないようにする
		blurStrength_ = 0.0f;
		
		// ラジアルブラーをオフにする
		radialBlurFilter_->SetIsActive(false);
	}

	// ブラーの強さを設定
	radialBlurFilter_->SetBlurStrength(blurStrength_);

	// プレイヤーの速度を徐々に落とす
	if (player_->GetMoveSpeedTitle() > 0.0f) {

		playerMoveSpeed_ -= 0.02f;
	}
	else {

		playerMoveSpeed_ = 0.0f;
	}

	// プレイヤーの速度を設定
	player_->SetMoveSpeedAuto(playerMoveSpeed_);

	// プレイヤーの速度が0.5fで ブラーがオフだったら
	if (playerMoveSpeed_ == 0.0f && radialBlurFilter_->GetIsActive() == false) {

		// 状態をプレイに変更
		stateRequest_ = PlayFlowState::ShowUI;
	}
}

void GamePlayScene::ShowUIInitialize() {

	// 警告UIのバウンスアニメーション開始
	ruleUI_->StartBounceAnimation();
}

void GamePlayScene::ShowUIUpdate() {

	// 警告UIの更新
	ruleUI_->Update();

	// エンターキーが押されたら
	if(ruleUI_->IsAnimationFinished()) {

		// 状態をプレイに変更
		stateRequest_ = PlayFlowState::Play;
	}
}

void GamePlayScene::PlayInitialize() {

	// プレイヤーモードをゲームプレイに変更
	player_->SetPlayerState(PlayerState::Manual);
}

void GamePlayScene::PlayUpdate() {

	// ビネットエフェクトの更新
	UpdateVignetteEffect();

	// ノルマUIに目標値を設定
	normaUI_->SetTargetValue(kClearNorma_);
	// ノルマUIに現在値を設定
	normaUI_->SetCurrentValue(killCount);

	// ノルマUIの更新
	normaUI_->Update();

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

	// ゴールライン到達の判定
	bool isReachedGoalLine = player_->GetWorldTransform().GetWorldPosition().z >= goalLineZ_;

	// プレイヤーのデスフラグを取得
	bool isPlayerDead = player_->IsDead();

	// ゴールラインに到達していたら
	if (isReachedGoalLine) {

		// 状態を結果表示に変更
		stateRequest_ = PlayFlowState::Result;
	}
	// プレイヤーがデスフラグが立っていたら
	else if (isPlayerDead) {
		
		// 黒フェード状態に変更
		stateRequest_ = PlayFlowState::BlackFade;
	}
}

void GamePlayScene::ResultInitialize() {

	// ノルマクリアの判定
	bool isNormaClear = killCount >= kClearNorma_;

	// ノルマクリアしていたら
	if (isNormaClear) {

		// リザルトUIにクリアアニメーションを開始させる
		resultUI_->StartAnimation(ResultType::Clear);
	}
	else {
		
		// リザルトUIにゲームオーバーアニメーションを開始させる
		resultUI_->StartAnimation(ResultType::GameOver);
	}

	// プレイヤーの操作を無効化
	player_->SetPlayerState(PlayerState::AutoPilot);
	player_->SetMoveSpeedAuto(0.5f);
}

void GamePlayScene::ResultUpdate() {

	// リザルトUIの更新
	resultUI_->Update();

	// リザルトUIのアニメーションが完了していたら
	if (resultUI_->IsAnimationFinished()) {

		// ノルマクリアの判定
		bool isNormaClear = killCount >= kClearNorma_;

		// ノルマクリアしていたら
		if (isNormaClear) {

			// 状態を白フェードに変更
			stateRequest_ = PlayFlowState::WhiteFade;
		}
		else {

			// 状態を黒フェードに変更
			stateRequest_ = PlayFlowState::BlackFade;
		}
	}
}

void GamePlayScene::WhiteFadeInitialize() {

	// 白フェードアニメーション開始
	whiteFade_->StartFadeAnimation();
}

void GamePlayScene::WhiteFadeUpdate() {

	// 白フェードの更新
	whiteFade_->Update();

	// フェードが完了していたら
	if (whiteFade_->IsFadeFinished()) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
}

void GamePlayScene::BlackFadeInitialize() {

	// ビネットフィルターをオフにする
	if (vignetteFilter_) {
		vignetteFilter_->SetIsActive(false);
	}

	// プレイヤーが生存していたら
	if (!player_->IsDead()) {

		// プレイヤーの操作を無効化
		player_->SetPlayerState(PlayerState::AutoPilot);
		player_->SetMoveSpeedAuto(0.5f);
	}

	// 黒フェードアニメーション開始
	blackFade_->StartFadeAnimation();
}

void GamePlayScene::BlackFadeUpdate() {

	// 黒フェードの更新
	blackFade_->Update();

	// フェードが完了していたら
	if (blackFade_->IsFadeFinished()) {
		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("OVER");
	}
}
