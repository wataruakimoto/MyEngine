#include "Framework.h"
#include "base/SrvManager.h"
#include "debug/ImGuiManager.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "2d/SpriteCommon.h"
#include "3d/Object3dCommon.h"
#include "3d/ModelCommon.h"
#include "3d/ModelManager.h"
#include "3d/Particle/ParticleCommon.h"
#include "3D/Skybox/SkyboxCommon.h"
#include "scene/SceneManager.h"

void Framework::Initialize() {

	// WindowsAPIの初期化
	winApp = std::make_unique <WinApp>();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	// DirectX初期化
	dxUtility = std::make_unique <DirectXUtility>();
	dxUtility->Initialize();

	// SrVマネージャ初期化
	SrvManager::GetInstance()->Initialize(dxUtility.get());

	// カメラ初期化
	camera = std::make_unique <Camera>();

	// レンダーテクスチャ初期化
	postEffect = std::make_unique <PostEffect>();
	postEffect->Initialize(dxUtility.get());

	// スワップチェイン初期化
	swapChain = std::make_unique <SwapChain>();
	swapChain->Initialize(winApp.get(), dxUtility.get());

	// ImGuiの初期化
	ImGuiManager::GetInstance()->Initialize(winApp.get(), dxUtility.get(), swapChain.get());

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// 入力の初期化
	Input::GetInstance()->Initialize(winApp.get());

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxUtility.get());

	// ポストエフェクトパイプラインの初期化
	filter = std::make_unique<DissolveFilter>();
	filter->Initialize(dxUtility.get(), postEffect.get());
	filter->SetMaskTextureFilePath("Resources", "noise0.png");

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxUtility.get());

	// スプライト共通部初期化
	SpriteCommon::GetInstance()->Initialize(dxUtility.get());

	// 3Dオブジェクト共通部初期化
	Object3dCommon::GetInstance()->Initialize(dxUtility.get());
	// 3Dオブジェクトのデフォルトカメラにカメラをセット
	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	// モデル基盤初期化
	ModelCommon::GetInstance()->Initialize(dxUtility.get());

	// パーティクル基盤初期化
	ParticleCommon::GetInstance()->Initialize(dxUtility.get());
	ParticleCommon::GetInstance()->SetDefaultCamera(camera.get());

	// Skybox共通部初期化
	SkyboxCommon::GetInstance()->Initialize(dxUtility.get());
	// Skyboxのデフォルトカメラにカメラをセット
	SkyboxCommon::GetInstance()->SetDefaultCamera(camera.get());
}

void Framework::Update() {

	// 入力の更新
	Input::GetInstance()->Update();

	// カメラの更新
	camera->Update();
}

void Framework::Finalize() {

	// Skybox共通部の終了
	SkyboxCommon::GetInstance()->Finalize();

	// パーティクル基盤の終了
	ParticleCommon::GetInstance()->Finalize();

	// モデル基盤の解放
	ModelCommon::GetInstance()->Finalize();

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

	// シーンマネージャ終了
	SceneManager::GetInstance()->Finalize();

	// ImGuiの終了
	ImGuiManager::GetInstance()->Finalize();

	// Srvマネージャの終了
	SrvManager::GetInstance()->Finalize();

	// DirectX機能の終了
	dxUtility->Finalize();
	
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