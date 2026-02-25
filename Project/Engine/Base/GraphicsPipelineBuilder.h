#pragma once

#include <d3d12.h>

namespace Engine {

	/// <summary>
	/// グラフィックスパイプラインビルダー
	/// </summary>
	class GraphicsPipelineBuilder {

	///-------------------------------------------/// 
	/// 構造体
	///-------------------------------------------///
	public:

		/// <summary>
		/// ブレンドモード
		/// </summary>
		enum class BlendMode {
			None,	  // なし
			Alpha,	  // アルファ
			Add,	  // 加算
			Subtract, // 減算
			Multiply, // 乗算
		};

		/// <summary>
		/// カリングモード
		/// </summary>
		enum class CullMode {
			None,  // カリングなし
			Front, // 表面(反時計回り)を表示しない
			Back,  // 裏面(時計回り)を表示しない
		};

		/// <summary>
		/// 深度モード
		/// </summary>
		enum class DepthMode {
			Disabled,  // 無効化
			ReadOnly,  // 読み込みのみ
			ReadWrite, // 読み書き可能
		};

		/// <summary>
		/// トポロジタイプ
		/// </summary>
		enum class TopologyType {
			Point,    // 点
			Line,     // 線
			Triangle, // 三角形
		};

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		GraphicsPipelineBuilder();

		/// <summary>
		/// デストラクタ
		/// </summary>
		~GraphicsPipelineBuilder();

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
	private:

		/// <summary>
		/// ブレンドステートの設定
		/// </summary>
		void ConfigureBlendState();

		/// <summary>
		/// ラスタライザーステートの設定
		/// </summary>
		void ConfigureRasterizerState();

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ブレンドステートの設定
		D3D12_BLEND_DESC blendDesc_ = {};

		// ブレンドモード
		BlendMode blendMode_ = BlendMode::None;

		// ラスタライザーステート
		D3D12_RASTERIZER_DESC rasterizerDesc_ = {};

		// カリングモード
		CullMode cullMode_ = CullMode::None;
	};
}

