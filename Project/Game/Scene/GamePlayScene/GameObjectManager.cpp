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

	// 敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Update();
	}

	// プレイヤーの弾の更新
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		bullet->Update();
	}

	// 敵の弾の更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}

	// 障害物の更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->Update();
	}

	// ゴールの更新
	if (goal_) goal_->Update();

	// 死んだオブジェクトの削除
	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });
	playerBullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->IsDead(); });
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
}

void GameObjectManager::Draw() {

	// プレイヤーの描画
	if (player_) player_->Draw();

	// 敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->Draw();
	}

	// プレイヤーの弾の描画
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		bullet->Draw();
	}

	// 敵の弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw();
	}

	// 障害物の描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->Draw();
	}

	// ゴールの描画
	if (goal_) goal_->Draw();
}

/// ================================================== ///
/// ワールド全体をずらす
void GameObjectManager::ShiftWorld(float shiftZ) {

	// ワールド全体を手前にずらす
	worldShiftZ_ += shiftZ;

	// プレイヤーを手前にずらす
	player_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });

	// 敵を手前にずらす
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 弾を手前にずらす
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		bullet->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 敵の弾を手前にずらす
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// 障害物を手前にずらす
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	}

	// ゴールを手前にずらす
	goal_->GetWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
	goal_->GetGateWorldTransform().AddTranslate({ 0.0f, 0.0f, -shiftZ });
}

/// ================================================== ///
/// オリジンシフトの確認と実行
void GameObjectManager::CheckOriginShift() {

	// プレイヤーのワールド座標のZ座標を取得
	const float playerZ = player_->GetWorldTransform().GetWorldPosition().z;

	// プレイヤーのZ座標がループ距離を超えたら
	if (playerZ >= kLoopDistance) {

		const float shiftZ = kLoopDistance - 100;

		// ワールド全体を手前にずらす
		ShiftWorld(shiftZ);
	}
}
