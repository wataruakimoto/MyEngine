#include "Framework.h"
#include "base/SrvManager.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "camera/DebugCamera.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include "3d/ParticleCommon.h"
#include "debug/ImGuiManager.h"
#include "scene/SceneManager.h"

void Framework::Initialize() {

	// WindowsAPIの初期化
	winApp = std::make_unique <WinApp>();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	// DirectX初期化
	dxCommon = std::make_unique <DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	// SrVマネージャ初期化
	SrvManager::GetInstance()->Initialize(dxCommon.get());

	// ImGuiの初期化
	ImGuiManager::GetInstance()->Initialize(winApp.get(), dxCommon.get());

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

	// 入力の初期化
	Input::GetInstance()->Initialize(winApp.get());

	// デバッグカメラ初期化
	debugCamera = std::make_unique <DebugCamera>();

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon.get());

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxCommon.get());

	// スプライト共通部初期化
	SpriteCommon::GetInstance()->Initialize(dxCommon.get());

	// 3Dオブジェクト共通部初期化
	Object3dCommon::GetInstance()->Initialize(dxCommon.get());
	// 3Dオブジェクトのデフォルトカメラにデバッグカメラをセット
	Object3dCommon::GetInstance()->SetDefaultCamera(debugCamera.get());

	// モデル基盤初期化
	ModelCommon::GetInstance()->Initialize(dxCommon.get());

	// パーティクル基盤初期化
	ParticleCommon::GetInstance()->Initialize(dxCommon.get());
	ParticleCommon::GetInstance()->SetDefaultCamera(debugCamera.get());
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

	// 入力の終了
	Input::GetInstance()->Finalize();

	// オーディオマネージャ終了
	AudioManager::GetInstance()->Finalize();

	// シーンマネージャ終了
	SceneManager::GetInstance()->Finalize();

	// ImGuiの終了
	ImGuiManager::GetInstance()->Finalize();

	// SrVマネージャの終了
	SrvManager::GetInstance()->Finalize();

	// DirectXの解放
	dxCommon->Finalize();
	
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