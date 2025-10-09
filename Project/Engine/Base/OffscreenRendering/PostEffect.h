#pragma once

#include "Vector4.h"

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

/// === 前方宣言 === ///
class DirectXUtility;

/// === ポストエフェクト処理 === ///
class PostEffect {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// デスクリプタヒープ生成
	/// </summary>
	void DescriptorHeapGenerate();

	/// <summary>
	/// レンダーターゲットビュー初期化
	/// </summary>
	void RenderTargetViewInitialize();

	/// <summary>
	/// シェーダーリソースビュー初期化
	/// </summary>
	void ShaderResourceViewInitialize();

	/// <summary>
	/// 深度ステンシルビュー初期化
	/// </summary>
	void DepthStencilViewInitialize();

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
/// ゲッター
///-------------------------------------------///
public:

	uint32_t GetSRVIndex() const { return srvIndex; } // SRVインデックスのゲッター

	uint32_t GetDepthSRVIndex() const { return depthSrvIndex; } // 深度用SRVインデックスのゲッター

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// HRESULT
	HRESULT hr;

	// SRVインデックス
	uint32_t srvIndex = 0;

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
	const Vector4 kRenderTargetClearValue = { 0.5f, 0.5f, 0.5f, 0.5f }; // グレーに設定

	// Depth用SRVインデックス
	uint32_t depthSrvIndex = 0;

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

	// ビューポート矩形
	D3D12_VIEWPORT viewportRect{};

	// シザー矩形
	D3D12_RECT scissorRect{};

	//  レンダーテクスチャ用のTrainsitionBarrier
	D3D12_RESOURCE_BARRIER renderTextureBarrier{};

	// レンダーテクスチャ用のバリアの状態
	D3D12_RESOURCE_STATES renderTextureState = D3D12_RESOURCE_STATE_RENDER_TARGET;

	// 深度ステンシル用のTrainsitionBarrier
	D3D12_RESOURCE_BARRIER depthStencilBarrier{};

	// 深度ステンシル用のバリアの状態
	D3D12_RESOURCE_STATES depthStencilState = D3D12_RESOURCE_STATE_DEPTH_WRITE;

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility = nullptr;
};

