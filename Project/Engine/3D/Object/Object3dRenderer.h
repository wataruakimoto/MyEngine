#pragma once

#include "GraphicsPipelineBuilder.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;
	class Camera;

	/// <summary>
	/// 3Dオブジェクトのレンダラー
	/// </summary>
	class Object3dRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static Object3dRenderer* instance_;

		// コンストラクタ隠蔽
		Object3dRenderer() = default;
		// デストラクタ隠蔽
		~Object3dRenderer() = default;
		// コピーコンストラクタ禁止
		Object3dRenderer(Object3dRenderer&) = delete;
		// コピーオペレータ禁止
		Object3dRenderer& operator=(Object3dRenderer&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 不透明用の描画設定
		/// </summary>
		void SettingDrawingOpaque();

		/// <summary>
		/// 透明用の描画設定
		/// </summary>
		void SettingDrawingAlpha();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
	private:

		/// <summary>
		/// 不透明用のグラフィックスパイプラインの生成
		/// </summary>
		void CreateGraphicsPipelinOpaque();

		/// <summary>
		/// 透明用のグラフィックスパイプラインの生成
		/// </summary>
		void CreateGraphicsPipelinAlpha();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static Object3dRenderer* GetInstance();

		/// <summary>
		/// デフォルトカメラのゲッター
		/// </summary>
		/// <returns>カメラのポインタ</returns>
		Camera* GetDefaultCamera() const { return defaultCamera_; }

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// デフォルトカメラのセッター
		/// </summary>
		/// <param name="camera">カメラのポインタ</param>
		void SetDefaultCamera(Camera* camera) { defaultCamera_ = camera; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// 不透明用パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilderOpaque_;

		// 透明用パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilderAlpha_;

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName = L"Object3d/Object3d.VS.hlsl";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"Object3d/Object3d.PS.hlsl";

		/// ===== 借りポインタ・インスタンス ===== ///

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager_ = nullptr;

		// デフォルトカメラ
		Camera* defaultCamera_ = nullptr;
	};
}

