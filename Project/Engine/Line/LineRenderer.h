#pragma once

#include "GraphicsPipelineBuilder.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///
	class DirectXUtility;
	class SrvManager;

	/// <summary>
	/// 線描画のレンダラー
	/// </summary>
	class LineRenderer {

	///-------------------------------------------/// 
	/// シングルトン
	///-------------------------------------------///
	private:

		// インスタンス
		static LineRenderer* instance_;

		// コンストラクタ隠蔽
		LineRenderer() = default;
		// デストラクタ隠蔽
		~LineRenderer() = default;
		// コピーコンストラクタ禁止
		LineRenderer(LineRenderer&) = delete;
		// コピーオペレータ禁止
		LineRenderer& operator=(LineRenderer&) = delete;

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
		static LineRenderer* GetInstance();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// パイプラインビルダー
		GraphicsPipelineBuilder pipelineBuilder_;

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName = L"Line/Line.VS.hlsl";

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName = L"Line/Line.PS.hlsl";

		// DirectXユーティリティのインスタンス
		DirectXUtility* dxUtility_ = nullptr;
	};
}

