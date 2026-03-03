#pragma once

#include "GraphicsPipelineBuilder.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;
	class Camera;

	/// <summary>
	/// パーティクルのレンダラー
	/// </summary>
	class ParticleRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static ParticleRenderer* instance_;

		// コンストラクタ隠蔽
		ParticleRenderer() = default;
		// デストラクタ隠蔽
		~ParticleRenderer() = default;
		// コピーコンストラクタ禁止
		ParticleRenderer(ParticleRenderer&) = delete;
		// コピーオペレータ禁止
		ParticleRenderer& operator=(ParticleRenderer&) = delete;

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
		static ParticleRenderer* GetInstance();

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
		std::wstring vertexShaderFileName = L"Particle/Particle.VS.hlsl";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"Particle/Particle.PS.hlsl";

		/// ===== 借りポインタ・インスタンス ===== ///

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager_ = nullptr;

		// デフォルトカメラ
		Camera* defaultCamera_ = nullptr;
	};
}

