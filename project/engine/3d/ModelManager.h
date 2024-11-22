#pragma once

#include <map>
#include <string>
#include <memory>

#include "ModelCommon.h"
#include "base/DirectXCommon.h"

/// === モデル === ///
class Model;

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
	/// <param name="dxCommon">DirectX基盤</param>
	void Initialize(DirectXCommon* dxCommon);

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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルデータ
	std::map<std::string, std::unique_ptr<Model>> models;

	// モデル共通部のインスタンス
	ModelCommon* modelCommon = nullptr;
};