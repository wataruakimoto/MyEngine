#include "TextureManager.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "StringUtility.h"

using namespace StringUtility;

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {

	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Initialize() {

	// DirectXUtilityのインスタンスを取得
	dxUtility_ = DirectXUtility::GetInstance();

	// SRVの数と同数
	textureDatas.reserve(SrvManager::kMaxSRVCount);
}

void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}

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

	HRESULT hr;

	if (filePathW.ends_with(L".dds")) { // .ddsファイルの場合。より安全な方法はいくらでもある
		
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image); // sRGBまで含めたフォーマット情報が格納されているので、FLAGは立てない
	}
	else { // WICファイルの場合

		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}

	assert(SUCCEEDED(hr));

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};

	if (DirectX::IsCompressed(image.GetMetadata().format)) { // 圧縮フォーマットなら

		mipImages = std::move(image); // 圧縮されていたらそのままimageを使うのでmoveする
	}
	else { // 圧縮されていないフォーマットなら

		// 画像サイズが1x1の場合はミップマップ生成をスキップ
		if (image.GetMetadata().width <= 1 && image.GetMetadata().height <= 1) {
			mipImages = std::move(image);
		}
		else {
			hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
			assert(SUCCEEDED(hr));
		}
	}

	assert(SUCCEEDED(hr));

	/// === テクスチャデータ追加 === ///

	TextureData& textureData = textureDatas[filePath];

	/// === テクスチャデータ書き込み === ///

	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = dxUtility_->CreateTextureResource(textureData.metaData);

	/// === テクスチャデータ送信 === ///

	// テクスチャデータをGPUに送信するための中間リソースを生成
	textureData.intermediateResource = dxUtility_->UploadTextureData(textureData.resource, mipImages);

	/// === デスクリプタハンドルの計算 === ///

	textureData.srvIndex = SrvManager::GetInstance()->Allocate();
	textureData.srvHandleCPU = SrvManager::GetInstance()->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = SrvManager::GetInstance()->GetGPUDescriptorHandle(textureData.srvIndex);

	/// === SRVの生成 === ///

	if (textureData.metaData.IsCubemap()) { // キューブマップテクスチャなら

		SrvManager::GetInstance()->CreateSRVforTextureCube(textureData.srvIndex, textureData.resource.Get(), textureData.metaData.format, UINT_MAX);
	}
	else { // 2Dテクスチャなら

		SrvManager::GetInstance()->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metaData.format, UINT(textureData.metaData.mipLevels));
	}
}

void TextureManager::LoadTextureRelative(const std::string& relativePath) {

	// フルパスを生成
	std::string fullPath = baseDirectoryPath + "/" + relativePath;

	// テクスチャ読み込み
	LoadTexture(fullPath);
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

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVGPUHandle(const uint32_t srvIndex) {

	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle = SrvManager::GetInstance()->GetGPUDescriptorHandle(srvIndex);
	return GPUHandle;
}

const DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& filePath) {

	TextureData& textureData = textureDatas[filePath];
	return textureData.metaData;
}