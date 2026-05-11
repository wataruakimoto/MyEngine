#pragma once

#include "Data/ModelData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <map>
#include <memory>

namespace Engine {

	/// === モデルマネージャー === ///
	class ModelManager {

	/// ================================================== ///
	/// シングルトン
	/// ================================================== ///
	private:

		// インスタンス
		static ModelManager* instance;

		// コンストラクタ(隠蔽)
		ModelManager() = default;

		// デストラクタ(隠蔽)
		~ModelManager() = default;

		// コピーコンストラクタ(封印)
		ModelManager(ModelManager&) = delete;

		// コピー代入演算子(封印)
		ModelManager& operator=(ModelManager&) = delete;

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

		static Node ReadNode(aiNode* node);

		/// ================================================== ///
		/// モデル読み込み
	
		/// <summary>
		/// モデルの読み込み
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		void LoadModel(const std::string& relativePath);

		/// <summary>
		/// モデルの読み込み
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		void LoadModelFullPath(const std::string& fullPath);

		/// ================================================== ///
		/// モデルの探索
		
		/// <summary>
		/// モデルデータを検索
		/// </summary>
		/// <param name="relativePath">相対パス</param>
		/// <returns>モデルデータ</returns>
		ModelData* FindModel(const std::string& relativePath);
		
		/// <summary>
		/// モデルデータを検索
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>モデルデータ</returns>
		ModelData* FindModelFullPath(const std::string& fullPath);

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
	private:

		/// <summary>
		/// モデルの読み込み
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		void LoadModelBase(const std::string& fullPath);

		/// <summary>
		/// 画像ファイルを探索
		/// </summary>
		/// <param name="modelFullPath">モデルのフルパス</param>
		/// <param name="fileName">ファイル名</param>
		/// <returns>画像ファイルのパス</returns>
		std::string FindTextureFilePath(const std::string& modelFullPath, const std::string& fileName);

		/// <summary>
		/// モデルの探索
		/// </summary>
		/// <param name="fullPath">フルパス</param>
		/// <returns>モデルデータ</returns>
		ModelData* FindModelBase(const std::string& fullPath);

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
	public:

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static ModelManager* GetInstance();

		/// <summary>
		/// ベースディレクトリパスのゲッター
		/// </summary>
		/// <returns>ベースディレクトリパス</returns>
		const std::string& GetBaseDirectoryPath() const { return baseDirectoryPath; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
	private:

		// モデルデータのマップコンテナ Key: フルパス
		std::map<std::string, std::unique_ptr<ModelData>> modelDatas;

		// ベースのディレクトリパス
		const std::string baseDirectoryPath = "Resources/Models";
	};
}