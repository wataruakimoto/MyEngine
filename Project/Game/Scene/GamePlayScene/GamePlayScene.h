#pragma once
#include "Scene/System/BaseScene.h"
#include "camera/Camera.h"
#include "Collision/CollisionManager.h"

#include "Skydome/Skydome.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Reticle/Reticle3D.h"
#include "Reticle/Reticle2D.h"
#include "RailCamera/RailCameraController.h"

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
	void ShowImGui();

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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラのポインタ
	std::unique_ptr<Camera> camera = nullptr;

	// レールカメラコントローラーのポインタ
	std::unique_ptr<RailCameraController> railCameraController_ = nullptr;

	// 衝突マネージャのポインタ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// 天球のポインタ
	Skydome* skydome = nullptr;

	// プレイヤーのポインタ
	Player* player = nullptr;

	// 敵のリスト
	std::list<Enemy*> enemies_;

	// 弾のリスト
	std::list<Bullet*> bullets_;

	// 3Dレティクルのポインタ
	std::unique_ptr<Reticle3D> reticle3D_ = nullptr;

	// 2Dレティクルのポインタ
	std::unique_ptr<Reticle2D> reticle2D_ = nullptr;

	// 敵を倒した数
	int killCount = 0;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWait_ = true;

	// 待機タイマー
	int32_t standbyTimer_ = 0;
};