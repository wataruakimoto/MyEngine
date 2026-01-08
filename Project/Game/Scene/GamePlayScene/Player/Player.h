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
#include <numbers>

/// === 前方宣言 === ///
class GamePlayScene;
class Enemy;
class Reticle3D;
class LockOn;
class Camera;

enum class PlayerState {
	AutoPilot,
	Manual,
	Rolling, // バレルロール
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

	/// <summary>
	/// ゲート衝突時の処理
	/// </summary>
	void OnGateCollision();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 射撃アニメーション更新
	/// </summary>
	void FireAnimationUpdate();

	/// <summary>
	/// レティクルに向かって移動
	/// </summary>
	void MoveToReticle();

	/// <summary>
	/// ダメージを受けたときの処理
	/// </summary>
	/// <param name="damage">ダメージ</param>
	void DamageProcess(uint16_t damage);

///-------------------------------------------/// 
/// 状態ごとの処理
///-------------------------------------------///

	void AutoPilotInitialize();

	void AutoPilotUpdate();

	void ManualInitialize();

	void ManualUpdate();

	void RollingInitialize();

	void RollingUpdate();

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

	/// <summary>
	/// HPのゲッター
	/// </summary>
	/// <returns></returns>
	uint16_t GetHP() const { return hp_; }

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

	/// ===== ポインタ・インスタンス ===== ///

	// モデルのポインタ
	std::unique_ptr<Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object = nullptr;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

	// ロックオンの借りポインタ
	LockOn* lockOn_ = nullptr;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	/// ===== 射撃アニメーション用 ===== ///

	Vector3 defaultScale_ = { 1.0f, 1.0f, 1.0f };
	Vector3 fireScale_ = { 0.8f, 0.8f, 0.8f };

	float fireTimer_ = 0.0f; // カウントダウンタイマー
	float fireAnimationTimer_ = 0.0f; // 射撃アニメーション用カウントダウンタイマー
	const float kFireDuration_ = 0.4f; // 射撃間隔 (秒)
	const float kFireAnimationDuration_ = 0.2f; // 射撃アニメーション時間 (秒)

	bool isFiring_ = false;

	/// ===== バレルロール用 ===== ///

	float rollTimer_ = 0.0f;    // 経過時間タイマー
	float rollDuration_ = 0.5f; // ロールにかかる時間（秒）
	int rollDirection_ = 0;     // 回転方向 (-1:左, 1:右)

	const float kRollMoveSpeed_ = 0.4f; // 移動速度
	const float kMaxRollAngle_ = 2.0f * std::numbers::pi_v<float>; // 最大傾き角度 (ラジアン)

	float rollCooldownTimer_ = 0.0f; // クールダウンタイマー
	const float kRollCooldownDuration_ = 0.5f; // クールダウン時間 (秒)

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

	// パーティクルを出したかどうか
	bool isParticleEmitted_ = false;

	/// ===== パラメータ等 ===== ///

	uint16_t hp_ = 5;

	float moveSpeedAuto = 0.5f;

	float moveSpeedManual = 0.5f;

	Vector2 screenPos_ = { 0.0f, 0.0f };

	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動範囲の制限
	const Vector2 kMoveMin = { -20.0f, 0.0f }; // X,Yの最小値 左下
	const Vector2 kMoveMax = { 20.0f, 20.0f }; // X,Yの最大値 右上

	// 状態
	PlayerState state_ = PlayerState::Manual;

	// 状態変更リクエスト
	std::optional<PlayerState> stateRequest_ = std::nullopt;
};