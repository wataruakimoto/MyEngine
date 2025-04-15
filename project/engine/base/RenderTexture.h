#pragma once
#include "math/Vector4.h"
#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

/// === 前方宣言 === ///
class DirectXUtility;

/// === レンダーテクスチャ処理 === ///
class RenderTexture {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxUtility"></param>
	void Initialize(DirectXUtility* dxUtility);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void CommandRelatedInitialize();

	/// <summary>
	/// デスクリプタヒープ生成
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
	/// フェンスの初期化
	/// </summary>
	void FenceInitialize();

	/// <summary>
	/// ビューポート矩形の初期化
	/// </summary>
	void ViewportRectInitialize();

	/// <summary>
	/// シザリング矩形の初期化
	/// </summary>
	void ScissoringRectInitialize();

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
/// メンバ変数
///-------------------------------------------///
private:

	// HRESULT
	HRESULT hr;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	// レンダーテクスチャリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> renderTextureResource = nullptr;

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

	// 深度バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;

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

	// フェンス
	Microsoft::WRL::ComPtr <ID3D12Fence> fence = nullptr;

	// フェンスの値
	uint64_t fenceValue = 0;

	// フェンスイベント
	HANDLE fenceEvent;

	// ビューポート矩形
	D3D12_VIEWPORT viewportRect{};

	// シザー矩形
	D3D12_RECT scissorRect{};

	// TrainsitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	// DirectX12共通処理
	DirectXUtility* dxUtility = nullptr;
};

