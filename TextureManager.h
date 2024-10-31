#pragma once
#include "DirectXCommon.h"

/// === テクスチャマネージャ === ///
class TextureManager {

///=====================================================/// 
/// シングルトン
///=====================================================///
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

///=====================================================/// 
/// メンバ関数
///=====================================================///
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

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
	/// <param name="textureIndex">テクスチャ番号</param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(uint32_t textureIndex);

///=====================================================/// 
/// 構造体
///=====================================================///
public:

	// テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

///=====================================================/// 
/// メンバ変数
///=====================================================///
private:

	// DirectXCommonのポインタ
	DirectXCommon* dxCommon_;

	// テクスチャデータ
	std::vector<TextureData> textureDatas;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;
};