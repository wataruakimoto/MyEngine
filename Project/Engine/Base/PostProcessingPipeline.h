#pragma once
#include "math/Matrix4x4.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

/// ===== 前方宣言 ===== ///
class DirectXUtility;
class SrvManager;
class PostEffect;
class Camera;

/// ===== レンダーテクスチャ用パイプライン ===== ///
class PostProcessingPipeline {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXUtility* dxUtility, PostEffect* postEffect);

	/// <summary>
	/// 描画設定
	/// </summary>
	void Draw();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// RootSignature作成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// InputLayout作成
	/// </summary>
	void CreateInputLayout();

	/// <summary>
	/// BlendState作成
	/// </summary>
	void CreateBlendState();

	/// <summary>
	/// RasterizerState作成
	/// </summary>
	void CreateRasterizerState();

	/// <summary>
	/// VertexShader作成
	/// </summary>
	void CreateVertexShader();

	/// <summary>
	/// PixelShader作成
	/// </summary>
	void CreatePixelShader();

	/// <summary>
	/// DepthStencilState作成
	/// </summary>
	void CreateDepthStencilState();

	/// <summary>
	/// GraphicsPipeline作成
	/// </summary>
	void CreateGraphicsPipeline();

	/// <summary>
	/// マテリアルデータ生成
	/// </summary>
	void GenerateMaterialData();

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	struct Material {

		Matrix4x4 projectionInverse; // 投影逆行列
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

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

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = nullptr;
	// マテリアルデータ
	Material* materialData = nullptr;

	// カメラの借りポインタ
	Camera* camera = nullptr;
};

