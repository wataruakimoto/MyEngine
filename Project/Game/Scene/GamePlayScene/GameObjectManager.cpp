#include "GameObjectManager.h"
#include "GamePlayScene.h"

#include <imgui.h>

void GameObjectManager::Initialize() {

	// フロアを生成
	floor_ = std::make_unique<Floor>();
	floor_->Initialize();

	// シリンダーの生成
	cylinder_ = std::make_unique<Cylinder>();
	cylinder_->Initialize();
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

	// カメラの座標をフロアに設定
	floor_->SetCameraTranslate(gamePlayScene_->GetCameraManager()->GetCamera()->GetWorldTransform().GetWorldPosition());

	// フロアの更新
	floor_->Update();

	// カメラの座標をシリンダーに設定
	cylinder_->SetCameraTranslate(gamePlayScene_->GetCameraManager()->GetCamera()->GetWorldTransform().GetWorldPosition());

	// シリンダーの更新
	cylinder_->Update();

	for (const std::unique_ptr<Enemy>& enemy : enemies_){

		// 敵が死んでいたら
		if (enemy->IsDead()) {
			
			// 敵が死んだときの処理を呼び出す
			gamePlayScene_->OnEnemyDefeated();
		}
	}

	// プレイヤーがゴールより奥にいたら
	if (player_->GetWorldTransform().GetWorldPosition().z >= goal_->GetWorldTransform().GetWorldPosition().z) {

		// ゴールに到達したときの処理を呼び出す
		gamePlayScene_->OnGoalReached();
	}

	// 死んだオブジェクトの削除
	enemies_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->IsDead(); });
	playerBullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->IsDead(); });
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
}

void GameObjectManager::Draw() {

	// シリンダーの描画
	if (cylinder_) cylinder_->Draw();

	// フロアの描画
	if (floor_) floor_->Draw();

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
}

void GameObjectManager::DrawAlpha() {

	// ゴールの描画
	if (goal_) goal_->Draw();
}

void GameObjectManager::ShowImGui() {

	// プレイヤーのImGui
	if (player_) player_->ShowImGui();

	// 敵のImGui
	for (std::unique_ptr<Enemy>& enemy : enemies_) {
		enemy->ShowImGui();
	}

	// プレイヤーの弾のImGui
	for (std::unique_ptr<Bullet>& bullet : playerBullets_) {
		//bullet->ShowImGui();
	}

	// 敵の弾のImGui
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		//bullet->ShowImGui();
	}

	// 障害物のImGui
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->ShowImGui();
	}

	// ゴールのImGui
	if (goal_) goal_->ShowImGui();

	if (floor_) floor_->ShowImGui();

	if (cylinder_) cylinder_->ShowImGui();
}

void GameObjectManager::Clear() {

	// プレイヤーのクリア
	player_.reset();

	// 敵のクリア
	enemies_.clear();

	// プレイヤーの弾のクリア
	playerBullets_.clear();

	// 敵の弾のクリア
	enemyBullets_.clear();

	// 障害物のクリア
	obstacles_.clear();

	// ゴールのクリア
	goal_.reset();
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
}

void GameObjectManager::RegisterAllColliders(Engine::CollisionManager* collisionManager) {

	collisionManager->RegisterCollider(player_->GetCollider());

	for (const std::unique_ptr<Enemy>& enemy : enemies_) {
		collisionManager->RegisterCollider(enemy->GetCollider());
	}

	for (const std::unique_ptr<Bullet>& bullet : playerBullets_) {
		collisionManager->RegisterCollider(bullet->GetCollider());
	}

	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		collisionManager->RegisterCollider(bullet->GetCollider());
	}

	for (const std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		collisionManager->RegisterCollider(obstacle->GetCollider());
	}
}
