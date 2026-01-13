#include "MyGame.h"
#include "DirectXUtility.h"
#include "OffscreenRendering/FilterManager.h"
#include "ImGuiManager.h"
#include "Particle/ParticleCommon.h"
#include "Particle/ParticleManager.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// テクスチャマネージャのインスタンス取得
	textureManager = TextureManager::GetInstance();

	// モデルマネージャのインスタンス取得
	modelManager = ModelManager::GetInstance();

	LoadAllResource();

	// シーンファクトリーを生成
	sceneFactory_ = std::make_unique <SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());

	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

	// パーティクルマネージャの初期化
	ParticleManager::GetInstance()->Initialize();
}

void MyGame::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (winApp->ProcessMessage()) {

		// ゲームループを抜ける
		endRequest_ = true;

	}
	else {

#ifdef _DEBUG

		// Tabキーが押されたら
		if (Input::GetInstance()->TriggerKey(VK_TAB)) {

			// エディットモード切り替え
			isEditMode_ = !isEditMode_;
		}

#endif // _DEBUG

		// 基底クラス更新
		Framework::Update();

		/// === ImGui開始 === ///
		ImGuiManager::GetInstance()->Begin();

		// シーンマネージャの更新
		SceneManager::GetInstance()->Update();

		// シーンのImGui表示
		SceneManager::GetInstance()->ShowImGui();

		// シーンビュー作成
		sceneRenderTexture->CreateSceneView();

		/// === ImGui終了 === ///
		ImGuiManager::GetInstance()->End();
	}
}

void MyGame::Draw() {

	/// ===== ゲームシーンの描画 ===== ///

	// ポストエフェクトの描画前処理
	postEffect->PreDraw();

	// シーンマネージャの描画
	SceneManager::GetInstance()->Draw();

	// ポストエフェクトの描画後処理
	postEffect->PostDraw();

	// エディットモードなら
	if (isEditMode_) {

		/// ===== フィルターへの描画 ===== ///

		// シーンレンダーテクスチャの描画前処理
		sceneRenderTexture->PreDraw();

		// フィルターマネージャの描画
		FilterManager::GetInstance()->Draw();

		// シーンレンダーテクスチャの描画後処理
		sceneRenderTexture->PostDraw();

		/// ===== 画面への描画 ===== ///

		// スワップチェイン描画前処理
		swapChain->PreDraw();

		// ImGuiの描画
		ImGuiManager::GetInstance()->Draw();

		// スワップチェイン描画後処理
		swapChain->PostDraw();
	}
	else {

		/// ===== 画面への描画 ===== ///

		// スワップチェイン描画前処理
		swapChain->PreDraw();

		// フィルターマネージャの描画
		FilterManager::GetInstance()->Draw();

		// スワップチェイン描画後処理
		swapChain->PostDraw();
	}

	// DirectXユーティリティの描画後処理
	DirectXUtility::GetInstance()->PostDraw();
}

void MyGame::Finalize() {

	// パーティクルマネージャの終了
	ParticleManager::GetInstance()->Finalize();

	// 基底クラス解放
	Framework::Finalize();
}

void MyGame::LoadAllResource() {

	/// ===== テクスチャの読み込み ===== ///

	textureManager->LoadTextureRelative("BlackScreen.png");
	textureManager->LoadTextureRelative("start.png");
	textureManager->LoadTextureRelative("title.png");
	textureManager->LoadTextureRelative("White1280x720.png");
	textureManager->LoadTextureRelative("Black1280x720.png");
	textureManager->LoadTextureRelative("LockOn.png");
	textureManager->LoadTextureRelative("2DReticle.png");
	textureManager->LoadTextureRelative("Rule/Rule.png");
	textureManager->LoadTextureRelative("Rule/Operation.png");
	textureManager->LoadTextureRelative("Norma/NormaText.png");
	textureManager->LoadTextureRelative("Norma/Slash.png");
	textureManager->LoadTextureRelative("Numbers.png");
	textureManager->LoadTextureRelative("Result/Clear.png");
	textureManager->LoadTextureRelative("Result/GameOver.png");
	textureManager->LoadTextureRelative("GameClear.png");
	textureManager->LoadTextureRelative("GameOver.png");

	textureManager->LoadTextureRelative("rostock_laage_airport_4k.dds");

	/// ===== モデルの読み込み ===== ///

	modelManager->LoadModelData("Player", "player.obj");
	modelManager->LoadModelData("Cylinder", "cylinder.obj");
	modelManager->LoadModelData("Floor", "floor.obj");
	modelManager->LoadModelData("PlayerBullet", "PlayerBullet.obj");
	modelManager->LoadModelData("Enemy", "enemy.obj");
	modelManager->LoadModelData("Enemy", "Kamikaze.obj");
	modelManager->LoadModelData("EnemyBullet", "EnemyBullet.obj");
	modelManager->LoadModelData("Goal", "Goal.obj");
	modelManager->LoadModelData("Gate", "Gate.obj");
	modelManager->LoadModelData("Reticle", "Reticle.obj");
}
