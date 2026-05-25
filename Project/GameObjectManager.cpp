#include "GameObjectManager.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Player/Bullet.h"
#include "Enemy/EnemyBullet.h"
#include "Obstacle/Obstacle.h"
#include "Goal/Goal.h"

void GameObjectManager::Initialize() {
}

void GameObjectManager::Update() {

	// プレイヤーの更新
	if (player_) player_->Update();

}

void GameObjectManager::Draw() {
}
