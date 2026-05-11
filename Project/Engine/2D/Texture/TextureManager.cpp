#include "TextureManager.h"
#include "DirectXUtility.h"
#include "SrvManager.h"
#include "StringUtility.h"

using namespace Engine;
using namespace StringUtility;

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

void TextureManager::LoadTexture(const std::string& fileName) {

	// テクスチャのファイルまでのフルパスを生成
	std::string fullPath = baseDirectoryPath + "/" + fileName;

	// テクスチャの読み込み
	LoadTextureBase(fullPath);
}

void TextureManager::LoadTextureFullPath(const std::string& fullPath) {

	// テクスチャの読み込み
	LoadTextureBase(fullPath);
}

void TextureManager::LoadTextureBase(const std::string& fullPath) {

	// 読み込み済みテクスチャならばスキップ
	if (textureDatas.contains(fullPath)) return;

	// テクスチャ枚数上限チェック
	assert(SrvManager::GetInstance()->CheckAllocate());

	DirectX::ScratchImage image{};
	// テクスチャファイルを読んでプログラムで扱えるようにする
	std::wstring filePathW = ConvertString(fullPath);

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

	TextureData& textureData = textureDatas[fullPath];

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

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {

	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

const TextureManager::TextureData& TextureManager::GetTextureData(const std::string& fullPath) {
	
	// 読み込み済みのテクスチャか確認
	assert(textureDatas.contains(fullPath));

	return textureDatas.at(fullPath);
}

const DirectX::TexMetadata& TextureManager::GetMetadata(const std::string& relativePath) {

	// テクスチャのファイルまでのフルパスを生成
	std::string fullPath = baseDirectoryPath + "/" + relativePath;

	return GetTextureData(fullPath).metaData;
}

const DirectX::TexMetadata& TextureManager::GetMetadataFullPath(const std::string& fullPath) {
	
	return GetTextureData(fullPath).metaData;
}

const uint32_t TextureManager::GetSRVIndex(const std::string& relativePath) {

	// テクスチャのファイルまでのフルパスを生成
	std::string fullPath = baseDirectoryPath + "/" + relativePath;

	return GetTextureData(fullPath).srvIndex;
}

const uint32_t TextureManager::GetSRVIndexFullPath(const std::string& fullPath) {

	return GetTextureData(fullPath).srvIndex;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& Engine::TextureManager::GetSRVGPUHandle(const std::string& relativePath) {
	
	// テクスチャのファイルまでのフルパスを生成
	std::string fullPath = baseDirectoryPath + "/" + relativePath;

	return GetTextureData(fullPath).srvHandleGPU;
}

const D3D12_GPU_DESCRIPTOR_HANDLE& TextureManager::GetSRVGPUHandleFullPath(const std::string& fullPath) {

	return GetTextureData(fullPath).srvHandleGPU;
}
