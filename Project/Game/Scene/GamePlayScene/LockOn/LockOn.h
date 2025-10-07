#pragma once

#include "2D/Sprite.h"
#include "2D/TextureManager.h"

#include <list>
#include <tuple>
#include <memory>

/// === 前方宣言 === ///
class Camera;
class Enemy;
class Player;
class Reticle2D;

/// === ロックオン === ///
class LockOn {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LockOn();

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
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:
	
	/// <summary>
	/// ロックオン対象の探索
	/// </summary>
	void SearchTarget();
	
	/// <summary>
	/// ロックオン対象を決める
	/// </summary>
	void DecideTarget();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ターゲットのゲッター
	/// </summary>
	/// <returns></returns>
	Enemy* GetTarget() { return target_; }

	/// <summary>
	/// ロックオン中のゲッター
	/// </summary>
	/// <returns></returns>
	bool IsLockOn() { return isLockOn_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// 敵リストのセッター(追加)
	/// </summary>
	/// <param name="enemies"></param>
	void SetEnemy(Enemy* enemy) { enemies_.push_back(enemy); }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 2Dレティクルのセッター
	/// </summary>
	/// <param name="reticle2D"></param>
	void SetReticle2D(Reticle2D* reticle2D) { reticle2D_ = reticle2D; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	// 敵のリスト
	std::list<Enemy*> enemies_;

	// 自機の借りポインタ
	Player* player_ = nullptr;

	// 2Dレティクルの借りポインタ
	Reticle2D* reticle2D_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ =  TextureManager::GetInstance();

	// スクリーン座標でのロックオン距離の限界値
	const float kDistanceLockOn_ = 75.0f;

	// 3D空間でのロックオン距離の限界値
	const float kDistanceLockOn3D_ = 150.0f;

	// ロックオン対象リスト
	std::list<std::tuple<float, float, Enemy*>> targets_; // <レティクルからの距離、自機からの距離、敵ポインタ＞

	// ロックオン対象
	Enemy* target_ = nullptr;

	// ロックオン中フラグ
	bool isLockOn_ = false;
};

