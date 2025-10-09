#pragma once

#include "DirectXTex.h"

#include <d3d12.h>
#include <unordered_map>
#include <string>
#include <wrl.h>

/// === 前方宣言 === ///
class DirectXUtility;

/// === テクスチャマネージャ === ///
class TextureManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
private:

	// インスタンス
	static TextureManager* instance;

	// コンストラクタの隠蔽
	TextureManager() = default;
	// デストラクタの隠蔽
	~TextureManager() = default;
	// コピーコンストラクタの封印
	TextureManager(TextureManager&) = delete;
	// コピー代入演算子の封印
	TextureManager& operator=(TextureManager&) = delete;

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// DirectXTexでテクスチャを読む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadTexture(const std::string& filePath);

	/// <summary>
	/// SRVインデックスの開始番号
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	/// <summary>
	/// テクスチャ番号からGPUハンドルを取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(const std::string& filePath);

	/// <summary>
	/// メタデータを取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	const DirectX::TexMetadata& GetMetadata(const std::string& filePath);

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// テクスチャ1枚分のデータ
	struct TextureData {
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource; // 中間リソース
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// DirectXUtilityのインスタンス
	DirectXUtility* dxUtility_ = nullptr;

	// テクスチャデータ
	std::unordered_map<std::string,TextureData> textureDatas;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
};