#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"
#include "Collision/Basecharacter.h"
#include "Collision/CollisionManager.h"
#include "Particle/ParticleEmitter.h"
#include "Data/Transform.h"

#include <memory>

/// ===== 弾 ===== ///
class Bullet : public BaseCharacter {

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
	/// 死んだかどうかのフラグ
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead; }

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	void OnCollision(Collider* other) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	void SetDirection(const Engine::Vector3& direction) { direction_ = direction; }

	void SetIsLockOn(bool isLockedOn) { isLockedOn_ = isLockedOn; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Engine::Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Engine::Object3d> object = nullptr;

	// 方向
	Engine::Vector3 direction_ = { 0.0f, 0.0f, 1.0f };

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動の速さ
	float moveSpeed = 4.0f;

	// 寿命 (秒)
	const float kLifeTime = 0.5f;

	// デスタイマー
	float deathTimer_ = 0.0f;

	bool isDead = false;

	// エミッターのポインタ
	std::unique_ptr <Engine::ParticleEmitter> particleEmitter = nullptr;

	// ロックオンしているかどうか
	bool isLockedOn_ = false;
};