#pragma once

#include "GraphicsPipelineBuilder.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;

	/// <summary>
	/// スプライトのレンダラー
	/// </summary>
	class SpriteRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static SpriteRenderer* instance_;

		// コンストラクタ隠蔽
		SpriteRenderer() = default;
		// デストラクタ隠蔽
		~SpriteRenderer() = default;
		// コピーコンストラクタ禁止
		SpriteRenderer(SpriteRenderer&) = delete;
		// コピーオペレータ禁止
		SpriteRenderer& operator=(SpriteRenderer&) = delete;

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
		static SpriteRenderer* GetInstance();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilder_;

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName = L"Sprite/Sprite.VS.hlsl";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"Sprite/Sprite.PS.hlsl";

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// SrvManagerのインスタンス
		SrvManager* srvManager_ = nullptr;
	};
}

