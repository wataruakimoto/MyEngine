#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "winApp/WinApp.h"
#include "array"
#include <dxcapi.h>
#include <string>
#include "DirectXTex/DirectXTex.h"
#include <chrono>

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
	/// 終了
	/// </summary>
	void Finalize();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///

	uint32_t GetBackBufferCount() const { return swapChainDesc.BufferCount; }

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

	/// <summary>
	/// シェーダーコンパイル
	/// </summary>
	/// <param name="filePath">Shaderファイルへのパス</param>
	/// <param name="profile">プロファイル</param>
	/// <returns></returns>
	ComPtr <IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// バッファリソース生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	ComPtr <ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// テクスチャリソース生成
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	ComPtr <ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャリソースにデータを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	void UploadTextureData(ComPtr <ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

private: // クラス内処理の関数

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
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

private: // メンバ変数

	// WindowsAPI
	WinApp* winApp_ = nullptr;

	// 深度バッファ
	ComPtr<ID3D12Resource> depthStencilResource;

	// RTV用デスクリプタサイズ
	uint32_t rtvDescriptorSize;

	// DSV用デスクリプタサイズ
	uint32_t dsvDescriptorSize;

	// RTV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	// DSV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap;

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];

	// TrainsitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;
};