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
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 速度のセッター
	/// </summary>
	/// <param name="velocity"></param>
	void SetVelocity(Vector3 velocity) { this->velocity_ = velocity; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object = nullptr;

	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動の速さ
	float moveSpeed = 1.0f;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 3;

	// デスタイマー
	int32_t deathTimer_ = kLifeTime;

	bool isDead = false;

	// エミッターのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitter = nullptr;
};