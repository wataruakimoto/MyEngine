#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <stdint.h>
#include <chrono>

class WinApp;
class DirectXUtility;

/// === スワップチェイン === ///
class SwapChain {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp, DirectXUtility* dxUtility);

	/// <summary>
	/// 描画前
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後
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
	/// スワップチェーンの生成
	/// </summary>
	void SwapChainGenerate();

	/// <summary>
	/// 各種デスクリプターヒープの生成
	/// </summary>
	void VariousDescriptorHeapGenerate();

	/// <summary>
	/// レンダーターゲットビューの初期化
	/// </summary>
	void RenderTargetViewInitialize();

	/// <summary>
	/// 深度バッファの生成
	/// </summary>
	void DepthBufferGenerate();

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
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

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

	// スワップチェーン
	Microsoft::WRL::ComPtr <IDXGISwapChain4> swapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// スワップチェーンから引っ張ってきたリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> swapChainResources[2] = { nullptr };

	// スワップチェーンリソース
	//std::array<ComPtr<ID3D12Resource>, 2> swapChainResources;

	// RTV用デスクリプタサイズ
	uint32_t rtvDescriptorSize;

	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	// DSV用デスクリプタサイズ
	uint32_t dsvDescriptorSize;

	// DSV用デスクリプタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap;

	// 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource;

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

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference;

	// WinAppの借りポインタ
	WinApp* winApp = nullptr;

	// DirectXUtilityの借りポインタ
	DirectXUtility* dxUtility = nullptr;
};

