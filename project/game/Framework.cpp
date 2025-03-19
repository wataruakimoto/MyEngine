#include "Framework.h"
#include "base/SrvManager.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "camera/DebugCamera.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include "3d/ParticleCommon.h"
#include "debug/D3DResourceLeakChecker.h"
#include "scene/SceneManager.h"

void Framework::Initialize() {

	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	// DirectX初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	// SrVマネージャ初期化
	SrvManager::GetInstance()->Initialize(dxCommon);

	// ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// 入力の初期化
	Input::GetInstance()->Initialize(winApp);

	// デバッグカメラ初期化
	debugCamera = new DebugCamera();

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxCommon);

	// スプライト共通部初期化
	SpriteCommon::GetInstance()->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	Object3dCommon::GetInstance()->Initialize(dxCommon);
	// 3Dオブジェクトのデフォルトカメラにデバッグカメラをセット
	Object3dCommon::GetInstance()->SetDefaultCamera(debugCamera);

	// モデル基盤初期化
	ModelCommon::GetInstance()->Initialize(dxCommon);

	// パーティクル基盤初期化
	ParticleCommon::GetInstance()->Initialize(dxCommon);
	ParticleCommon::GetInstance()->SetDefaultCamera(debugCamera);
}

void Framework::Update() {

	// 入力の更新
	Input::GetInstance()->Update();

	// デバッグカメラの更新
	debugCamera->Update();
}

void Framework::Finalize() {

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

	// デバッグカメラの解放
	delete debugCamera;

	// 入力の終了
	Input::GetInstance()->Finalize();

	// オーディオマネージャ終了
	AudioManager::GetInstance()->Finalize();

	// シーンマネージャ終了
	SceneManager::GetInstance()->Finalize();

	// シーンファクトリーの解放
	delete sceneFactory_;

	// ImGuiの終了
	imGuiManager->Finalize();
	// ImGuiの解放
	delete imGuiManager;

	// SrVマネージャの終了
	SrvManager::GetInstance()->Finalize();

	// DirectXの解放
	dxCommon->Finalize();
	delete dxCommon;

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPIの解放
	delete winApp;

	// リークチェック
	D3DResourceLeakChecker leakCheck;
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