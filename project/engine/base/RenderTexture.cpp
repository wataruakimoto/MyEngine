#include "RenderTexture.h"
#include "winApp/WinApp.h"

using namespace Microsoft::WRL;

void RenderTexture::Initialize(DirectXCommon* dxCommon) {

	// NULL検出
	assert(dxCommon);

	// メンバ変数に記録
	this->dxCommon = dxCommon;
}

void RenderTexture::RenderTargetViewInitialize() {

	const Vector4 kRenderTargetClearColor = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤に設定
	
	auto renderTextureResource = CreateRenderTextureResource(
		WinApp::kClientWidth, // クライアント領域の幅
		WinApp::kClientHeight, // クライアント領域の高さ
		DXGI_FORMAT_R8G8B8A8_UNORM, // フォーマット
		kRenderTargetClearColor // クリアカラー
	);

	dxCommon->GetDevice()->CreateRenderTargetView(renderTextureResource.Get(), &, dxCommon->GetRTVHandle(2));
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