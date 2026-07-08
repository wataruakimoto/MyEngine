#include "LevelBuilder.h"
#include "GameObjectManager.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Obstacle/Obstacle.h"
#include "Goal/Goal.h"

void LevelBuilder::BuildLevel(const GameLevelData& levelData) {

	// プレイヤーの設定
	SetPlayerFromLevelData(levelData);

	// 敵の生成
	SpawnEnemiesFromLevelData(levelData);

	// 障害物の生成
	SpawnObstaclesFromLevelData(levelData);

	// ゴールの設定
	SetGoalFromLevelData(levelData);
}

void LevelBuilder::SetPlayerFromLevelData(const GameLevelData& levelData) {

	Player* player = GameObjectManager::GetInstance()->GetPlayer();

	player->GetWorldTransform().SetTranslate(levelData.playerPosition);
}

void LevelBuilder::SpawnEnemiesFromLevelData(const GameLevelData& levelData) {

	for (const EnemySpawnData& spawn : levelData.enemySpawnDatas) {
		
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->SetEnemyType(spawn.type);
		enemy->Initialize();
		enemy->GetWorldTransform().SetTranslate(spawn.position);
		// 敵にプレイヤーのポインタを渡す
		enemy->SetPlayer(GameObjectManager::GetInstance()->GetPlayer());
		enemy->SetGameObjectManager(GameObjectManager::GetInstance());

		// 敵をゲームオブジェクトマネージャーに追加
		GameObjectManager::GetInstance()->AddEnemy(std::move(enemy));
	}
}

void LevelBuilder::SpawnObstaclesFromLevelData(const GameLevelData& levelData) {

	for (const ObstacleSpawnData& spawn : levelData.obstacleSpawnDatas) {

		std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
		obstacle->Initialize();
		obstacle->GetWorldTransform().SetScale(spawn.scale);
		obstacle->GetWorldTransform().SetTranslate(spawn.position);

		// 障害物をゲームオブジェクトマネージャーに追加
		GameObjectManager::GetInstance()->AddObstacle(std::move(obstacle));
	}
}

void LevelBuilder::SetGoalFromLevelData(const GameLevelData& levelData) {

	// ゴールの生成
	std::unique_ptr<Goal> goal = std::make_unique<Goal>();
	// ゴールの初期化
	goal->Initialize();
	// ゴールの位置をレベルデータから設定
	goal->GetWorldTransform().SetTranslate(levelData.goalPosition);

	// ゴールをゲームオブジェクトマネージャーに追加
	GameObjectManager::GetInstance()->SetGoal(std::move(goal));
}
