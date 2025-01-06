#pragma once

#include "base/DirectXCommon.h"

// スプライト共通部
class SpriteCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static SpriteCommon* instance;

	// コンストラクタの隠蔽
	SpriteCommon() = default;
	// デストラクタの隠蔽
	~SpriteCommon() = default;
	// コピーコンストラクタの封印
	SpriteCommon(SpriteCommon&) = delete;
	// コピー代入演算子の封印
	SpriteCommon& operator=(SpriteCommon&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SpriteCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 共通描画設定
	/// </summary>
	void SettingCommonDrawing();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// RootSignatureの生成
	/// </summary>
	void CreateRootSignature();

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

	/// <summary>
	/// GraphicsPipelineの生成
	/// </summary>
	void CreateGraphicsPipeline();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// DirectXCommonのゲッター
	/// </summary>
	/// <returns></returns>
	DirectXCommon* GetDxCommon() const { return dxCommon_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
	
	// DirectXCommonのポインタ
	DirectXCommon* dxCommon_;

	// RootSignature
	Microsoft::WRL::ComPtr <ID3D12RootSignature> rootSignature = nullptr;

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};

	// BlendState
	D3D12_BLEND_DESC blendDesc{};

	// RasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// VertexShader
	Microsoft::WRL::ComPtr <IDxcBlob> vertexShaderBlob = nullptr;

	// PixelShader
	Microsoft::WRL::ComPtr <IDxcBlob> pixelShaderBlob = nullptr;

	// DepthStencilState
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// GraphicsPipeline
	Microsoft::WRL::ComPtr <ID3D12PipelineState> graphicsPipelineState = nullptr;
};