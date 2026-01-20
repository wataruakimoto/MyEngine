#include "Framework.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "ImGuiManager.h"
#include "AudioManager.h"
#include "Input.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteCommon.h"
#include "Object/Object3dCommon.h"
#include "Model/ModelManager.h"
#include "Particle/ParticleCommon.h"
#include "Skybox/SkyboxCommon.h"
#include "SceneManager.h"

void Framework::Initialize() {

	// WindowsAPIの初期化
	winApp = std::make_unique <WinApp>();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	// DirectX初期化
	DirectXUtility::GetInstance()->Initialize();

	// SrVマネージャ初期化
	SrvManager::GetInstance()->Initialize();

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// 入力の初期化
	Input::GetInstance()->Initialize(winApp.get());

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize();

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize();

	// スプライト共通部初期化
	SpriteCommon::GetInstance()->Initialize();

	// 3Dオブジェクト共通部初期化
	Object3dCommon::GetInstance()->Initialize();

	// パーティクル基盤初期化
	ParticleCommon::GetInstance()->Initialize();

	// Skybox共通部初期化
	SkyboxCommon::GetInstance()->Initialize();

	// スワップチェイン初期化
	swapChain = std::make_unique <SwapChain>();
	swapChain->Initialize(winApp.get());

	// シーンバッファ初期化
	sceneBuffer = std::make_unique<SceneBuffer>();
	sceneBuffer->Initialize();

	// ポストプロセスバッファ初期化
	postProcessBuffer = std::make_unique<PostProcessBuffer>();
	postProcessBuffer->Initialize();

	// フィルターマネージャ初期化
	FilterManager::GetInstance()->Initialize();

	// ImGuiの初期化
	ImGuiManager::GetInstance()->Initialize(winApp.get(), swapChain.get());
}

void Framework::Update() {

	// 入力の更新
	Input::GetInstance()->Update();
}

void Framework::Finalize() {

	// シーンマネージャ終了
	SceneManager::GetInstance()->Finalize();

	// フィルターマネージャの終了
	FilterManager::GetInstance()->Finalize();

	// ImGuiの終了
	ImGuiManager::GetInstance()->Finalize();

	// Skybox共通部の終了
	SkyboxCommon::GetInstance()->Finalize();

	// パーティクル基盤の終了
	ParticleCommon::GetInstance()->Finalize();

	// 3Dオブジェクト共通部の解放
	Object3dCommon::GetInstance()->Finalize();

	// スプライト共通部の解放
	SpriteCommon::GetInstance()->Finalize();

	// モデルマネージャの終了
	ModelManager::GetInstance()->Finalize();

	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();

	// 入力の終了
	Input::GetInstance()->Finalize();

	// オーディオマネージャ終了
	AudioManager::GetInstance()->Finalize();

	// Srvマネージャの終了
	SrvManager::GetInstance()->Finalize();

	// DirectX機能の終了
	DirectXUtility::GetInstance()->Finalize();
	
	// WindowsAPIの終了処理
	winApp->Finalize();
}

void Framework::Run() {

	// 初期化
	Initialize();

	// ゲームループ
	while (true) {

		// 更新
		Update();

		// 終了リクエストが来たらループから抜ける
		if (IsEndRequest()) {
			break;
		}

		// 描画
		Draw();
	}

	// 終了
	Finalize();
}