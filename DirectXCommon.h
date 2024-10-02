#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"

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

private: // クラス内のみの関数

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

private: // メンバ変数

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	// DirectX12デバイス
	ComPtr<ID3D12Device> device;

	// DXGIファクトリー
	ComPtr<IDXGIFactory7> dxgiFactory;

	// コマンドアロケータ
	ComPtr <ID3D12CommandAllocator> commandAllocator;
	
	// コマンドリスト
	ComPtr <ID3D12GraphicsCommandList> commandList;
	
	// コマンドキュー
	ComPtr <ID3D12CommandQueue> commandQueue;

	// スワップチェーン
	ComPtr <IDXGISwapChain4> swapChain;
};