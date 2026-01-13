#pragma once

#include "Data/ModelData.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <map>
#include <memory>

/// === モデルマネージャー === ///
class ModelManager {

///-------------------------------------------/// 
/// シングルトン
///-------------------------------------------///
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

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// モデルデータの読み込み
	/// </summary>
	/// <param name="directoryName">ディレクトリ名</param>
	/// <param name="fileName">ファイル名</param>
	void LoadModelData(const std::string& directoryName, const std::string& fileName);

	/// <summary>
	/// モデルデータを検索
	/// </summary>
	/// <param name="directoryName">ディレクトリ名</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns>モデルデータ</returns>
	ModelData* FindModelData(const std::string& directoryName, const std::string& fileName);

	static Node ReadNode(aiNode* node);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 画像ファイルの探索
	/// </summary>
	/// <param name="directoryName">ディレクトリ名</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	std::string FindTextureFilePath(const std::string& directoryName, const std::string& filename);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルデータのマップコンテナ キー : ディレクトリ + ファイル名
	std::map<std::string, std::unique_ptr<ModelData>> modelDatas;

	// ベースのディレクトリパス
	const std::string baseDirectoryPath = "Resources/Models";
};