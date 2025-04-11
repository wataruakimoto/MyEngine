#include "RenderTexture.h"
#include "winApp/WinApp.h"
#include "DirectXUtility.h"

using namespace Microsoft::WRL;

void RenderTexture::Initialize(DirectXUtility* dxUtility) {

	// NULL検出
	assert(dxUtility);

	// メンバ変数に記録
	this->dxUtility = dxUtility;

	// コマンド関連の初期化
	CommandRelatedInitialize();

	// ディスクリプタヒープの生成
	DescriptorHeapGenerate();

	// レンダーターゲットビューの初期化
	RenderTargetViewInitialize();

	// 深度ステンシルビューの初期化
	DepthStencilViewInitialize();

	// フェンスの初期化
	FenceInitialize();

	// ビューポート矩形の初期化
	ViewportRectInitialize();

	// シザリング矩形の初期化
	ScissoringRectInitialize();
}

void RenderTexture::PreDraw() {

	// 描画先のRTVを指定
	commandList->OMSetRenderTargets(1, &rtvHandles[0], false, nullptr);

	// 画面全体の色をクリア
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	commandList->ClearRenderTargetView(rtvHandles[0], clearColor, 0, nullptr);

	// 画面全体の震度をクリア
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート矩形の設定
	commandList->RSSetViewports(1, &viewportRect);

	// シザリング矩形の設定
	commandList->RSSetScissorRects(1, &scissorRect);
}

void RenderTexture::PostDraw() {

	// ----------バックバッファの番号取得----------
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

	// ----------リソースバリアで表示状態に変更----------
	// 画面に描く処理はすべて終わり、画面に映すので、状態を遷移
	// 今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	// ----------グラフィックスコマンドをクローズ----------
	// コマンドリストの内容を確定させる。すべてのコマンドを積んでからCloseすること
	hr = commandList->Close();
	assert(SUCCEEDED(hr));

	// ----------GPUコマンドの実行----------
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	// ----------GPU画面の交換を通知----------
	swapChain->Present(1, 0);

	// ----------Fenceの値を更新----------
	fenceValue++;

	// ----------コマンドキューにシグナルを送る----------
	commandQueue->Signal(fence.Get(), fenceValue);

	// ----------コマンド完了待ち----------
	// Fenceの値が指定したSignal値にたどり着いているか確認する
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (fence->GetCompletedValue() < fenceValue) {

		// 指定したSignalにたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		// イベント待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// ----------コマンドアロケータのリセット----------
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));

	// ----------コマンドリストのリセット----------
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void RenderTexture::Finalize() {

	// 各オブジェクトの解放
	CloseHandle(fenceEvent);

	delete instance;
	instance = nullptr;
}

void RenderTexture::CommandRelatedInitialize() {

	// ----------コマンドアロケータ生成----------
	hr = dxUtility->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	// コマンドアロケーターの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// ----------コマンドリスト生成----------
	hr = dxUtility->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	// コマンドリストの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));

	// ----------コマンドキュー生成----------
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	// デバッガの機能の終了後に停止させないで警告を表示
	hr = dxUtility->GetDevice()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	// コマンドキューの生成がうまくいかなかったので起動できない
	assert(SUCCEEDED(hr));
}

void RenderTexture::DescriptorHeapGenerate() {

	// RTV用のデスクリプタサイズを取得
	rtvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTV用のデスクリプタヒープを生成
	rtvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false); // 数は2 シェーダで使わない

	// DSV用のデスクリプタサイズを取得
	dsvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// DSV用のデスクリプタヒープを生成
	dsvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false); // 数は1 シェーダで使わない
}

void RenderTexture::RenderTargetViewInitialize() {
	
	// Resourceの生成
	auto renderTextureResource = CreateRenderTextureResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_R8G8B8A8_UNORM, // フォーマット
		kRenderTargetClearValue // クリアカラー
	);

	// RTV用の設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // フォーマット
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// デスクリプタヒープの先頭を取得
	D3D12_CPU_DESCRIPTOR_HANDLE startHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (uint32_t i = 0; i < 2; ++i) {

		// RTVハンドルを取得
		rtvHandles[i] = startHandle;
		rtvHandles[i].ptr += rtvDescriptorSize * i;
		
		// RTV生成
		dxUtility->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, rtvHandles[i]);
	}
}

void RenderTexture::DepthStencilViewInitialize() {

	// Resourceの生成
	auto depthStencilResource = CreateDepthStencilResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_D24_UNORM_S8_UINT, // フォーマット
		kDepthClearValue // クリア深度
	);

	// DSV用の設定
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// DSV生成
	dxUtility->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvHandle);
}

void RenderTexture::FenceInitialize() {

	// ----------フェンス生成----------
	hr = dxUtility->GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	// FenceのSignalを持つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void RenderTexture::ViewportRectInitialize() {

	// ----------ビューポート矩形の設定----------
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = WinApp::kClientWidth;
	viewportRect.Height = WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void RenderTexture::ScissoringRectInitialize() {

	// ----------シザリング矩形の設定----------
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}

ComPtr<ID3D12Resource> RenderTexture::CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {

	// Resourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.Format = format; // Textureのフォーマット
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // RenderTargetとして使う

	// Heapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// ClearValueの設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	// Resourceの生成
	ComPtr <ID3D12Resource> resource = nullptr;

	dxUtility->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_RENDER_TARGET, // RenderTargetとして使う
		&clearValue, // Clear最適値。ClearRenderTargetをこの色でClearする。最適化なので高速
		IID_PPV_ARGS(&resource) // 作成するResourceポインタへのポインタ
	);

	return resource;
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderTexture::CreateDepthStencilResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const float clearDepth) {
	
	// Resourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数	
	resourceDesc.DepthOrArraySize = 1; // 奥行きor配列Textureの配列数
	resourceDesc.Format = format; // Textureのフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う

	// Heapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// ClearValueの設定
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = format;
	clearValue.DepthStencil.Depth = clearDepth;

	// Resourceの生成
	ComPtr <ID3D12Resource> resource = nullptr;

	dxUtility->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // DepthStencilとして使う
		&clearValue, // Clear最適値。
		IID_PPV_ARGS(&resource) // 作成するResourceポインタへのポインタ
	);

	return resource;
}

RenderTexture* RenderTexture::instance = nullptr;

RenderTexture* RenderTexture::GetInstance() {
	
	if (instance == nullptr) {
		instance = new RenderTexture();
	}
	return instance;
}
