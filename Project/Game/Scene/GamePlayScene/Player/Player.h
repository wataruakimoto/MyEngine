#pragma once

#include "Sprite/Sprite.h"
#include "Model/Model.h"
#include "Object/Object3d.h"
#include "Collision/Basecharacter.h"
#include "Collision/CollisionManager.h"
#include "Particle/ParticleEmitter.h"
#include "Data/Transform.h"

#include <list>
#include <memory>
#include <optional>

/// === 前方宣言 === ///
class GamePlayScene;
class Enemy;
class Reticle3D;
class LockOn;
class Camera;

enum class PlayerState {
	AutoPilot,
	Manual,
	Dead,
};

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
	/// 衝突時の処理
	/// </summary>
	void OnCollision(Collider* other) override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// レティクルに向かって移動
	/// </summary>
	void MoveToReticle();

///-------------------------------------------/// 
/// 状態ごとの処理
///-------------------------------------------///

	void AutoPilotInitialize();

	void AutoPilotUpdate();

	void ManualInitialize();

	void ManualUpdate();

	void DeadInitialize();

	void DeadUpdate();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// スクリーン座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetScreenPos() { return screenPos_; }

	float GetMoveSpeedTitle() { return moveSpeedAuto; }

	float GetMoveSpeedPlay() { return moveSpeedManual; }

	PlayerState GetState() { return state_; }
	
	bool IsDead() { return isDead_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// ゲームプレイシーンのセッター
	/// </summary>
	/// <param name="scene"></param>
	void SetGamePlayScene(GamePlayScene* scene) { this->gamePlayScene_ = scene; }

	/// <summary>
	/// 3Dレティクルのセッター
	/// </summary>
	/// <param name="reticle"></param>
	void SetReticle3D(Reticle3D* reticle) { this->reticle3D_ = reticle; }

	/// <summary>
	/// ロックオンのセッター
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOn(LockOn* lockOn) { this->lockOn_ = lockOn; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera_ = camera; }

	/// <summary>
	/// 状態変更リクエストのセッター
	/// </summary>
	/// <param name="state"></param>
	void SetPlayerState(PlayerState state) { this->stateRequest_ = state; }

	void SetMoveSpeedAuto(float speed) { this->moveSpeedAuto = speed; }

	void SetMoveSpeedManual(float speed) { this->moveSpeedManual = speed; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object = nullptr;

	float moveSpeedAuto = 0.5f;

	float moveSpeedManual = 0.5f;

	Vector2 screenPos_ = { 0.0f, 0.0f };

	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	float fireTimer = 60.0f * 0.4f;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

	// ロックオンの借りポインタ
	LockOn* lockOn_ = nullptr;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	// 状態
	PlayerState state_ = PlayerState::Manual;

	// 状態変更リクエスト
	std::optional<PlayerState> stateRequest_ = std::nullopt;

	/// ===== 死亡アニメーション用 ===== ///

	float deathTimer_ = 0.0f;

	Vector3 deathVelocity_ = {};

	Vector3 deathRotateVelocity_ = {};

	const float kFallStartSpeed = -0.02f;
	const float kFallAcceleration = -0.001f;
	const float kMaxFallSpeed = -0.2f;

	const float kRollSpeed = 0.1f;
	const float kRollAcceleration = 0.001f;
	const float kRollMaxSpeed = 0.3f;

	const float kSwayAmplitude = 0.1f;
	const float kSwayFrequency = 4.0f;

	const float kGroundHeight = 0.0f;
	const float kParticleSpawnDelay = 0.2f;

	// 地面着地フラグ
	bool isGroundHit_ = false;

	bool isDead_ = false;

	/// ===== パーティクル用 ===== ///

	// エミッターREDのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitterRed = nullptr;

	// エミッターBLUEのポインタ
	std::unique_ptr <ParticleEmitter> particleEmitterBlue = nullptr;

	// エミッタ変換データ
	Transform emitterTransform = {};

	// パーティクル設定
	Particle particleSetting = {};

	// パーティクルを出したかどうか
	bool isParticleEmitted_ = false;

	// 移動範囲の制限
	const Vector2 kMoveMin = { -20.0f, 0.0f }; // X,Yの最小値 左下
	const Vector2 kMoveMax = { 20.0f, 20.0f }; // X,Yの最大値 右上
};