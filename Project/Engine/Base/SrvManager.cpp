#include "SrvManager.h"
#include "DirectXUtility.h"
#include "SwapChain.h"

#include <cassert>

SrvManager* SrvManager::instance = nullptr;

SrvManager* SrvManager::GetInstance() {

	if (instance == nullptr) {
		instance = new SrvManager;
	}
	return instance;
}

const uint32_t SrvManager::kMaxSRVCount = 512;

void SrvManager::Initialize(DirectXUtility* dxUtility) {

	//　引数をメンバ変数にコピー
	this->dxUtility_ = dxUtility;

	// デスクリプタヒープの作成
	descriptorHeap = dxUtility_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxSRVCount, true);

	// デスクリプタ1個分のサイズを取得して記録
	descriptorSize = dxUtility_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void SrvManager::PreDraw() {

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	dxUtility_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
}

void SrvManager::Finalize() {
	delete instance;
	instance = nullptr;
}

uint32_t SrvManager::Allocate() {

	// 上限に達していないかチェックしてassert
	assert(descriptorSize < kMaxSRVCount);

	/// returnする番号を一旦記録する
	int index = useIndex;

	// 次回のために番号を1進める
	useIndex++;

	// 上で記録した番号をreturn
	return index;
}

bool SrvManager::CheckAllocate() {

	// 上限に達していなければtrue
	if (useIndex < kMaxSRVCount) {

		return true;
	}
	else {

		return false;
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {

	// デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	// ハンドルをindex分進める
	handleCPU.ptr += descriptorSize * index;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {

	// デスクリプタヒープのハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	// ハンドルをindex分進める
	handleGPU.ptr += descriptorSize * index;
	return handleGPU;
}

void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = mipLevels;
	// 設定を元にSRVの生成
	dxUtility_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, uint32_t numElements, UINT structureByteStride) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;
	// 設定を元にSRVの生成
	dxUtility_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	// 設定を元にSRVの生成
	dxUtility_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::CreateSRVforDepthStencil(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	// 設定を元にSRVの生成
	dxUtility_->GetDevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SrvManager::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, uint32_t srvIndex) {

	dxUtility_->GetCommandList()->SetGraphicsRootDescriptorTable(rootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}