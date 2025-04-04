#include "SwapChain.h"
#include "winApp/WinApp.h"
#include "DirectXUtility.h"

#include <cassert>
#include <thread>

void SwapChain::Initialize(WinApp* winApp, DirectXUtility* dxUtility) {

	// 引数をメンバ変数に設定
	this->winApp = winApp;
	this->dxUtility = dxUtility;

	// FPS固定初期化
	InitializeFixFPS();

	// コマンド関連の初期化
	CommandRelatedInitialize();

	// スワップチェーンの生成
	SwapChainGenerate();

	// フェンスの初期化
	FenceInitialize();

	// ビューポート矩形の初期化
	ViewportRectInitialize();

	// シザリング矩形の初期化
	ScissoringRectInitialize();
}

void SwapChain::PreDraw() {

	// ----------バックバッファの番号取得----------
	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();

	// ----------リソースバリアで書き込み可能に変更----------

	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	// 遷移前(現在)のResouceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	// 遷移後のResouceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// TransitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	// ----------描画先のRTVとDSVを指定する----------

	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex], false, &dsvHandle);

	// ----------画面全体の色をクリア----------
	float clearColor[] = { 0.1f,0.25f,0.5f,1.0f }; // 青っぽい色。RGBAの順
	commandList->ClearRenderTargetView(rtvHandles[backBufferIndex], clearColor, 0, nullptr);

	// ----------画面全体の深度をクリア----------
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ----------ビューポート領域の設定----------
	commandList->RSSetViewports(1, &viewportRect);

	// ----------シザー矩形の設定----------
	commandList->RSSetScissorRects(1, &scissorRect);
}

void SwapChain::PostDraw() {

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

	// FPS固定更新
	UpdateFixFPS();

	// ----------コマンドアロケータのリセット----------
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));

	// ----------コマンドリストのリセット----------
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void SwapChain::Finalize() {

	// 各オブジェクトの解放
	CloseHandle(fenceEvent);
}

void SwapChain::CommandRelatedInitialize() {

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

void SwapChain::SwapChainGenerate() {

	// ----------スワップチェーン生成の設定----------
	swapChainDesc.Width = WinApp::kClientWidth;   // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Height = WinApp::kClientHeight; // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2; // ダブルバッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタにうつしたら、中身を破棄

	// ----------スワップチェーン生成----------
	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxUtility->GetDXGIFactory()->CreateSwapChainForHwnd(commandQueue.Get(), winApp->GetHwnd(), &swapChainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void SwapChain::VariousDescriptorHeapGenerate() {

	// ----------RTV用のDescriptorSizeを取得----------
	rtvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// ----------DSV用のDescriptorSizeを取得----------
	dsvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// ----------RTV用のDescriptorHeap生成----------
	// RTV用のヒープでディスクリプタの数はダブルバッファ用に2つ。RTVはShader内で触るものではないので、ShaderVisibleはfalse
	rtvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);

	// ----------DSV用のDescriptorHeap生成----------
	// DSV用のヒープでディスクリプタの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	dsvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
}

void SwapChain::RenderTargetViewInitialize() {

	// ----------スワップチェーンからリソースを引っ張ってくる----------
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	// うまく取得できなければ起動できない
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));

	// ----------RTV用の設定----------
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャとして書き込む

	// ----------RTVハンドルの要素数を2個に変更する----------
	// ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle = rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// ディスクリプタのインクリメントサイズを取得
	UINT rtvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 裏表の2つ分
	for (uint32_t i = 0; i < 2; ++i) {

		// ----------RTVハンドルを取得----------
		rtvHandles[i] = rtvStartHandle;
		rtvHandles[i].ptr += rtvDescriptorSize * i;

		// ----------レンダーターゲットビューの生成----------
		dxUtility->GetDevice()->CreateRenderTargetView(swapChainResources[i].Get(), &rtvDesc, rtvHandles[i]);
	}
}

void SwapChain::DepthBufferGenerate() {

	// ----------深度バッファリソース設定----------
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinApp::kClientWidth; // Textureの幅
	resourceDesc.Height = WinApp::kClientHeight; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Texureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	// ----------利用するヒープの設定----------
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	// ----------深度値のクリア設定----------
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	// ----------深度バッファの生成----------
	depthStencilResource = nullptr;

	// ----------リソースの生成----------
	HRESULT hr = dxUtility->GetDevice()->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし。
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&depthStencilResource) // 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));
}

void SwapChain::DepthStencilViewInitialize() {

	// ----------DSV用の設定----------
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2Dテクスチャ

	// ----------DSV用をデスクリプタヒープの先頭につくる----------
	dxUtility->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

void SwapChain::FenceInitialize() {

	// ----------フェンス生成----------
	hr = dxUtility->GetDevice()->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));

	// FenceのSignalを持つためのイベントを作成する
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void SwapChain::ViewportRectInitialize() {

	// ----------ビューポート矩形の設定----------
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = WinApp::kClientWidth;
	viewportRect.Height = WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void SwapChain::ScissoringRectInitialize() {

	// ----------シザリング矩形の設定----------
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}

void SwapChain::InitializeFixFPS() {

	// 現在時間を記録する
	reference = std::chrono::steady_clock::now();
}

void SwapChain::UpdateFixFPS() {

	// 1/60秒ピッタリの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回記録からの経過時間を取得する
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference);

	// 1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference < kMinTime) {
			// 1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference = std::chrono::steady_clock::now();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetRTVCPUDescriptorHandle(uint32_t index) {

	return dxUtility->GetCPUDescriptorHandle(rtvDescriptorHeap, rtvDescriptorSize, index);
}

D3D12_GPU_DESCRIPTOR_HANDLE SwapChain::GetRTVGPUDescriptorHandle(uint32_t index) {

	return dxUtility->GetGPUDescriptorHandle(rtvDescriptorHeap, rtvDescriptorSize, index);
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetDSVCPUDescriptorHandle(uint32_t index) {
	
	return dxUtility->GetCPUDescriptorHandle(dsvDescriptorHeap, dsvDescriptorSize, index);
}

D3D12_GPU_DESCRIPTOR_HANDLE SwapChain::GetDSVGPUDescriptorHandle(uint32_t index) {

	return dxUtility->GetGPUDescriptorHandle(dsvDescriptorHeap, dsvDescriptorSize, index);
}
