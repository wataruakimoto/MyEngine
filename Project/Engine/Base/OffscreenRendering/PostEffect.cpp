#include "PostEffect.h"
#include "winApp/WinApp.h"
#include "Base/DirectXUtility.h"
#include "base/SrvManager.h"

using namespace Microsoft::WRL;

void PostEffect::Initialize(DirectXUtility* dxUtility) {

	// NULL検出
	assert(dxUtility);

	// メンバ変数に記録
	this->dxUtility = dxUtility;

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

void PostEffect::PreDraw() {

	/// === レンダーテクスチャ用のバリアの設定 === ///

	// バリアはTransition
	renderTextureBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	renderTextureBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象はレンダーテクスチャリソース
	renderTextureBarrier.Transition.pResource = renderTextureResource.Get();

	/// === 深度ステンシル用のバリアの設定 === ///

	// バリアはTransition
	depthStencilBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	depthStencilBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象は深度ステンシルリソース
	depthStencilBarrier.Transition.pResource = depthStencilResource.Get();

	// バリアの状態がレンダーターゲットではなかったらバリアを張る
	if (renderTextureState != D3D12_RESOURCE_STATE_RENDER_TARGET) {

		// 遷移前(現在)のResouceState
		renderTextureBarrier.Transition.StateBefore = renderTextureState;
		// 遷移後のResouceState
		renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		// TransitionBarrierを張る
		dxUtility->GetCommandList()->ResourceBarrier(1, &renderTextureBarrier);

		// 状態をレンダーターゲットにしておく
		renderTextureState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}

	// 描画先のRTVとDSVを指定する
	dxUtility->GetCommandList()->OMSetRenderTargets(1, &rtvHandles[0], false, &dsvHandle);

	// 画面全体の色をクリア
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	dxUtility->GetCommandList()->ClearRenderTargetView(rtvHandles[0], clearColor, 0, nullptr);

	// バリアの状態が深度書き込みではなかったらバリアを張る
	if (depthStencilState != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
	
		// 遷移前(現在)のResouceState
		depthStencilBarrier.Transition.StateBefore = depthStencilState;
		// 遷移後のResouceState
		depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		// TransitionBarrierを張る
		dxUtility->GetCommandList()->ResourceBarrier(1, &depthStencilBarrier);

		// 状態を深度書き込みにしておく
		depthStencilState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}

	// 画面全体の震度をクリア
	dxUtility->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート矩形の設定
	dxUtility->GetCommandList()->RSSetViewports(1, &viewportRect);

	// シザリング矩形の設定
	dxUtility->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void PostEffect::PostDraw() {

	// バリアの状態がピクセルシェーダーではなかったらバリアを張る
	if (renderTextureState != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) {

		// 遷移前(現在)のResouceState
		renderTextureBarrier.Transition.StateBefore = renderTextureState;
		// 遷移後のResouceState
		renderTextureBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		// TransitionBarrierを張る
		dxUtility->GetCommandList()->ResourceBarrier(1, &renderTextureBarrier);

		// 状態をピクセルシェーダーにしておく
		renderTextureState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}

	// バリアの状態がピクセルシェーダーではなかったらバリアを張る
	if (depthStencilState != D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) {

		// 遷移前(現在)のResouceState
		depthStencilBarrier.Transition.StateBefore = depthStencilState;
		// 遷移後のResouceState
		depthStencilBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		// TransitionBarrierを張る
		dxUtility->GetCommandList()->ResourceBarrier(1, &depthStencilBarrier);

		// 状態をピクセルシェーダーにしておく
		depthStencilState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	}
}

void PostEffect::DescriptorHeapGenerate() {

	// RTV用のデスクリプタサイズを取得
	rtvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTV用のデスクリプタヒープを生成
	rtvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false); // 数は2 シェーダで使わない

	// DSV用のデスクリプタサイズを取得
	dsvDescriptorSize = dxUtility->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// DSV用のデスクリプタヒープを生成
	dsvDescriptorHeap = dxUtility->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false); // 数は1 シェーダで使わない
}

void PostEffect::RenderTargetViewInitialize() {
	
	// Resourceの生成
	renderTextureResource = CreateRenderTextureResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, // フォーマット
		kRenderTargetClearValue // クリアカラー
	);

	// RTV用の設定
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // フォーマット
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

void PostEffect::ShaderResourceViewInitialize() {

	// SRV確保
	srvIndex = SrvManager::GetInstance()->Allocate();

	// SRV作成
	SrvManager::GetInstance()->CreateSRVforRenderTexture(srvIndex, renderTextureResource.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
}

void PostEffect::DepthStencilViewInitialize() {

	// Resourceの生成
	depthStencilResource = CreateDepthStencilResource(
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

	// SRV確保
	depthSrvIndex = SrvManager::GetInstance()->Allocate();

	// SRV作成
	SrvManager::GetInstance()->CreateSRVforDepthStencil(depthSrvIndex, depthStencilResource.Get(), DXGI_FORMAT_R24_UNORM_X8_TYPELESS);
}

void PostEffect::ViewportRectInitialize() {

	// ----------ビューポート矩形の設定----------
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewportRect.Width = WinApp::kClientWidth;
	viewportRect.Height = WinApp::kClientHeight;
	viewportRect.TopLeftX = 0;
	viewportRect.TopLeftY = 0;
	viewportRect.MinDepth = 0.0f;
	viewportRect.MaxDepth = 1.0f;
}

void PostEffect::ScissoringRectInitialize() {

	// ----------シザリング矩形の設定----------
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;
}

ComPtr<ID3D12Resource> PostEffect::CreateRenderTextureResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor) {

	// Resourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.DepthOrArraySize = 1; // 2Dテクスチャの配列に合わせるが通常1
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.Format = format; // Textureのフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
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

Microsoft::WRL::ComPtr<ID3D12Resource> PostEffect::CreateDepthStencilResource(uint32_t width, uint32_t height, DXGI_FORMAT format, const float clearDepth) {
	
	// Resourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.DepthOrArraySize = 1; // 奥行きor配列Textureの配列数
	resourceDesc.MipLevels = 1; // mipmapの数
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