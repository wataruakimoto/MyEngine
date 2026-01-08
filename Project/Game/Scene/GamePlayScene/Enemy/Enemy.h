#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"
#include "Collision/Basecharacter.h"
#include "Collision/CollisionManager.h"
#include "Particle/ParticleEmitter.h"

#include <memory>

/// === 前方宣言 === ///
class Player;
class GamePlayScene;

// 敵の種類
enum class EnemyType {
	Normal,     // 通常（弾を撃つ）
	Kamikaze,   // 特攻（プレイヤーに突っ込む）
};

/// ===== 敵 ===== ///
class Enemy : public Basecharacter {

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
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	void OnCollision(Collider* other) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:
	
	/// <summary>
	/// 自機に向ける
	/// </summary>
	void AimToPlayer();

	/// <summary>
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 射撃アニメーション更新
	/// </summary>
	void FireAnimationUpdate();

	/// <summary>
	/// 前進
	/// </summary>
	void MoveForward();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { this->player = player; }

	/// <summary>
	/// ゲームプレイシーンのセッター
	/// </summary>
	/// <param name="scene"></param>
	void SetGamePlayScene(GamePlayScene* scene) { this->gamePlayScene_ = scene; }

	void SetEnemyType(EnemyType type) { enemyType_ = type; }

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// 死亡フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object = nullptr;

	// プレイヤーの借りポインタ
	Player* player = nullptr;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;

	float moveSpeed = 0.2f;

	bool isDead = false;

	// エミッターBlackのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitterBlack = nullptr;

	// エミッターWhiteのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitterWhite = nullptr;

	/// ===== 射撃アニメーション用 ===== ///

	Vector3 defaultScale_ = { 1.0f, 1.0f, 1.0f };
	Vector3 fireScale_ = { 0.8f, 0.8f, 0.8f };

	float fireTimer_ = 0.0f; // カウントダウンタイマー
	float fireAnimationTimer_ = 0.0f; // 射撃アニメーション用カウントダウンタイマー
	const float kFireDuration_ = 1.0f; // 射撃間隔 (秒)
	const float kFireAnimationDuration_ = 0.2f; // 射撃アニメーション間隔 (秒)

	bool isFiring_ = false;

	EnemyType enemyType_ = EnemyType::Normal;

	// ドリル回転用
	float drillRotation_ = 0.0f; // ドリル回転角度（ラジアン）

	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
};