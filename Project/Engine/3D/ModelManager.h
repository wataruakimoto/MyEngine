#pragma once
#include "3d/Model.h"
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
	/// モデルファイル読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadModel(const std::string& filePath);

	/// <summary>
	/// モデルの検索
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>モデル</returns>
	Model* FindModel(const std::string& filePath);

	/// <summary>
	/// .mtlファイル読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// モデルデータ読み込み
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイル名</param>
	/// <returns></returns>
	void LoadModelData(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// モデルデータを検索
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData* FindModelData(const std::string& directoryPath, const std::string& filename);

	static Node ReadNode(aiNode* node);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

	std::map<std::string, std::unique_ptr<ModelData>> modelDatas;
};