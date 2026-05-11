#pragma once

#include "DirectXTex.h"

#include <d3d12.h>
#include <unordered_map>
#include <string>
#include <wrl.h>

namespace Engine {

	/// === 前方宣言 === ///
	class DirectXUtility;

	/// === テクスチャマネージャ === ///
	class TextureManager {

	/// ================================================== ///
	/// シングルトン
	/// ================================================== ///
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


	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
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

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 終了
		/// </summary>
		void Finalize();

		/// ================================================== ///
		/// テクスチャの読み込み

		/// <summary>
		/// テクスチャの読み込み
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		void LoadTexture(const std::string& relativePath);

		/// <summary>
		/// テクスチャの読み込み
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		void LoadTextureFullPath(const std::string& fullPath);

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		/// <summary>
		/// テクスチャの読み込み
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		void LoadTextureBase(const std::string& fullPath);

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// シングルトンインスタンスの取得
		/// </summary>
		/// <returns></returns>
		static TextureManager* GetInstance();

		/// <summary>
		/// テクスチャデータの取得
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>テクスチャデータ</returns>
		const TextureData& GetTextureData(const std::string& fullPath);

		/// <summary>
		/// メタデータの取得
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		/// <returns>メタデータ</returns>
		const DirectX::TexMetadata& GetMetadata(const std::string& relativePath);

		/// <summary>
		/// メタデータを取得
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>メタデータ</returns>
		const DirectX::TexMetadata& GetMetadataFullPath(const std::string& fullPath);

		/// <summary>
		/// SRVインデックスの取得
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		/// <returns>SRVインデックス</returns>
		const uint32_t GetSRVIndex(const std::string& relativePath);

		/// <summary>
		/// SRVインデックスの取得
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>SRVインデックス</returns>
		const uint32_t GetSRVIndexFullPath(const std::string& fullPath);

		/// <summary>
		/// CPUハンドルの取得
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		/// <returns>GPUハンドル</returns>
		const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVGPUHandle(const std::string& relativePath);

		/// <summary>
		/// GPUハンドルの取得
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>GPUハンドル</returns>
		const D3D12_GPU_DESCRIPTOR_HANDLE& GetSRVGPUHandleFullPath(const std::string& fullPath);

		/// <summary>
		/// ベースディレクトリパスのゲッター
		/// </summary>
		/// <returns></returns>
		const std::string& GetBaseDirectoryPath() const { return baseDirectoryPath; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// DirectXUtilityのインスタンス
		DirectXUtility* dxUtility_ = nullptr;

		// テクスチャデータ Key: フルパス
		std::unordered_map<std::string, TextureData> textureDatas;

		// ベースのディレクトリパス
		const std::string baseDirectoryPath = "Resources/Textures";
	};
}