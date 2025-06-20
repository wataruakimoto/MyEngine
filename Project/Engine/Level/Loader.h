#pragma once
#include <string>

///-------------------------------------------/// 
/// ローダー
///-------------------------------------------///
class Loader {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// レベルデータファイルの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadLevel(const std::string& filePath);

	/// <summary>
	/// オブジェクトの配置
	/// </summary>
	void PlaceObject();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// デフォルトのディレクトリ
	const std::string kDefaultDirectory = "Resources/Levels/";

	// レベルデータ格納用インスタンス
	LevelData* levelData = nullptr;
};

