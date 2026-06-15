#include "LevelBuilder.h"
#include "GameObjectManager.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"
#include "Goal/Goal.h"

void LevelBuilder::BuildLevel(const GameLevelData& levelData, GameObjectManager* gameObjectManager) {

	// プレイヤーの設定
	SetPlayerFromLevelData(levelData, gameObjectManager);

	// 敵の生成
	SpawnEnemiesFromLevelData(levelData, gameObjectManager);

	// 障害物の生成
	SpawnObstaclesFromLevelData(levelData, gameObjectManager);

	// ゴールの設定
	SetGoalFromLevelData(levelData, gameObjectManager);
}

void LevelBuilder::SetPlayerFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager) {

	Player* player = gameObjectManager->GetPlayer();

	player->GetWorldTransform().SetTranslate(levelData.playerPosition);
}

void LevelBuilder::SpawnEnemiesFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager) {

	for (const EnemySpawnData& spawn : levelData.enemySpawnDatas) {
		
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetEnemyType(spawn.type);
		enemy->Initialize();
		enemy->GetWorldTransform().SetTranslate(spawn.position);
		// 敵にプレイヤーのポインタを渡す
		enemy->SetPlayer(gameObjectManager->GetPlayer());
		enemy->SetGameObjectManager(gameObjectManager);

		// 敵をゲームオブジェクトマネージャーに追加
		gameObjectManager->AddEnemy(std::move(enemy));
	}
}

void LevelBuilder::SpawnObstaclesFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager) {

	for (const ObstacleSpawnData& spawn : levelData.obstacleSpawnDatas) {

		std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
		obstacle->Initialize();
		obstacle->GetWorldTransform().SetScale(spawn.scale);
		obstacle->GetWorldTransform().SetTranslate(spawn.position);

		// 障害物をゲームオブジェクトマネージャーに追加
		gameObjectManager->AddObstacle(std::move(obstacle));
	}
}

void LevelBuilder::SetGoalFromLevelData(const GameLevelData& levelData, GameObjectManager* gameObjectManager) {

	// ゴールの生成
	std::unique_ptr<Goal> goal = std::make_unique<Goal>();
	// ゴールの初期化
	goal->Initialize();
	// ゴールの位置をレベルデータから設定
	goal->GetWorldTransform().SetTranslate(levelData.goalPosition);

	// ゴールをゲームオブジェクトマネージャーに追加
	gameObjectManager->SetGoal(std::move(goal));
}
