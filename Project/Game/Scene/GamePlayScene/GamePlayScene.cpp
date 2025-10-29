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

#include <imgui.h>

void GamePlayScene::Initialize() {

	// カメラの生成&初期化
	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	// カメラコントローラーの生成&初期化
	cameraController_ = std::make_unique<FollowCameraController>();
	cameraController_->Initialize();
	cameraController_->SetCamera(camera_.get());

	// カメラの設定
	SkyboxCommon::GetInstance()->SetDefaultCamera(camera_.get());
	Object3dCommon::GetInstance()->SetDefaultCamera(camera_.get());

	// パーティクルシステムの初期化
	particleSystem->SetCamera(camera_.get());
	particleSystem->CreateParticleGroup("circle2", "Resources/circle2.png", ParticleType::PLANE);
	particleSystem->CreateParticleGroup("Red", "Resources/Red.png", ParticleType::CUBE);
	particleSystem->CreateParticleGroup("Blue", "Resources/Blue.png", ParticleType::CUBE);

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

	// 警告UIの生成&初期化
	warningUI_ = std::make_unique<WarningUI>();
	warningUI_->Initialize();

	// 状態リクエストに減速を設定
	stateRequest_ = PlayFlowState::SpeedDown;
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

	default:
		break;
	}

	// デスフラグの立った敵を削除
	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) {return enemy->IsDead(); });

	// デスフラグが立った弾を削除
	playerBullets_.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });

	// デスフラグが立った敵の弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {return bullet->IsDead(); });

	// プレイヤーが死んでいたら
	if (player_->IsDead()) {

		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	if (player_->GetCenterPosition().z >= 1700.0f) {

		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

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

	// 衝突マネージャの更新
	collisionManager_->Update();

	// パーティクルシステムの更新
	particleSystem->Update();

	// 衝突判定と応答
	CheckAllCollisions();
}

void GamePlayScene::Draw() {

	/// === スカイボックスの描画準備 === ///
	SkyboxCommon::GetInstance()->SettingDrawing();

	// スカイボックスの描画
	skyBox_->Draw();

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画

	// シリンダーの描画
	cylinder_->Draw();

	// フロアの描画
	floor_->Draw();

	// プレイヤー描画
	player_->Draw();

	// 弾の描画
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {

		bullet->Draw();
	}

	// 敵の弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {

		bullet->Draw();
	}

	// 敵描画
	for (std::unique_ptr<Enemy>& enemy : enemies_) {

		enemy->Draw();
	}

	// 3Dレティクルの描画
	//reticle3D_->Draw();

	// 衝突マネージャの描画
	collisionManager_->Draw();

	/// === パーティクルの描画準備 === ///
	particleCommon->SettingDrawing();

	// パーティクルシステムの描画
	particleSystem->Draw();

	/// === UIの描画準備 === ///
	SpriteCommon::GetInstance()->SettingDrawing();

	// TODO: 全てのスプライト個々の描画

	// 2Dレティクルの描画
	reticle2D_->Draw();

	// ロックオンの描画
	lockOn_->Draw();

	// 警告UIの描画
	warningUI_->Draw();
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

	warningUI_->ShowImGui();

	filterManager_->ShowImGui();

	particleSystem->ShowImGui("ParticleSystem");

#ifdef _DEBUG

	ImGui::Begin("GamePlayScene");
	ImGui::Text("PlayFlowState: %d", static_cast<int>(playFlowState_));
	ImGui::End();

#endif // _DEBUG

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
	warningUI_->StartBounceAnimation();
}

void GamePlayScene::ShowUIUpdate() {

	// 警告UIの更新
	warningUI_->Update();

	// エンターキーが押されたら
	if(warningUI_->IsAnimationFinished()) {

		// 状態をプレイに変更
		stateRequest_ = PlayFlowState::Play;
	}
}

void GamePlayScene::PlayInitialize() {

	// プレイヤーモードをゲームプレイに変更
	player_->SetPlayerState(PlayerState::Manual);
}

void GamePlayScene::PlayUpdate() {

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();
}
