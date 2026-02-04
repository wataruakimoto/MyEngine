#pragma once

#include "Sprite/Sprite.h"

#include <memory>

/// === 前方宣言 === ///
class Camera;
class Enemy;
class Player;
class Reticle;

/// === ロックオン === ///
class LockOn {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
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
	/// ターゲットの探索
	/// </summary>
	/// <param name="enemies">敵のリスト</param>
	void SearchTarget(const std::list<std::unique_ptr<Enemy>>& enemies);

	/// <summary>
	/// ターゲットのクリア
	/// </summary>
	void ClearTarget() { target_ = nullptr; }

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ターゲットのゲッター
	/// </summary>
	/// <returns></returns>
	Enemy* GetTarget() { return target_; }

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
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// レティクルのセッター
	/// </summary>
	/// <param name="reticle"></param>
	void SetReticle(Reticle* reticle) { reticle_ = reticle; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	/// ===== パラメータ ===== ///

	const float kMaxRadius = 50.0f;   // 最大ロックオン半径

	const float kKeepRadius = 70.0f;  // ロックオン維持半径

	const float kMaxDistanceZ = 75.0f; // 最大ロックオン距離

	// ロックオン対象
	Enemy* target_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	/// ===== 借りポインタ・インスタンス ===== ///

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	// 自機の借りポインタ
	Player* player_ = nullptr;

	// レティクルの借りポインタ
	Reticle* reticle_ = nullptr;
};

