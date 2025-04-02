#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <wrl.h>

/// ===  === ///
class DirectXUtility {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>ID3D12Device</returns>
	Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// HRESULT
	HRESULT hr;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

	// フェンス
	Microsoft::WRL::ComPtr <ID3D12Fence> fence = nullptr;

	// フェンスの値
	uint64_t fenceValue = 0;

	// フェンスイベント
	HANDLE fenceEvent;

	// DXCユーティリティ
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;

	// DXCコンパイラ
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;

	// デフォルトインクルードハンドラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
};

