#pragma once
#include "Scene/System/BaseScene.h"
#include "Collision/CollisionManager.h"

#include "Player/Player.h"
#include "Player/Bullet.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include "Reticle/Reticle3D.h"
#include "Reticle/Reticle2D.h"
#include "Camera/Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "SkyBox/SkyBoxGame.h"

#include <list>
#include <sstream>
#include <memory>

/// ===== ゲームプレイシーン ===== ///
class GamePlayScene : public BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 自機の弾の追加
	/// </summary>
	/// <param name="bullet"></param>
	void AddPlayerBullet(std::unique_ptr<Bullet> bullet);

	/// <summary>
	/// 敵の弾の追加
	/// </summary>
	/// <param name="bullet"></param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// カメラコントローラ
	std::unique_ptr<ICameraController> cameraController_ = nullptr;

	// 衝突マネージャのポインタ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// プレイヤーのポインタ
	std::unique_ptr<Player> player = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

	// 自機の弾のリスト
	std::list<std::unique_ptr<Bullet>> playerBullets_;

	// 敵の弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// 3Dレティクルのポインタ
	std::unique_ptr<Reticle3D> reticle3D_ = nullptr;

	// 2Dレティクルのポインタ
	std::unique_ptr<Reticle2D> reticle2D_ = nullptr;

	// フロアのポインタ
	std::unique_ptr<Floor> floor_ = nullptr;

	// シリンダーのポインタ
	std::unique_ptr<Cylinder> cylinder_ = nullptr;

	// スカイボックスのポインタ
	std::unique_ptr<SkyBoxGame> skyBox_ = nullptr;

	// 敵を倒した数
	int killCount = 0;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWait_ = true;

	// 待機タイマー
	int32_t standbyTimer_ = 0;
};