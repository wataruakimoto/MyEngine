#pragma once

#include "GameLevelData.h"

#include <string>

/// <summary>
/// レベルローダー
/// </summary>
class LevelLoader {

/// ================================================== ///
/// メンバ関数
/// ================================================== ///
public:

	/// <summary>
	/// レベルデータ読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadLevel(const std::string& fileName);

/// ================================================== ///
/// ゲッター
/// ================================================== ///
public:

	/// <summary>
	/// レベルデータの取得
	/// </summary>
	/// <returns>GameLevelData</returns>
	const GameLevelData& GetLevelData() const { return levelData_; }

/// ================================================== ///
/// メンバ変数 
/// ================================================== ///
private:

	// ゲームのレベルデータ
	GameLevelData levelData_ = {};

	// データのあるディレクトリのパス
	const std::string kDataDirectoryPath_ = "Resources/Datas/GameLevel";
};

