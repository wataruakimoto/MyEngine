#pragma once

#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Player/Bullet.h"
#include "Enemy/EnemyBullet.h"
#include "Obstacle/Obstacle.h"
#include "Goal/Goal.h"

#include <list>
#include <memory>

/// === 前方宣言 === ///

class GamePlayScene;

namespace Engine {
	class CollisionManager;
}

/// <summary>
/// ゲームオブジェクト管理クラス
/// </summary>
class GameObjectManager {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 透明オブジェクトの描画
	/// </summary>
	void DrawAlpha();

	/// <summary>
	/// ImGuiの表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// クリア
	/// </summary>
	void Clear();

	/// <summary>
	/// ワールド全体をずらす
	/// </summary>
	/// <param name="shiftZ">Z方向のシフト量</param>
	void ShiftWorld(float shiftZ);

	/// <summary>
	/// 全オブジェクトのコライダーを登録
	/// </summary>
	/// <param name="collisionManager">衝突判定マネージャー</param>
	void RegisterAllColliders(Engine::CollisionManager* collisionManager);

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	Player* GetPlayer() const { return player_.get(); }

	const std::list<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }

	const std::list<std::unique_ptr<Bullet>>& GetPlayerBullets() const { return playerBullets_; }

	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() const { return enemyBullets_; }

	const std::list<std::unique_ptr<Obstacle>>& GetObstacles() const { return obstacles_; }

	Goal* GetGoal() const { return goal_.get(); }

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	void SetPlayer(std::unique_ptr<Player> player) { player_ = std::move(player); }

	void AddEnemy(std::unique_ptr<Enemy> enemy) { enemies_.push_back(std::move(enemy)); }

	void AddPlayerBullet(std::unique_ptr<Bullet> bullet) { playerBullets_.push_back(std::move(bullet)); }

	void AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet) { enemyBullets_.push_back(std::move(bullet)); }

	void AddObstacle(std::unique_ptr<Obstacle> obstacle) { obstacles_.push_back(std::move(obstacle)); }

	void SetGoal(std::unique_ptr<Goal> goal) { goal_ = std::move(goal); }

	void SetGamePlayScene(GamePlayScene* scene) { gamePlayScene_ = scene; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ========== 定数 ========== ///
	
	// ループする距離
	const float kLoopDistance = 1000.0f;

	// ずらした量を蓄積
	float worldShiftZ_ = 0.0f;

	/// ========== ポインタ ========== ///

	// プレイヤーのポインタ
	std::unique_ptr<Player> player_ = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

	// 自機の弾のリスト
	std::list<std::unique_ptr<Bullet>> playerBullets_;

	// 敵の弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// 障害物のリスト
	std::list<std::unique_ptr<Obstacle>> obstacles_;

	// ゴールのポインタ
	std::unique_ptr<Goal> goal_ = nullptr;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;
};

