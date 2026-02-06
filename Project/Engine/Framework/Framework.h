#pragma once

#include "WinApp.h"
#include "SwapChain.h"
#include "SceneBuffer.h"
#include "OffscreenRendering/PostProcessBuffer.h"
#include "AbstractSceneFactory.h"

#include <memory>

namespace Engine {

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

		// スワップチェインのポインタ
		std::unique_ptr<SwapChain> swapChain = nullptr;

		// シーンバッファのポインタ
		std::unique_ptr<SceneBuffer> sceneBuffer = nullptr;

		// ポストプロセスバッファのポインタ
		std::unique_ptr<PostProcessBuffer> postProcessBuffer = nullptr;

		// シーンファクトリー
		std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

		// ゲーム終了フラグ
		bool endRequest_ = false;
	};
}