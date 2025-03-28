#pragma once
#include "DirectXCommon.h"
#include "math/Vector4.h"
#include <wrl.h>

/// === レンダーテクスチャ処理 === ///
class RenderTexture {

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
	/// レンダーターゲットビュー初期化
	/// </summary>
	void RenderTargetViewInitialize();

	/// <summary>
	/// レンダーテクスチャ生成
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr <ID3D12Resource> CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// DirectX12共通処理
	DirectXCommon* dxCommon = nullptr;

	// RTV用デスクリプタサイズ
	uint32_t rtvDescriptorSize;

	// RTV用デスクリプタヒープ
	ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
};

