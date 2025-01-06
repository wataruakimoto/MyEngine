#include "Framework.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
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

	// ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// 入力の初期化
	Input::GetInstance()->Initialize(winApp);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxCommon);

	// スプライト共通部初期化
	SpriteCommon::GetInstance()->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	Object3dCommon::GetInstance()->Initialize(dxCommon);

	// モデル基盤初期化
	ModelCommon::GetInstance()->Initialize(dxCommon);
}

void Framework::Update() {

	// 入力の更新
	Input::GetInstance()->Update();
}

void Framework::Finalize() {

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
	imGuiManager->Finalize();
	// ImGuiの解放
	delete imGuiManager;

	// DirectXの解放
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