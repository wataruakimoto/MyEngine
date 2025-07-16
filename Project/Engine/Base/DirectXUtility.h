#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>
#include <chrono>

#include "DirectXTex/DirectXTex.h"

/// === DirectX機能 === ///
class DirectXUtility {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// デスクリプタヒープを作成する
	/// </summary>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// シェーダーコンパイル
	/// </summary>
	/// <param name="filePath">Shaderファイルへのパス</param>
	/// <param name="profile">プロファイル</param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// デバイス初期化
	/// </summary>
	void DeviceInitialize();

	/// <summary>
	/// コマンド関連の初期化
	/// </summary>
	void CommandRelatedInitialize();

	/// <summary>
	/// フェンスの初期化
	/// </summary>
	void FenceInitialize();

	/// <summary>
	/// DXCコンパイラの生成
	/// </summary>
	void DXCCompilerGenerate();

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>ID3D12Device</returns>
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; }

	/// <summary>
	/// コマンドリストを取得
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() const { return commandList; }

	/// <summary>
	/// コマンドキューを取得
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() const { return commandQueue; }

	/// <summary>
	/// DXGIファクトリーの取得
	/// </summary>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<IDXGIFactory7> GetDXGIFactory() { return dxgiFactory; }

	/// <summary>
	/// CPUのデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// GPUのデスクリプタハンドルを取得する
	/// </summary>
	/// <param name="descriptorHeap"></param>
	/// <param name="descriptorSize"></param>
	/// <param name="index"></param>
	/// <returns></returns>
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// バッファリソース生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// テクスチャリソース生成
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

	/// <summary>
	/// テクスチャリソースにデータを転送する
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// HRESULT
	HRESULT hr;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	// フェンス
	Microsoft::WRL::ComPtr <ID3D12Fence> fence = nullptr;

	// フェンスの値
	uint64_t fenceValue = 0;

	// フェンスイベント
	HANDLE fenceEvent;

	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

	// DXCユーティリティ
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;

	// DXCコンパイラ
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;

	// デフォルトインクルードハンドラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

	// 記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference;
};

