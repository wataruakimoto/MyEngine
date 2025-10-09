#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>

/// ===== 前方宣言 ===== ///
class Camera;
class DirectXUtility;

/// ===== パーティクル基盤 ===== ///
class ParticleCommon {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static ParticleCommon* instance;
	// コンストラクタの隠蔽
	ParticleCommon() = default;
	// デストラクタの隠蔽
	~ParticleCommon() = default;
	// コピーコンストラクタの封印
	ParticleCommon(ParticleCommon&) = delete;
	// コピー代入演算子の封印
	ParticleCommon& operator=(ParticleCommon&) = delete;

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
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ParticleCommon* GetInstance();

	/// <summary>
	/// デフォルトカメラのゲッター
	/// </summary>
	/// <returns>Camera</returns>
	Camera* GetDefaultCamera() const { return defaultCamera_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// デフォルトカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = nullptr;

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

	// GraphicsPipeline
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState = nullptr;

	// デフォルトカメラ
	Camera* defaultCamera_ = nullptr;
};

