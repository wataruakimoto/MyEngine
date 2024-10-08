#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include "array"
#include <dxcapi.h>

// DirectX基盤
class DirectXCommon {

public:
	// namespace省略 エイリアステンプレート
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

public:	// メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// デスクリプタヒープを作成する
	/// </summary>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	ComPtr <ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// RTVのCPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// RTVのGPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVのCPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// SRVのGPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVのCPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

	/// <summary>
	/// DSVのGPUのデスクリプタハンドルを取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

private: // クラス内処理の関数

	/// <summary>
	/// デバイスの初期化
	/// </summary>
	void DeviceInitialize();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void CommandRelatedInitialize();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void SwapChainGenerate();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void DepthBufferGenerate();

	/// <summary>
	/// 各種デスクリプターヒープの生成
	/// </summary>
	void VariousDescriptorHeapGenerate();

	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void RenderTargetViewInitialize();
	
	/// <summary>
	/// CPUのデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr <ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUのデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr <ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// 深度ステンシルビューの初期化
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
	/// DXCコンパイラの生成
	/// </summary>
	void DXCCompilerGenerate();

	/// <summary>
	/// ImGuiの初期化
	/// </summary>
	void ImGuiInitialize();

private: // メンバ変数

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	// DirectX12デバイス
	ComPtr<ID3D12Device> device = nullptr;

	// DXGIファクトリー
	ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

	// コマンドアロケータ
	ComPtr <ID3D12CommandAllocator> commandAllocator = nullptr;
	
	// コマンドリスト
	ComPtr <ID3D12GraphicsCommandList> commandList = nullptr;
	
	// コマンドキュー
	ComPtr <ID3D12CommandQueue> commandQueue = nullptr;

	// スワップチェーン
	ComPtr <IDXGISwapChain4> swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// リソース
	ComPtr <ID3D12Resource> resource = nullptr;

	// RTV用デスクリプタサイズ
	uint32_t descriptorSizeRTV;

	// SRV用デスクリプタサイズ
	uint32_t descriptorSizeSRV;

	// DSV用デスクリプタサイズ
	uint32_t descriptorSizeDSV;

	// RTV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	// SRV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> srvDescriptorHeap;

	// DSV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap;

	// スワップチェーンから引っ張ってきたリソース
	ComPtr <ID3D12Resource> swapChainResources[2] = { nullptr };

	// スワップチェーンリソース
	//std::array<ComPtr<ID3D12Resource>, 2> swapChainResource;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	// フェンス
	ComPtr <ID3D12Fence> fence = nullptr;

	// フェンスの値
	uint64_t fenceValue = 0;

	// フェンスイベント
	HANDLE fenceEvent;

	// ビューポート矩形
	D3D12_VIEWPORT viewport{};

	// シザー矩形
	D3D12_RECT scissorRect{};

	// DXCユーティリティ
	ComPtr <IDxcUtils> dxcUtils = nullptr;

	// DXCコンパイラ
	ComPtr <IDxcCompiler3> dxcCompiler = nullptr;

	// デフォルトインクルードハンドラ
	ComPtr <IDxcIncludeHandler> includeHandler = nullptr;

	// TrainsitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
};