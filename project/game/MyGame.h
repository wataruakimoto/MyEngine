#pragma once

#include "input/Input.h"
#include "winApp/WinApp.h"
#include "base/DirectXCommon.h"
#include "debug/ImGuiManager.h"
#include "2d/SpriteCommon.h"
#include "3d/Object3dCommon.h"
#include "3d/ModelCommon.h"
#include "3d/Camera.h"

/// === ゲーム関連 === ///
class MyGame {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 入力のポインタ
	Input* input = nullptr;

	// WindowsAPIのポインタ
	WinApp* winApp = nullptr;

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon = nullptr;

	// ImGui管理クラスのポインタ
	ImGuiManager* imGuiManager = nullptr;

	// カメラのポインタ
	Camera* camera = nullptr;

	// スプライト共通部のポインタ
	SpriteCommon* spriteCommon = nullptr;

	// 3Dオブジェクト共通部のポインタ
	Object3dCommon* object3dCommon = nullptr;

	// モデル基盤のポインタ
	ModelCommon* modelCommon = nullptr;
};