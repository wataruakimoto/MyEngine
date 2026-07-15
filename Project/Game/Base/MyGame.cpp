#include "MyGame.h"
#include "DirectXUtility.h"
#include "OffscreenRendering/FilterManager.h"
#include "ImGuiManager.h"
#include "Particle/ParticleManager.h"
#include "SceneManager.h"
#include "SceneFactory.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Model/ModelManager.h"
#include "LineRenderer.h"
#include "LineManager.h"
#include "TransitionManager.h"

using namespace Engine;

void MyGame::Initialize() {

	// エンジン層の初期化
	Framework::Initialize();

	LoadAllResource();

	// シーンファクトリーを生成
	sceneFactory_ = std::make_unique <SceneFactory>();
	sceneManager_->SetSceneFactory(sceneFactory_.get());

	// シーンマネージャに最初のシーンをセット
#ifdef _DEBUG
	sceneManager_->ChangeScene("DEBUG");
#else
	sceneManager_->ChangeScene("TITLE");
#endif

	// パーティクルマネージャの初期化
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize();

	// 遷移マネージャのインスタンス取得
	transitionManager_ = TransitionManager::GetInstance();
}

void MyGame::Update() {

	// エンジン層の更新
	Framework::Update();

	// 遷移マネージャの更新
	transitionManager_->Update();

#ifdef _DEBUG

	// Tabキーが押されたら
	if (input_->TriggerKey(VK_TAB)) {

		// エディットモード切り替え
		isEditMode_ = !isEditMode_;
	}

#endif // _DEBUG
}

void MyGame::Draw() {

	/// ========== ゲームシーンの描画開始 ========== ///

	/// ===== フィルター適応のある描画 ===== ///

	sceneBuffer->PreDrawFiltered();

	sceneManager_->DrawFiltered();

	sceneBuffer->PostDraw();

	filterManager_->Draw(sceneBuffer.get(), postProcessBuffer.get());

	/// ===== フィルター適応のない描画 ===== ///

	sceneBuffer->PreDrawUnfiltered();

	sceneManager_->DrawUnfiltered();

	// 線描画の設定
	lineRenderer_->SettingDrawing();

#ifdef _DEBUG

	// 線描画
	//lineManager_->Render();

#endif // _DEBUG

	sceneBuffer->PostDraw();

	/// ========== ゲームシーンの描画終了 ========== ///

	/// ========== 画面への描画開始 ========== ///

	swapChain->PreDraw();

	// エディットモードの場合
	if (isEditMode_) {

		imguiManager_->Draw();
	}
	// エディットモードでない場合
	else {

		filterManager_->DrawTexture(sceneBuffer->GetSrvIndex());
	}

	/// ========== 画面への描画終了 ========== ///

	swapChain->PostDraw();

	// DirectXユーティリティの描画後処理
	dxUtility_->PostDraw();
}

void MyGame::Finalize() {

	// 遷移マネージャの終了
	transitionManager_->Finalize();

	// パーティクルマネージャの終了
	particleManager_->Finalize();

	// 基底クラス解放
	Framework::Finalize();
}

void MyGame::ShowImGui() {

	Framework::ShowImGui();

	particleManager_->ShowImGui();

	transitionManager_->ShowImGui();
}

void MyGame::LoadAllResource() {

	/// ========== テクスチャの読み込み ========== ///

	textureManager_->LoadTexture("BlackScreen.png");
	textureManager_->LoadTexture("start.png");
	textureManager_->LoadTexture("title.png");
	textureManager_->LoadTexture("White1280x720.png");
	textureManager_->LoadTexture("Black1280x720.png");
	textureManager_->LoadTexture("LockOn.png");
	textureManager_->LoadTexture("2DReticle.png");
	textureManager_->LoadTexture("Rule/Rule.png");
	textureManager_->LoadTexture("Rule/Operation.png");
	textureManager_->LoadTexture("Norma/NormaText.png");
	textureManager_->LoadTexture("Norma/Slash.png");
	textureManager_->LoadTexture("Numbers.png");
	textureManager_->LoadTexture("Result/Clear.png");
	textureManager_->LoadTexture("Result/GameOver.png");
	textureManager_->LoadTexture("GameClear.png");
	textureManager_->LoadTexture("GameOver.png");
	textureManager_->LoadTexture("White1x1.png");
	textureManager_->LoadTexture("PauseUI/ResumeButton.png");
	textureManager_->LoadTexture("PauseUI/RestartButton.png");
	textureManager_->LoadTexture("PauseUI/QuitButton.png");
	textureManager_->LoadTexture("PauseUI/Frame.png");
	textureManager_->LoadTexture("SpaceToTitle.png");
	textureManager_->LoadTexture("Particles/gradationLine.png");

	/// ===== UIテクスチャ ===== ///

	textureManager_->LoadTexture("UI/Guide/Mouse.png");
	textureManager_->LoadTexture("UI/Guide/MouseClick.png");
	textureManager_->LoadTexture("UI/Guide/ButtonA.png");
	textureManager_->LoadTexture("UI/Guide/ButtonD.png");
	textureManager_->LoadTexture("UI/Guide/PushA.png");
	textureManager_->LoadTexture("UI/Guide/PushD.png");
	textureManager_->LoadTexture("UI/Guide/Pause.png");
	textureManager_->LoadTexture("UI/Guide/Back.png");

	// 数字のスプライトを生成 0~9まで
	for (uint32_t i = 0; i < 10; ++i) {
		textureManager_->LoadTexture("UI/Timer/" + std::to_string(i) + ".png");
	}

	textureManager_->LoadTexture("UI/Timer/Colon.png");

	textureManager_->LoadTexture("rostock_laage_airport_4k.dds");

	/// ========== モデルの読み込み ========== ///

	modelManager_->LoadModel("Player/player.obj");
	modelManager_->LoadModel("Cylinder/cylinder.obj");
	modelManager_->LoadModel("Floor/floor.obj");
	modelManager_->LoadModel("PlayerBullet/PlayerBullet.obj");
	modelManager_->LoadModel("Enemy/enemy.obj");
	modelManager_->LoadModel("Enemy/Kamikaze.obj");
	modelManager_->LoadModel("EnemyBullet/EnemyBullet.obj");
	modelManager_->LoadModel("Goal/Goal.obj");
	modelManager_->LoadModel("Gate/Gate.obj");
	modelManager_->LoadModel("Reticle/Reticle.obj");
	modelManager_->LoadModel("Obstacle/Obstacle.obj");
	modelManager_->LoadModel("Sphere/sphere.obj");
	modelManager_->LoadModel("Terrain/terrain.obj");
}
