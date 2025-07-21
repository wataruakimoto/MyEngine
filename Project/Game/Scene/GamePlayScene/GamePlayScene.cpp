#include "GamePlayScene.h"

#include "input/Input.h"
#include "2d/TextureManager.h"
#include "2d/SpriteCommon.h"
#include "3d/Object3dCommon.h"
#include "3d/Particle/ParticleCommon.h"
#include "3d/Particle/ParticleSystem.h"
#include "math/Vector3.h"
#include "scene/SceneManager.h"

#include <imgui.h>

void GamePlayScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f, 0.0f, -50.0f });

	// カメラを設定
	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	// レールカメラコントローラーの生成
	railCameraController_ = std::make_unique<RailCameraController>();
	// レールカメラコントローラーの初期化
	railCameraController_->Initialize();

	// パーティクルシステムの初期化
	ParticleSystem::GetInstance()->SetCamera(camera.get());
	ParticleSystem::GetInstance()->CreateParticleGroup("circle2", "Resources/circle2.png", ParticleType::PLANE);

	// 衝突マネージャの初期化
	collisionManager_ = std::make_unique<CollisionManager>();

	// 天球の生成&初期化
	skydome = new Skydome();
	skydome->Initialize();

	// プレイヤーの生成&初期化
	player = new Player();
	player->Initialize();
	player->GetWorldTransform().SetParent(&railCameraController_->GetWorldTransform());

	// 敵の生成
	LoadEnemyPopData();

	// 3Dレティクルの生成
	reticle3D_ = std::make_unique<Reticle3D>();
	reticle3D_->Initialize();
	// レティクルのプレイヤー設定
	reticle3D_->SetPlayer(player);
	// レティクルのカメラ設定
	reticle3D_->SetCamera(camera.get());

	// プレイヤーにレティクルを設定
	player->SetReticle3D(reticle3D_.get());

	// 2Dレティクルの生成
	reticle2D_ = std::make_unique<Reticle2D>();
	reticle2D_->Initialize();
	// 3Dレティクルを2Dレティクルに設定
	reticle2D_->SetReticle3D(reticle3D_.get());
	// カメラを2Dレティクルに設定
	reticle2D_->SetCamera(camera.get());

	// 3Dレティクルに2Dレティクルを設定
	reticle3D_->SetReticle2D(reticle2D_.get());
}

void GamePlayScene::Update() {

	// デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {

		if (enemy->IsDead()) {

			enemy->Finalize();
			delete enemy;

			return true;
		}

		return false;
		});

	// プレイヤーが死んでいたら
	if (player->IsDead()) {

		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("FAIL");
	}

	if (killCount >= 20) {

		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	// ImGuiの表示
	ShowImGui();

	/// === カメラ更新 === ///
	camera->Update();

	// レールカメラコントローラーの更新
	railCameraController_->Update();

	// 天球更新
	skydome->Update();

	// プレイヤー更新
	player->Update();

	// 弾のリストを受け取る
	bullets_ = player->GetBullets();

	// 敵発生コマンドの更新
	UpdateEnemyPopCommands();

	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {

		// プレイヤーを敵にセット
		enemy->SetPlayer(player);

		// 敵更新
		enemy->Update();
	}

	// 3Dレティクルの更新
	reticle3D_->Update();

	// 2Dレティクルの更新
	reticle2D_->Update();

	// 衝突マネージャの更新
	collisionManager_->Update();

	// パーティクルシステムの更新
	ParticleSystem::GetInstance()->Update();

	// 衝突判定と応答
	CheckAllCollisions();
}

void GamePlayScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画

	// 天球描画
	skydome->Draw();

	// プレイヤー描画
	player->Draw();

	// 敵描画
	for (Enemy* enemy : enemies_) {

		enemy->Draw();
	}

	// 3Dレティクルの描画
	reticle3D_->Draw();

	// 衝突マネージャの描画
	collisionManager_->Draw();

	/// === パーティクルの描画準備 === ///
	ParticleCommon::GetInstance()->SettingDrawing();

	// パーティクルシステムの描画
	ParticleSystem::GetInstance()->Draw();

	/// === UIの描画準備 === ///
	SpriteCommon::GetInstance()->SettingDrawing();

	// TODO: 全てのスプライト個々の描画

	// 2Dレティクルの描画
	reticle2D_->Draw();
}

void GamePlayScene::Finalize() {

	// 敵の解放
	for (Enemy* enemy : enemies_) {

		enemy->Finalize();
		delete enemy;
	}

	// プレイヤーの解放
	player->Finalize();
	delete player;

	// 天球の解放
	skydome->Finalize();
	delete skydome;
}

void GamePlayScene::ShowImGui() {

	Input::GetInstance()->ShowImgui();

	ParticleSystem::GetInstance()->ShowImGui("particleSystem");

	camera->ShowImGui("camera");

	railCameraController_->ShowImGui();

	player->ShowImGui();

	for (Enemy* enemy : enemies_) {
		enemy->ShowImGui();
	}

	reticle3D_->ShowImGui();
	reticle2D_->ShowImGui();
}

void GamePlayScene::CheckAllCollisions() {

	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに追加
	collisionManager_->AddCollider(player);

	for (Enemy* enemy : enemies_) {
		collisionManager_->AddCollider(enemy);
	}

	for (Bullet* bullet : bullets_) {
		collisionManager_->AddCollider(bullet);
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
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
			Enemy* enemy = new Enemy();
			enemy->Initialize();
			enemy->GetWorldTransform().SetTranslate({x, y, z});
			enemies_.push_back(enemy);

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
