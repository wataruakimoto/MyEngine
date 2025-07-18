#pragma once
#include "winApp/WinApp.h"
#include "base/DirectXUtility.h"
#include "base/OffscreenRendering/PostEffect.h"
#include "base/OffscreenRendering/Filters/DissolveFilter.h"
#include "base/SwapChain.h"
#include "camera/Camera.h"
#include "scene/AbstractSceneFactory.h"

#include <memory>

/// === 前方宣言 === ///
class WinApp;
class DirectXUtility;

/// === フレームワーク === ///
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
	std::unique_ptr<WinApp> winApp = nullptr;

	// DirectX基盤のポインタ
	std::unique_ptr<DirectXUtility> dxUtility = nullptr;

	// レンダーテクスチャのポインタ
	std::unique_ptr<PostEffect> postEffect = nullptr;

	// ポストエフェクトパイプラインのポインタ
	std::unique_ptr<DissolveFilter> filter = nullptr;

	// スワップチェインのポインタ
	std::unique_ptr<SwapChain> swapChain = nullptr;

	// デバッグカメラ
	std::unique_ptr<Camera> camera = nullptr;

	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

	// ゲーム終了フラグ
	bool endRequest_ = false;
};