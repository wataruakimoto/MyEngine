#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

/// ===== 前方宣言 ===== ///
class DirectXUtility;
class SrvManager;
class PostEffect;
class Camera;

/// ===== フィルター基底クラス ===== ///
class BaseFilter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	~BaseFilter() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(DirectXUtility* dxUtility, PostEffect* postEffect) = 0;

	/// <summary>
	/// 描画設定
	/// </summary>
	virtual void Draw() = 0;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
protected:

	/// <summary>
	/// RootSignature作成
	/// </summary>
	virtual void CreateRootSignature() = 0;

	/// <summary>
	/// InputLayout作成
	/// </summary>
	virtual void CreateInputLayout() = 0;

	/// <summary>
	/// BlendState作成
	/// </summary>
	virtual void CreateBlendState() = 0;

	/// <summary>
	/// RasterizerState作成
	/// </summary>
	virtual void CreateRasterizerState() = 0;

	/// <summary>
	/// VertexShader作成
	/// </summary>
	virtual void CreateVertexShader() = 0;

	/// <summary>
	/// PixelShader作成
	/// </summary>
	virtual void CreatePixelShader() = 0;

	/// <summary>
	/// DepthStencilState作成
	/// </summary>
	virtual void CreateDepthStencilState() = 0;

	/// <summary>
	/// GraphicsPipeline作成
	/// </summary>
	virtual void CreateGraphicsPipeline() = 0;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// DirectXUtilityのポインタ
	DirectXUtility* dxUtility = nullptr;

	// PostEffectのポインタ
	PostEffect* postEffect = nullptr;

	// HRESULT
	HRESULT hr;

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

	// GraphicsPipeline
	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState = nullptr;
};

