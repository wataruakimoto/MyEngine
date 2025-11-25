#pragma once

#include "DirectXUtility.h"

#include <d3d12.h>
#include <wrl.h>
#include <string>

enum class Preset {
	Default,  // デフォルト設定
	Sprite,   // スプライト
	Object3D, // 3Dオブジェクト
};

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
	/// パイプライン生成
	/// </summary>
	void Create(Preset preset);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// BlendStateの生成
	/// </summary>
	void CreateBlendState(Preset preset);

	/// <summary>
	/// RasterizerStateの生成
	/// </summary>
	void CreateRasterizerState(Preset preset);

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
	void CreateDepthStencilState(Preset preset);

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// RootSignatureのセッター
	/// </summary>
	/// <param name="rootSignature"></param>
	void SetRootSignature(ID3D12RootSignature* rootSignature) { this->rootSignature = rootSignature; }

	/// <summary>
	/// InputLayoutのセッター
	/// </summary>
	/// <param name="inputLayoutDesc"></param>
	void SetInputLayoutDesc(const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc) { this->inputLayoutDesc = inputLayoutDesc; }

	/// <summary>
	/// 頂点シェーダーのファイル名のセッター
	/// </summary>
	/// <param name="fileName"></param>
	void SetVSFileName(const std::wstring& fileName) { vertexShaderFileName = fileName; }
	
	/// <summary>
	/// ピクセルシェーダーのファイル名のセッター
	/// </summary>
	/// <param name="fileName"></param>
	void SetPSFileName(const std::wstring& fileName) { pixelShaderFileName = fileName; }

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

	// シェーダーのディレクトリパス
	const std::wstring shaderDirectoryPath = L"Resources/Shaders/";

	// 頂点シェーダーのファイル名
	std::wstring vertexShaderFileName = L"";

	// 頂点シェーダーのフルパス
	std::wstring vertexShaderFullPath = L"";

	// ピクセルシェーダーのファイル名
	std::wstring pixelShaderFileName = L"";

	// ピクセルシェーダーのフルパス
	std::wstring pixelShaderFullPath = L"";
};

