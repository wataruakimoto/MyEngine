#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <string>
#include <vector>
#include <wrl.h>

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class DirectXUtility;

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

		/// <summary>
		/// インプットエレメントの追加
		/// </summary>
		/// <param name="semanticName">セマンティック名</param>
		/// <param name="semanticIndex">セマンティック番号</param>
		/// <param name="format">フォーマット</param>
		void AddInputElement(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format);

		/// <summary>
		/// パイプラインを生成
		/// </summary>
		void Build();

	///-------------------------------------------/// 
	/// クラス内関数
	///-------------------------------------------///
	private:

		/// <summary>
		/// ルートシグネチャの設定
		/// </summary>
		void ConfigureRootSignature();

		/// <summary>
		/// 頂点シェーダーの設定
		/// </summary>
		void ConfigureVertexShader();

		/// <summary>
		/// ピクセルシェーダーの設定
		/// </summary>
		void ConfigurePixelShader();

		/// <summary>
		/// ブレンドステートの設定
		/// </summary>
		void ConfigureBlendState();

		/// <summary>
		/// ラスタライザーステートの設定
		/// </summary>
		void ConfigureRasterizerState();

		/// <summary>
		/// 深度ステンシルステートの設定
		/// </summary>
		void ConfigureDepthStencilState();

		/// <summary>
		/// インプットレイアウトの設定
		/// </summary>
		void ConfigureInputLayout();

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		void SetVertexShaderFileName(const std::wstring& fileName) { vertexShaderFileName_ = fileName; }

		void SetPixelShaderFileName(const std::wstring& fileName) { pixelShaderFileName_ = fileName; }

		void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }

		void SetCullMode(CullMode cullMode) { cullMode_ = cullMode; }

		void SetDepthMode(DepthMode depthMode) { depthMode_ = depthMode; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

		// シェーダーファイルのディレクトリパス
		const std::wstring shaderDirectoryPath_ = L"Resources/Shaders/";

		// 頂点シェーダーのファイル名
		std::wstring vertexShaderFileName_ = L"";

		// 頂点シェーダーのデータ
		Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;

		// ピクセルシェーダーのファイル名
		std::wstring pixelShaderFileName_ = L"";

		// ピクセルシェーダーのデータ
		Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

		// ブレンドステートの設定
		D3D12_BLEND_DESC blendDesc_ = {};

		// ブレンドモード
		BlendMode blendMode_ = BlendMode::None;

		// ラスタライザーステートの設定
		D3D12_RASTERIZER_DESC rasterizerDesc_ = {};

		// カリングモード
		CullMode cullMode_ = CullMode::None;

		// 深度ステンシルステートの設定
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc_ = {};

		// 深度モード
		DepthMode depthMode_ = DepthMode::Disabled;

		// インプットエレメントの設定の動的配列
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs_;

		// インプットレイアウトの設定
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};

		// グラフィックスパイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = nullptr;
	};
}

