#include "Framework.h"
#include "audio/AudioManager.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include "debug/D3DResourceLeakChecker.h"

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

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxCommon);


	// スプライト共通部初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	// モデル基盤初期化
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);
}

void Framework::Update() {

	// 入力の更新
	input->Update();
}

void Framework::Finalize() {

	// モデル基盤の解放
	delete modelCommon;

	// 3Dオブジェクト共通部の解放
	delete object3dCommon;

	// スプライト共通部の解放
	delete spriteCommon;

	// モデルマネージャの終了
	ModelManager::GetInstance()->Finalize();

	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();

	// オーディオマネージャ終了
	AudioManager::GetInstance()->Finalize();

	// 入力の解放
	delete input;

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