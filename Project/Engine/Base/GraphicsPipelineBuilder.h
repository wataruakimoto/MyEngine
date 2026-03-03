#pragma once

#include "DirectXUtility.h"

#include <d3d12.h>
#include <dxcapi.h>
#include <string>
#include <vector>
#include <deque>
#include <wrl.h>

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
		/// トポロジーモード
		/// </summary>
		enum class TopologyMode {
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
		/// ルートパラメーターの追加
		/// </summary>
		/// <param name="type">ルートパラメータの種類 (CBV / SRV / UAV)</param>
		/// <param name="shaderRegister">シェーダーのレジスタ番号</param>
		/// <param name="visibility">アクセスするシェーダー</param>
		void AddRootParameter(D3D12_ROOT_PARAMETER_TYPE type, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

		/// <summary>
		/// テーブルを使ったルートパラメーターの追加
		/// </summary>
		/// <param name="type">ディスクリプタレンジの種類 (SRV / UAV / CBV / Sampler)</param>
		/// <param name="baseShaderRegister">シェーダーのレジスタ番号の基準</param>
		/// <param name="visibility">アクセスするシェーダー</param>
		void AddRootParameterTable(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT baseShaderRegister, D3D12_SHADER_VISIBILITY visibility);

		/// <summary>
		/// 静的サンプラーの追加
		/// </summary>
		/// <param name="filter">フィルターモード</param>
		/// <param name="addressMode">テクスチャ端の処理</param>
		/// <param name="shaderRegister">シェーダーのレジスタ番号</param>
		/// <param name="visibility">アクセスするシェーダー</param>
		void AddStaticSampler(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressMode, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

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

		/// <summary>
		/// パイプラインを初期化
		/// </summary>
		void Reset();

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

		/// <summary>
		/// プリミティブトポロジーの設定
		/// </summary>
		void ConfigureTopologyType();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// ルートシグネチャのゲッター
		/// </summary>
		/// <returns>ID3D12RootSignature</returns>
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature_; }

		/// <summary>
		/// グラフィックスパイプラインのゲッター
		/// </summary>
		/// <returns>ID3D12PipelineState</returns>
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipeline() { return graphicsPipelineState_; }

	///-------------------------------------------/// 
	/// セッター
	///-------------------------------------------///
	public:

		/// <summary>
		/// 頂点シェーダーのファイル名のセッター
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		void SetVertexShaderFileName(const std::wstring& fileName) { vertexShaderFileName_ = fileName; }

		/// <summary>
		/// ピクセルシェーダーのファイル名のセッター
		/// </summary>
		/// <param name="fileName">ファイル名</param>
		void SetPixelShaderFileName(const std::wstring& fileName) { pixelShaderFileName_ = fileName; }

		/// <summary>
		/// ブレンドモードのセッター
		/// </summary>
		/// <param name="blendMode">ブレンドモード</param>
		void SetBlendMode(BlendMode blendMode) { blendMode_ = blendMode; }

		/// <summary>
		/// カリングモードのセッター
		/// </summary>
		/// <param name="cullMode">カリングモード</param>
		void SetCullMode(CullMode cullMode) { cullMode_ = cullMode; }

		/// <summary>
		/// 深度モードのセッター
		/// </summary>
		/// <param name="depthMode">深度モード</param>
		void SetDepthMode(DepthMode depthMode) { depthMode_ = depthMode; }

		/// <summary>
		/// トポロジーモードのセッター
		/// </summary>
		/// <param name="topologyMode">トポロジーモード</param>
		void SetTopologyMode(TopologyMode topologyMode) { topologyMode_ = topologyMode; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
	private:

		/// ===== RootSignature ===== ///

		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

		// ルートパラメーターの動的配列
		std::vector<D3D12_ROOT_PARAMETER> rootParameters_;

		// ディスクリプタレンジの動的配列
		std::deque<D3D12_DESCRIPTOR_RANGE> descriptorRanges_;

		// 静的サンプラーの動的配列
		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerDescs_;

		/// ===== Shader ===== ///

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

		/// ===== BlendState ===== ///

		// ブレンドステートの設定
		D3D12_BLEND_DESC blendDesc_ = {};

		// ブレンドモード
		BlendMode blendMode_ = BlendMode::None;

		/// ===== RasterizerState ===== ///

		// ラスタライザーステートの設定
		D3D12_RASTERIZER_DESC rasterizerDesc_ = {};

		// カリングモード
		CullMode cullMode_ = CullMode::None;

		/// ===== DepthStencilState ===== ///

		// 深度ステンシルステートの設定
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc_ = {};

		// 深度モード
		DepthMode depthMode_ = DepthMode::Disabled;

		/// ===== InputLayout ===== ///

		// インプットエレメントの設定の動的配列
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs_;

		// インプットレイアウトの設定
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};

		/// ===== TopologyType ===== ///

		// トポロジーモード
		TopologyMode topologyMode_ = TopologyMode::Triangle;

		// プリミティブトポロジータイプ
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType_ = {};

		/// ===== Pipeline ===== ///

		// グラフィックスパイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_ = nullptr;

		/// ===== インスタンス ===== ///

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();
	};
}

