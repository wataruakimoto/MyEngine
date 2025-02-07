#include "TextureManager.h"
#include "utility/StringUtility.h"
#include  "base/SrvManager.h"

using namespace StringUtility;

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {

	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Initialize(DirectXCommon* dxCommon) {

	// 引数をメンバ変数に代入
	dxCommon_ = dxCommon;

	// SRVの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}

uint32_t TextureManager::kSRVIndexTop = 1;

void TextureManager::LoadTexture(const std::string& filePath) {

	/// === ファイル読み込み === ///

	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		
		// 読み込み済みなら終了
		return;
	}

	// テクスチャ枚数上限チェック
	assert(SrvManager::GetInstance()->CheckAllocate());

	DirectX::ScratchImage image{};
	// テクスチャファイルを読んでプログラムで扱えるようにする
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	// ミップマップの作成
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	/// === テクスチャデータ追加 === ///

	TextureData& textureData = textureDatas[filePath];

	/// === テクスチャデータ書き込み === ///

	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = dxCommon_->CreateTextureResource(textureData.metaData);

	/// === テクスチャデータ送信 === ///

	dxCommon_->UploadTextureData(textureData.resource, mipImages);

	/// === デスクリプタハンドルの計算 === ///

	textureData.srvIndex = SrvManager::GetInstance()->Allocate();
	textureData.srvHandleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = SrvManager::GetInstance()->GetGPUDescriptorHandle(textureData.srvIndex);

	/// === SRVの生成 === ///

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	// SRVの設定を行う
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
	// 設定を元にSRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {

	// 読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {

		// 読み込み済みなら要素番号を返す
		uint32_t textureIndex = textureDatas[filePath].srvIndex;
		return textureIndex;
	}

	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVGPUHandle(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];
	return textureData.metaData;
}