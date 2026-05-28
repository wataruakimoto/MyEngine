#pragma once

#include "LevelLoader.h"

/// === 前方宣言 === ///

class GameObjectManager;

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
	/// <param name="gameObjectManager">オブジェクトマネージャー</param>
	void BuildLevel(const GameLevelData& levelData, GameObjectManager* gameObjectManager);

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// レベルデータから自機を設定
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="gameObjectManager">オブジェクトマネージャー</param>
	void SetPlayerFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager);

	/// <summary>
	/// レベルデータから敵をスポーン
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="gameObjectManager">オブジェクトマネージャー</param>
	void SpawnEnemiesFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager);

	/// <summary>
	/// レベルデータから障害物をスポーン
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="gameObjectManager">オブジェクトマネージャー</param>
	void SpawnObstaclesFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager);

	/// <summary>
	/// レベルデータからゴールを設定
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="gameObjectManager">オブジェクトマネージャー</param>
	void SetGoalFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager);
};

