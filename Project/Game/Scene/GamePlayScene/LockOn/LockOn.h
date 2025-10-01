#pragma once

#include "2D/Sprite.h"
#include "2D/TextureManager.h"

#include <list>
#include <utility>
#include <memory>

/// === 前方宣言 === ///
class Camera;
class Enemy;
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

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

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
	/// 敵リストのセッター
	/// </summary>
	/// <param name="enemies"></param>
	void SetEnemy(Enemy* enemy) { enemies_.push_back(enemy); }

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

	// 2Dレティクルの借りポインタ
	Reticle2D* reticle2D_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ =  TextureManager::GetInstance();

	// スクリーン座標でのロックオン距離の限界値
	const float kDistanceLockOn_ = 10.0f;

	// ロックオン対象リスト
	std::list<std::pair<float, Enemy*>> targets_;

	// ロックオン対象
	Enemy* target_ = nullptr;
};

