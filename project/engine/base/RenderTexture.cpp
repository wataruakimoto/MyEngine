#include "RenderTexture.h"
#include "winApp/WinApp.h"

using namespace Microsoft::WRL;

void RenderTexture::Initialize(DirectXCommon* dxCommon) {

	// NULL検出
	assert(dxCommon);

	// メンバ変数に記録
	this->dxCommon = dxCommon;
}

void RenderTexture::PreDraw() {

	// 描画先のRTVを指定
	dxCommon->GetCommandList()->OMSetRenderTargets(1, &rtvHandles[0], false, nullptr);

	// 画面全体の色をクリア
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	dxCommon->GetCommandList()->ClearRenderTargetView(rtvHandles[0], clearColor, 0, nullptr);

	// 画面全体の震度をクリア
	dxCommon->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// ビューポート矩形の設定
    D3D12_VIEWPORT viewRect = dxCommon->GetViewportRect();
    dxCommon->GetCommandList()->RSSetViewports(1, &viewRect);

	// シザリング矩形の設定
	D3D12_RECT scissorRect = dxCommon->GetScissorRect();
	dxCommon->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void RenderTexture::Finalize() {

	delete instance;
	instance = nullptr;
}

void RenderTexture::DescriptorHeapGenerate() {

	// RTV用のデスクリプタサイズを取得
	rtvDescriptorSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// RTV用のデスクリプタヒープを生成
	rtvDescriptorHeap = dxCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false); // 数は2 シェーダで使わない

	// DSV用のデスクリプタサイズを取得
	dsvDescriptorSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// DSV用のデスクリプタヒープを生成
	dsvDescriptorHeap = dxCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false); // 数は1 シェーダで使わない
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
		dxCommon->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &rtvDesc, rtvHandles[i]);
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
	dxCommon->GetDevice()->CreateDepthStencilView(depthStencilResource.Get(), &dsvDesc, dsvHandle);
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

	dxCommon->GetDevice()->CreateCommittedResource(
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

	dxCommon->GetDevice()->CreateCommittedResource(
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
