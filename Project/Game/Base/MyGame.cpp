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

	/// ===== テクスチャの読み込み ===== ///

	textureManager_->LoadTextureRelative("BlackScreen.png");
	textureManager_->LoadTextureRelative("start.png");
	textureManager_->LoadTextureRelative("title.png");
	textureManager_->LoadTextureRelative("White1280x720.png");
	textureManager_->LoadTextureRelative("Black1280x720.png");
	textureManager_->LoadTextureRelative("LockOn.png");
	textureManager_->LoadTextureRelative("2DReticle.png");
	textureManager_->LoadTextureRelative("Rule/Rule.png");
	textureManager_->LoadTextureRelative("Rule/Operation.png");
	textureManager_->LoadTextureRelative("Norma/NormaText.png");
	textureManager_->LoadTextureRelative("Norma/Slash.png");
	textureManager_->LoadTextureRelative("Numbers.png");
	textureManager_->LoadTextureRelative("Result/Clear.png");
	textureManager_->LoadTextureRelative("Result/GameOver.png");
	textureManager_->LoadTextureRelative("GameClear.png");
	textureManager_->LoadTextureRelative("GameOver.png");
	textureManager_->LoadTextureRelative("Guide/Mouse.png");
	textureManager_->LoadTextureRelative("Guide/MouseClick.png");
	textureManager_->LoadTextureRelative("Guide/ButtonA.png");
	textureManager_->LoadTextureRelative("Guide/ButtonD.png");
	textureManager_->LoadTextureRelative("Guide/PushA.png");
	textureManager_->LoadTextureRelative("Guide/PushD.png");
	textureManager_->LoadTextureRelative("Guide/Pause.png");
	textureManager_->LoadTextureRelative("Guide/Back.png");
	textureManager_->LoadTextureRelative("White1x1.png");
	textureManager_->LoadTextureRelative("PauseUI/ResumeButton.png");
	textureManager_->LoadTextureRelative("PauseUI/RestartButton.png");
	textureManager_->LoadTextureRelative("PauseUI/QuitButton.png");
	textureManager_->LoadTextureRelative("PauseUI/Frame.png");
	textureManager_->LoadTextureRelative("SpaceToTitle.png");

	textureManager_->LoadTextureRelative("rostock_laage_airport_4k.dds");

	/// ===== モデルの読み込み ===== ///

	modelManager_->LoadModelData("Player", "player.obj");
	modelManager_->LoadModelData("Cylinder", "cylinder.obj");
	modelManager_->LoadModelData("Floor", "floor.obj");
	modelManager_->LoadModelData("PlayerBullet", "PlayerBullet.obj");
	modelManager_->LoadModelData("Enemy", "enemy.obj");
	modelManager_->LoadModelData("Enemy", "Kamikaze.obj");
	modelManager_->LoadModelData("EnemyBullet", "EnemyBullet.obj");
	modelManager_->LoadModelData("Goal", "Goal.obj");
	modelManager_->LoadModelData("Gate", "Gate.obj");
	modelManager_->LoadModelData("Reticle", "Reticle.obj");
	modelManager_->LoadModelData("Obstacle", "Obstacle.obj");
	modelManager_->LoadModelData("Sphere", "sphere.obj");
	modelManager_->LoadModelData("Terrain", "terrain.obj");
}
