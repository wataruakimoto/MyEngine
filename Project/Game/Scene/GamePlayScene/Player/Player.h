#pragma once

#include "2d/Sprite.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "Bullet.h"
#include "Scene/GamePlayScene/Collision/Basecharacter.h"
#include "Scene/GamePlayScene/Collision/CollisionManager.h"

#include <list>

/// === 前方宣言 === ///
class Reticle3D;

/// ===== プレイヤー ===== ///
class Player : public Basecharacter {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:
	
	/// <summary>
	///	初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
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
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	void OnCollision(Collider* other) override;

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// 死亡フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead; }

	/// <summary>
	/// 弾リストのゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Bullet*>* GetBulletList() { return &bullets; }

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform() { return worldTransform_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	void SetReticle3D(Reticle3D* reticle) { this->reticle3D_ = reticle; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	Model* model = nullptr;

	// 3Dオブジェクトのポインタ
	Object3d* object = nullptr;

	// 弾リストのポインタ
	std::list <Bullet*> bullets;

	float moveSpeed = 0.2f;

	float fireTimer = 60.0f * 0.2f;

	bool isDead = false;

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

public:
	// 弾リストを作成
	const std::list<Bullet*>& GetBullets() const { return bullets; }
};