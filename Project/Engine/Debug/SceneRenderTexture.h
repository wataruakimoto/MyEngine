#pragma once

#include "Vector4.h"

#include <d3d12.h>
#include <wrl.h>
#include <stdint.h>

/// ===== 前方宣言 ===== ///
class DirectXUtility;

/// ===== ImGuiを用いたシーンビュー ===== ///
class SceneRenderTexture {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// シーンビュー作成
	/// </summary>
	void CreateSceneView();

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
	/// RTV初期化
	/// </summary>
	void RenderTargetViewInitialize();

	/// <summary>
	/// DSV初期化
	/// </summary>
	void DepthStencilViewInitialize();

	/// <summary>
	/// SRV初期化
	/// </summary>
	void ShaderResourceViewInitialize();

	/// <summary>
	/// ビューポート矩形の初期化
	/// </summary>
	void ViewportRectInitialize();

	/// <summary>
	/// シザリング矩形の初期化
	/// </summary>
	void ScissoringRectInitialize();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	/// ===== RTV用の変数 ===== ///

	// レンダーテクスチャリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> renderTextureResource = nullptr;

	// RTV用デスクリプタサイズ
	uint32_t rtvDescriptorSize;

	// RTV用デスクリプタヒープ
	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> rtvDescriptorHeap;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

	// RTVハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;

	// クリアする色
	const Vector4 kRenderTargetClearValue = { 1.0f, 1.0f, 1.0f, 1.0f }; // 白に設定

	/// ===== DSV用の変数 ===== ///

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

	/// ===== SRV用の変数 ===== ///

	// SRVインデックス
	uint32_t srvIndex = 0;

	/// ===== その他の変数 ===== ///

	// ビューポート矩形
	D3D12_VIEWPORT viewportRect{};

	// シザリング矩形
	D3D12_RECT scissorRect{};

	// レンダーテクスチャ用のTrainsitionBarrier
	D3D12_RESOURCE_BARRIER renderTextureBarrier{};

	// 深度ステンシル用のTrainsitionBarrier
	D3D12_RESOURCE_BARRIER depthStencilBarrier{};

	/// ===== 借りポインタ ===== ///

	// DirectXユーティリティのポインタ
	DirectXUtility* dxUtility = nullptr;
};

