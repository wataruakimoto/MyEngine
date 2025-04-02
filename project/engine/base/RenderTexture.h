#pragma once
#include "DirectXCommon.h"
#include "math/Vector4.h"
#include <wrl.h>

/// === レンダーテクスチャ処理 === ///
class RenderTexture {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static RenderTexture* instance;
	// コンストラクタの隠蔽
	RenderTexture() = default;
	// デストラクタの隠蔽
	~RenderTexture() = default;
	// コピーコンストラクタの封印
	RenderTexture(RenderTexture&) = delete;
	// コピー代入演算子の封印
	RenderTexture& operator=(RenderTexture&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon"></param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// ディスクリプタヒープ生成
	/// </summary>
	void DescriptorHeapGenerate();

	/// <summary>
	/// レンダーターゲットビュー初期化
	/// </summary>
	void RenderTargetViewInitialize();

	/// <summary>
	/// 深度ステンシルビュー初期化
	/// </summary>
	void DepthStencilViewInitialize();

	/// <summary>
	/// レンダーテクスチャ生成
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr <ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	/// <summary>
	/// 深度ステンシルリソース生成
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearDepth"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr <ID3D12Resource> CreateDepthStencilResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const float clearDepth);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	static RenderTexture* GetInstance();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectX12共通処理
	DirectXCommon* dxCommon = nullptr;

	// RTV用デスクリプタサイズ
	uint32_t rtvDescriptorSize;

	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	// クリアする色
	const Vector4 kRenderTargetClearValue = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤に設定

	// DSV用デスクリプタサイズ
	uint32_t dsvDescriptorSize;

	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap;

	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;

	// DSVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	// クリアする深度
	const float kDepthClearValue = 1.0f;
};

