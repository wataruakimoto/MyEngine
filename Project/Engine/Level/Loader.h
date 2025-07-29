#pragma once
#include "LevelData.h"
#include "3D/Object3d.h"

#include <string>
#include <map>
#include <memory>

/// === ローダー === ///
class Loader {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadLevel(const std::string& fileName);

	/// <summary>
	/// オブジェクトの配置
	/// </summary>
	void PlaceObject();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// オブジェクトの解析
	/// </summary>
	void ParseObject(nlohmann::json& object);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// デフォルトのディレクトリ
	const std::string kDefaultDirectory = "Resources/Levels";

	// レベルデータ格納用インスタンス
	std::unique_ptr<LevelData> levelData = nullptr;

	// JSON文字列から読み込んだデータ
	nlohmann::json deserialized;

	// モデルのリスト
	std::map<std::string, std::unique_ptr<Model>> models;

	// オブジェクトのリスト
	std::map<std::string, std::unique_ptr<Object3d>> objects;
};

