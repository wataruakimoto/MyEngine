#pragma once

#include "DirectXUtility.h"

#include <d3d12.h>
#include <wrl.h>
#include <string>

/// ===== グラフィックスパイプライン生成クラス ===== ///
class GraphicsPipelineCreater {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GraphicsPipelineCreater();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GraphicsPipelineCreater();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void Create();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// InputLayoutの生成
	/// </summary>
	void CreateInputLayout();

	/// <summary>
	/// BlendStateの生成
	/// </summary>
	void CreateBlendState();

	/// <summary>
	/// RasterizerStateの生成
	/// </summary>
	void CreateRasterizerState();

	/// <summary>
	/// VertexShaderの生成
	/// </summary>
	void CreateVertexShader();

	/// <summary>
	/// PixelShaderの生成
	/// </summary>
	void CreatePixelShader();

	/// <summary>
	/// DepthStencilStateの生成
	/// </summary>
	void CreateDepthStencilState();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// グラフィックスパイプラインステートのゲッター
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() const { return graphicsPipelineState; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = DirectXUtility::GetInstance();

	// RootSignature
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	// BlendState
	D3D12_BLEND_DESC blendDesc{};

	// RasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// VertexShader
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = nullptr;

	// PixelShader
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = nullptr;

	// DepthStencilState
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// グラフィックスパイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	// ディレクトリパス
	const std::string shaderDirectoryPath = "Resources/Shaders/";

	// 頂点シェーダーのファイル名

};

