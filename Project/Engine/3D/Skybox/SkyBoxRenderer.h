#pragma once

#include "GraphicsPipelineBuilder.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;
	class Camera;

	/// <summary>
	/// スカイボックスのレンダラー
	/// </summary>
	class SkyBoxRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static SkyBoxRenderer* instance_;

		// コンストラクタ隠蔽
		SkyBoxRenderer() = default;
		// デストラクタ隠蔽
		~SkyBoxRenderer() = default;
		// コピーコンストラクタ禁止
		SkyBoxRenderer(SkyBoxRenderer&) = delete;
		// コピーオペレータ禁止
		SkyBoxRenderer& operator=(SkyBoxRenderer&) = delete;

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 描画設定
		/// </summary>
		void SettingDrawing();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static SkyBoxRenderer* GetInstance();

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

		// パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilder_;

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName = L"SkyBox/Skybox.VS.hlsl";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"SkyBox/Skybox.PS.hlsl";

		/// ===== 借りポインタ・インスタンス ===== ///
		
		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager_ = nullptr;

		// デフォルトカメラ
		Camera* defaultCamera_ = nullptr;
	};
}

