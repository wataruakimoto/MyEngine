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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	const std::string kDefaultDirectory = "Resources/Levels/";
};

