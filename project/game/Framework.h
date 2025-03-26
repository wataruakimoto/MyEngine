#pragma once

#include "winApp/WinApp.h"
#include "base/DirectXCommon.h"
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
	std::unique_ptr <WinApp> winApp = nullptr;

	// DirectX基盤のポインタ
	std::unique_ptr <DirectXCommon> dxCommon = nullptr;

	// デバッグカメラ
	std::unique_ptr <DebugCamera> debugCamera = nullptr;

	// シーンファクトリー
	std::unique_ptr <AbstractSceneFactory> sceneFactory_ = nullptr;

	// ゲーム終了フラグ
	bool endRequest_ = false;
};