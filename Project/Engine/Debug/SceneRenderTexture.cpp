#include "SceneRenderTexture.h"
#include "DirectXUtility.h"
#include "WinApp.h"
#include "SrvManager.h"

#include <imgui.h>

void SceneRenderTexture::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// ディスクリプタヒープの生成
	DescriptorHeapGenerate();

	// レンダーターゲットビューの初期化
	RenderTargetViewInitialize();

	// 深度ステンシルビューの初期化
	DepthStencilViewInitialize();

	// シェーダーリソースビューの初期化
	ShaderResourceViewInitialize();

	// ビューポート矩形の初期化
	ViewportRectInitialize();

	// シザリング矩形の初期化
	ScissoringRectInitialize();
}

void SceneRenderTexture::PreDraw() {

	// コマンドリストをDirectXUtilityから取得A
	ID3D12GraphicsCommandList* commandList = dxUtility->GetCommandList().Get();

	/// === レンダーテクスチャ用のバリアの設定 === ///

	// バリアはTransition
	renderTextureBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	renderTextureBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象はレンダーテクスチャリソース
	renderTextureBarrier.Transition.pResource = renderTextureResource.Get();
	// 遷移前(現在)のResouceState
	renderTextureBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	// 遷移後のResouceState
	renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描く
	// 全部まとめて変える
	renderTextureBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &renderTextureBarrier);

	/// === 深度ステンシル用のバリアの設定 === ///

	// バリアはTransition
	depthStencilBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	depthStencilBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象は深度ステンシルリソース
	depthStencilBarrier.Transition.pResource = depthStencilResource.Get();
	// 遷移前(現在)のResouceState
	depthStencilBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	// 遷移後のResouceState
	depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
	// 全部まとめて変える
	depthStencilBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &depthStencilBarrier);

	/// ===== RTVとDSVの設定 ===== ///

	// レンダーターゲットと深度ステンシルビューを設定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	/// ===== 画面のクリア ===== ///

	// 色をクリア
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	// 深度をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, kDepthClearValue, 0, 0, nullptr);

	/// ===== ビューポートとシザーの設定 ===== ///

	// ビューポート矩形の設定
	commandList->RSSetViewports(1, &viewportRect);
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &scissorRect);
}

void SceneRenderTexture::PostDraw() {

	// コマンドリストをDirectXUtilityから取得
	ID3D12GraphicsCommandList* commandList = dxUtility->GetCommandList().Get();

	/// === レンダーテクスチャ用のバリアの設定 === ///

	// 遷移前(現在)のResouceState
	renderTextureBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描く
	// 遷移後のResouceState
	renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &renderTextureBarrier);

	/// === 深度ステンシル用のバリアの設定 === ///

	// 遷移前(現在)のResouceState
	depthStencilBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
	// 遷移後のResouceState
	depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &depthStencilBarrier);
}

void SceneRenderTexture::CreateSceneView() {

	ImGui::Begin("シーンビュー");

	// ウィンドウサイズの設定
	ImVec2 windowSize = ImGui::GetContentRegionAvail(); // とりあえず利用可能なサイズを取得
	ImGui::Image((ImTextureID)SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex).ptr, windowSize);

	ImGui::End();
}

void SceneRenderTexture::DescriptorHeapGenerate() {

	// DeviceをDirectXUtilityから取得
	ID3D12Device* device = dxUtility->GetDevice().Get();

	// RTV用のディスクリプタサイズを取得
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTV用のディスクリプタヒープを生成
	rtvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1, false); // 数は1 シェーダで使わない

	// DSV用のディスクリプタサイズを取得
	dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// DSV用のディスクリプタヒープを生成
	dsvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false); // 数は1 シェーダで使わない
}

void SceneRenderTexture::RenderTargetViewInitialize() {

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

void SceneRenderTexture::DepthStencilViewInitialize() {

	// Resourceの生成
	depthStencilResource = dxUtility->CreateDepthStencilResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_D24_UNORM_S8_UINT, // フォーマット
		kDepthClearValue // クリア深度
	);

	// DSV用の設定
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// デスクリプタヒープの先頭を取得
	dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// DSV生成
	dxUtility->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvHandle);
}

void SceneRenderTexture::ShaderResourceViewInitialize() {

	// SRV確保
	srvIndex = SrvManager::GetInstance()->Allocate();

	// SRV作成
	SrvManager::GetInstance()->CreateSRVforRenderTexture(srvIndex, renderTextureResource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
}

void SceneRenderTexture::ViewportRectInitialize() {

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = (float)WinApp::kClientWidth;
	viewportRect.Height = (float)WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void SceneRenderTexture::ScissoringRectInitialize() {

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}
