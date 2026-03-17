#pragma once

#include "WinApp.h"
#include "SwapChain.h"
#include "SceneBuffer.h"
#include "OffscreenRendering/PostProcessBuffer.h"
#include "AbstractSceneFactory.h"
#include "OffscreenRendering/FilterManager.h"

#include <memory>

namespace Engine {

	/// === 前方宣言 === ///
	
	class DirectXUtility;
	class SrvManager;
	class FilterManager;
	class ImGuiManager;
	class TextureManager;
	class SpriteRenderer;
	class ModelManager;
	class Object3dRenderer;
	class SkyBoxRenderer;
	class ParticleRenderer;
	class LineRenderer;
	class AudioManager;
	class Input;
	class SceneManager;
	class LineManager;

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

		// ゲーム終了フラグ
		bool endRequest_ = false;

		/// ===== ポインタ・インスタンス ===== ///

		// WindowsAPIのポインタ
		std::unique_ptr<WinApp> winApp = nullptr;

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// Srvマネージャのインスタンス
		SrvManager* srvManager_ = nullptr;

		// スプライトレンダラーのインスタンス
		SpriteRenderer* spriteRenderer_ = nullptr;

		// モデルマネージャのインスタンス
		ModelManager* modelManager_ = nullptr;

		// 3Dオブジェクトレンダラーのインスタンス
		Object3dRenderer* object3dRenderer_ = nullptr;

		// スカイボックスレンダラーのインスタンス
		SkyBoxRenderer* skyBoxRenderer_ = nullptr;

		// パーティクルレンダラーのインスタンス
		ParticleRenderer* particleRenderer_ = nullptr;

		// 線のレンダラーのインスタンス
		LineRenderer* lineRenderer_ = nullptr;

		// スワップチェインのポインタ
		std::unique_ptr<SwapChain> swapChain = nullptr;

		// シーンバッファのポインタ
		std::unique_ptr<SceneBuffer> sceneBuffer = nullptr;

		// ポストプロセスバッファのポインタ
		std::unique_ptr<PostProcessBuffer> postProcessBuffer = nullptr;

		// フィルタマネージャのインスタンス
		FilterManager* filterManager_ = nullptr;

		// ImGuiマネージャのインスタンス
		ImGuiManager* imguiManager_ = nullptr;

		// テクスチャマネージャのインスタンス
		TextureManager* textureManager_ = nullptr;

		// シーンファクトリー
		std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;

		// オーディオマネージャのインスタンス
		AudioManager* audioManager_ = nullptr;

		// 入力のインスタンス
		Input* input_ = nullptr;

		// シーンマネージャのインスタンス
		SceneManager* sceneManager_ = nullptr;

		// 線マネージャのインスタンス
		LineManager* lineManager_ = nullptr;
	};
}