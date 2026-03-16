#include "Framework.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "Texture/TextureManager.h"
#include "Sprite/SpriteRenderer.h"
#include "Model/ModelManager.h"
#include "Object/Object3dRenderer.h"
#include "Skybox/SkyBoxRenderer.h"
#include "Particle/ParticleRenderer.h"
#include "LineRenderer.h"
#include "OffscreenRendering/FilterManager.h"
#include "ImGuiManager.h"
#include "AudioManager.h"
#include "Input.h"
#include "SceneManager.h"
#include "LineManager.h"

using namespace Engine;

void Framework::Initialize() {

	// WindowsAPIの初期化
	winApp = std::make_unique <WinApp>();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	// DirectXユーティリティの初期化
	dxUtility_ = DirectXUtility::GetInstance();
	dxUtility_->Initialize();

	// Srvマネージャ初期化
	srvManager_ = SrvManager::GetInstance();
	srvManager_->Initialize();

	// テクスチャマネージャ初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize();

	// スプライトレンダラーの初期化
	spriteRenderer_ = SpriteRenderer::GetInstance();
	spriteRenderer_->Initialize();

	// モデルマネージャ初期化
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize();

	// 3Dオブジェクトレンダラー初期化
	object3dRenderer_ = Object3dRenderer::GetInstance();
	object3dRenderer_->Initialize();

	// スカイボックスレンダラー初期化
	skyBoxRenderer_ = SkyBoxRenderer::GetInstance();
	skyBoxRenderer_->Initialize();

	// パーティクルレンダラー初期化
	particleRenderer_ = ParticleRenderer::GetInstance();
	particleRenderer_->Initialize();

	// 線のレンダラー初期化
	lineRenderer_ = LineRenderer::GetInstance();
	lineRenderer_->Initialize();

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
	filterManager_ = FilterManager::GetInstance();
	filterManager_->Initialize();

	// ImGuiの初期化
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(winApp.get(), swapChain.get());

	// オーディオマネージャ初期化
	audioManager_ = AudioManager::GetInstance();
	audioManager_->Initialize();

	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp.get());

	// シーンマネージャのインスタンス取得
	sceneManager_ = SceneManager::GetInstance();

	// 線マネージャの初期化
	lineManager_ = LineManager::GetInstance();
	lineManager_->Initialize();
}

void Framework::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (winApp->ProcessMessage()) {

		// ゲームループを抜ける
		endRequest_ = true;
	}
	else {

		// 入力の更新
		Input::GetInstance()->Update();

		// 線マネージャのリセット
		lineManager_->Clear();

		/// === ImGui開始 === ///
		imguiManager_->Begin();

		// シーンマネージャの更新
		sceneManager_->Update();

		// シーンのImGui表示
		sceneManager_->ShowImGui();

		// シーンビュー作成
		sceneBuffer->CreateSceneView();

		/// === ImGui終了 === ///
		imguiManager_->End();
	}
}

void Framework::Finalize() {

	// 線マネージャの終了
	lineManager_->Finalize();

	// シーンマネージャ終了
	sceneManager_->Finalize();

	// 入力の終了
	input_->Finalize();

	// オーディオマネージャ終了
	audioManager_->Finalize();

	// 線のレンダラーの終了
	lineRenderer_->Finalize();

	// パーティクルレンダラーの終了
	particleRenderer_->Finalize();

	// スカイボックスレンダラーの終了
	skyBoxRenderer_->Finalize();

	// 3Dオブジェクトレンダラーの終了
	object3dRenderer_->Finalize();

	// モデルマネージャの終了
	modelManager_->Finalize();

	// スプライトレンダラーの終了
	spriteRenderer_->Finalize();

	// テクスチャマネージャの終了
	textureManager_->Finalize();

	// ImGuiマネージャの終了
	imguiManager_->Finalize();

	// フィルタマネージャの終了
	filterManager_->Finalize();

	// Srvマネージャの終了
	srvManager_->Finalize();

	// DirectXユーティリティの終了
	dxUtility_->Finalize();

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