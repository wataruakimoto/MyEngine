#pragma once

#include "LevelLoader.h"

/// <summary>
/// レベルビルダー
/// </summary>
class LevelBuilder {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// レベルデータからオブジェクトを生成
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void BuildLevel(const GameLevelData& levelData);

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// レベルデータから自機を設定
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void SetPlayerFromLevelData(const GameLevelData& levelData);

	/// <summary>
	/// レベルデータから敵をスポーン
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void SpawnEnemiesFromLevelData(const GameLevelData& levelData);

	/// <summary>
	/// レベルデータから障害物をスポーン
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void SpawnObstaclesFromLevelData(const GameLevelData& levelData);

	/// <summary>
	/// レベルデータからゴールを設定
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void SetGoalFromLevelData(const GameLevelData& levelData);
};

