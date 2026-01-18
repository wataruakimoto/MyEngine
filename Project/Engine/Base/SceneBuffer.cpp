#include "SceneBuffer.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "WinApp.h"

#include <imgui.h>

void SceneBuffer::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility = DirectXUtility::GetInstance();

	// SRVマネージャのインスタンスを取得
	srvManager = SrvManager::GetInstance();

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

void SceneBuffer::CreateSceneView() {

#ifdef USE_IMGUI

	// ウィンドウのパディングをなくす（見た目をスッキリさせるため）
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("シーンビュー");

	// 1. 今使えるウィンドウの中身のサイズを取得 (タブバーなどは除外されている)
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	// 2. ターゲットのアスペクト比 (16:9)
	float targetAspect = 16.0f / 9.0f;

	// 3. 現在のウィンドウのアスペクト比
	float currentAspect = windowSize.x / windowSize.y;

	ImVec2 imageSize = windowSize;

	// 4. 比率に合わせてサイズを調整 (レターボックス計算)
	if (currentAspect > targetAspect) {
		// ウィンドウが「横長」すぎる場合 -> 横幅を縮める (左右に黒帯)
		imageSize.x = windowSize.y * targetAspect;
		imageSize.y = windowSize.y;
	}
	else {
		// ウィンドウが「縦長」すぎる場合 -> 高さを縮める (上下に黒帯)
		imageSize.x = windowSize.x;
		imageSize.y = windowSize.x / targetAspect;
	}

	// 5. 画像を中央に配置するためのカーソル位置調整
	ImVec2 cursorPos = ImGui::GetCursorPos(); // 現在の描画開始位置
	cursorPos.x += (windowSize.x - imageSize.x) * 0.5f;
	cursorPos.y += (windowSize.y - imageSize.y) * 0.5f;
	ImGui::SetCursorPos(cursorPos);

	// 6. 画像描画
	ImGui::Image(
		(ImTextureID)srvManager->GetGPUDescriptorHandle(srvIndex).ptr,
		imageSize
	);

	ImGui::End();
	ImGui::PopStyleVar();

#endif // USE_IMGUI
}

void SceneBuffer::PreDrawFiltered() {

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

	/// === 深度ステンシル用のバリアの設定 === ///

	// 書き込み状態でなければ
	if (currentDsvState != D3D12_RESOURCE_STATE_DEPTH_WRITE) {

		// バリアはTransition
		depthStencilBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		// Noneにしておく
		depthStencilBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		// バリアを張る対象は深度ステンシルリソース
		depthStencilBarrier.Transition.pResource = depthStencilResource.Get();
		// 遷移前(現在)のResourceState
		depthStencilBarrier.Transition.StateBefore = currentDsvState; // 読む
		// 遷移後のResourceState
		depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
		// 全部まとめて変える
		depthStencilBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		// TransitionBarrierを張る
		commandList->ResourceBarrier(1, &depthStencilBarrier);

		// 現在のリソースステートを更新
		currentDsvState = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
	}

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

void SceneBuffer::PreDrawUnfiltered() {

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

	/// === 深度ステンシル用のバリアの設定 === ///

	// 書き込み状態でなければ
	if (currentDsvState != D3D12_RESOURCE_STATE_DEPTH_WRITE) {

		// バリアはTransition
		depthStencilBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		// Noneにしておく
		depthStencilBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		// バリアを張る対象は深度ステンシルリソース
		depthStencilBarrier.Transition.pResource = depthStencilResource.Get();
		// 遷移前(現在)のResourceState
		depthStencilBarrier.Transition.StateBefore = currentDsvState; // 読む
		// 遷移後のResourceState
		depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
		// 全部まとめて変える
		depthStencilBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		// TransitionBarrierを張る
		commandList->ResourceBarrier(1, &depthStencilBarrier);

		// 現在のリソースステートを更新
		currentDsvState = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 書く
	}

	/// ===== RTVとDSVの設定 ===== ///

	// レンダーターゲットと深度ステンシルビューを設定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	/// ===== 画面のクリア ===== ///

	// ※重ね掛けするので色はクリアしない

	// 深度をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, kDepthClearValue, 0, 0, nullptr);

	/// ===== ビューポートとシザーの設定 ===== ///

	// ビューポート矩形の設定
	commandList->RSSetViewports(1, &viewportRect);
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &scissorRect);
}

void SceneBuffer::PreDrawResolve() {

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

	/// === 深度ステンシル用のバリアの設定 === ///

	// ※深度の状態は読むのままなのでバリアは張らない

	/// ===== RTVとDSVの設定 ===== ///

	// レンダーターゲットと深度ステンシルビューを設定
	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

	/// ===== 画面のクリア ===== ///

	// ※重ね掛けするので色はクリアしない

	// ※深度は使わないのでクリアしない

	/// ===== ビューポートとシザーの設定 ===== ///

	// ビューポート矩形の設定
	commandList->RSSetViewports(1, &viewportRect);
	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &scissorRect);
}

void SceneBuffer::PostDraw() {

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

	/// === 深度ステンシル用のバリアの設定 === ///

	// 書き込み状態であれば
	if (currentDsvState == D3D12_RESOURCE_STATE_DEPTH_WRITE) {

		// 遷移前(現在)のResourceState
		depthStencilBarrier.Transition.StateBefore = currentDsvState; // 書く
		// 遷移後のResourceState
		depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む

		// TransitionBarrierを張る
		commandList->ResourceBarrier(1, &depthStencilBarrier);

		// 現在のリソースステートを更新
		currentDsvState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // 読む
	}
}

void SceneBuffer::DescriptorHeapGenerate() {

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

void SceneBuffer::RenderTargetViewInitialize() {

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

void SceneBuffer::DepthStencilViewInitialize() {

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

void SceneBuffer::ShaderResourceViewInitialize() {

	// SRV確保
	srvIndex = srvManager->Allocate();

	// SRV作成
	srvManager->CreateSRVforRenderTexture(srvIndex, renderTextureResource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);

	// SRV確保
	depthSrvIndex = SrvManager::GetInstance()->Allocate();

	// SRV作成
	SrvManager::GetInstance()->CreateSRVforDepthStencil(depthSrvIndex, depthStencilResource.Get(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
}

void SceneBuffer::ViewportRectInitialize() {

	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = (float)WinApp::kClientWidth;
	viewportRect.Height = (float)WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void SceneBuffer::ScissoringRectInitialize() {

	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}
