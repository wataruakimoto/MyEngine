#pragma once

#include "3D/Model/Model.h"
#include "3D/Object/Object3d.h"
#include "Scene/GamePlayScene/Collision/Basecharacter.h"
#include "Scene/GamePlayScene/Collision/CollisionManager.h"
#include "3d/Particle/ParticleEmitter.h"
#include "3D/Data/Transform.h"

#include <memory>

/// === 前方宣言 === ///
class Player;
class GamePlayScene;

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

	float fireTimer = 60.0f;

	bool isDead = false;

	// パーティクル発生機
	std::unique_ptr <ParticleEmitter> particleEmitter1 = nullptr;

	Transform EmitterTransform1 = {};

	Particle particleSetting1 = {};
};