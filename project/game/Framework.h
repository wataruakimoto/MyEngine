#pragma once

#include "winApp/WinApp.h"
#include "base/DirectXCommon.h"
#include "debug/ImGuiManager.h"
#include "camera/DebugCamera.h"
#include "2d/SpriteCommon.h"
#include "3d/ModelCommon.h"
#include "3d/Object3dCommon.h"
#include "scene/AbstractSceneFactory.h"

class Framework {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 終了フラグのチェック
	/// </summary>
	/// <returns></returns>
	virtual bool IsEndRequest() { return endRequest_; }

	/// <summary>
	/// ゲーム実行
	/// </summary>
	void Run();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// WindowsAPIのポインタ
	WinApp* winApp = nullptr;

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon = nullptr;

	// ImGui管理クラスのポインタ
	ImGuiManager* imGuiManager = nullptr;

	DebugCamera* debugCamera = nullptr;

	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// ゲーム終了フラグ
	bool endRequest_ = false;
};