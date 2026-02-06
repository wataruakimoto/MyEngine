#include "PostProcessBuffer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "WinApp.h"

#include <imgui.h>

using namespace Engine;

void PostProcessBuffer::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// SRVマネージャのインスタンスを取得
	srvManager = SrvManager::GetInstance();

	// ディスクリプタヒープの生成
	DescriptorHeapGenerate();

	// レンダーターゲットビューの初期化
	RenderTargetViewInitialize();

	// シェーダーリソースビューの初期化
	ShaderResourceViewInitialize();

	// ビューポート矩形の初期化
	ViewportRectInitialize();

	// シザリング矩形の初期化
	ScissoringRectInitialize();
}

void PostProcessBuffer::PreDraw() {

	// コマンドリストをDirectXUtilityから取得
	ID3D12GraphicsCommandList* commandList = dxUtility->GetCommandList().Get(); // コマンドリスト

	/// === レンダーテクスチャ用のバリアの設定 === ///

	// 描き込み状態でなければ
	if (currentRtvState != D3D12_RESOURCE_STATE_RENDER_TARGET) {

		// バリアはTransition
		renderTextureBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		// Noneにしておく
		renderTextureBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		// バリアを張る対象はレンダーテクスチャリソース
		renderTextureBarrier.Transition.pResource = renderTextureResource.Get();
		// 遷移前(現在)のResourceState
		renderTextureBarrier.Transition.StateBefore = currentRtvState; // 読む
		// 遷移後のResourceState
		renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描く
		// 全部まとめて変える
		renderTextureBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		// TransitionBarrierを張る
		commandList->ResourceBarrier(1, &renderTextureBarrier);

		// 現在のリソースステートを更新
		currentRtvState = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描く
	}

	/// ===== RTVとDSVの設定 ===== ///

	// レンダーターゲットと深度ステンシルビューを設定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	/// ===== 画面のクリア ===== ///

	// 色をクリア
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	/// ===== ビューポートとシザーの設定 ===== ///

	// ビューポート矩形の設定
	commandList->RSSetViewports(1, &viewportRect);
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &scissorRect);
}

void PostProcessBuffer::PostDraw() {

	// コマンドリストをDirectXUtilityから取得
	ID3D12GraphicsCommandList* commandList = dxUtility->GetCommandList().Get(); // コマンドリスト

	/// === レンダーテクスチャ用のバリアの設定 === ///

	// 描き込み状態であれば
	if (currentRtvState == D3D12_RESOURCE_STATE_RENDER_TARGET) {

		// 遷移前(現在)のResourceState
		renderTextureBarrier.Transition.StateBefore = currentRtvState; // 描く
		// 遷移後のResourceState
		renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む

		// TransitionBarrierを張る
		commandList->ResourceBarrier(1, &renderTextureBarrier);

		// 現在のリソースステートを更新
		currentRtvState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	}
}

void PostProcessBuffer::DescriptorHeapGenerate() {

	// DeviceをDirectXUtilityから取得
	ID3D12Device* device = dxUtility->GetDevice().Get();

	// RTV用のディスクリプタサイズを取得
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTV用のディスクリプタヒープを生成
	rtvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false); // 数は1 シェーダで使わない
}

void PostProcessBuffer::RenderTargetViewInitialize() {

	// Resourceの生成
	renderTextureResource = dxUtility->CreateRenderTextureResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, // フォーマット
		kRenderTargetClearValue // クリアカラー
	);

	// RTV用の設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // フォーマット
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// RTVハンドルを取得
	rtvHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// RTV生成
	dxUtility->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, rtvHandle);
}

void PostProcessBuffer::ShaderResourceViewInitialize() {

	// SRV確保
	srvIndex = srvManager->Allocate();

	// SRV作成
	srvManager->CreateSRVforRenderTexture(srvIndex, renderTextureResource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
}

void PostProcessBuffer::ViewportRectInitialize() {

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = (float)WinApp::kClientWidth;
	viewportRect.Height = (float)WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void PostProcessBuffer::ScissoringRectInitialize() {

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}
