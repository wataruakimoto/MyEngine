#pragma once

#include "Sprite/Sprite.h"
#include "Model/Model.h"
#include "Object/Object3d.h"
#include "Collision/Basecharacter.h"
#include "Collision/CollisionManager.h"

#include <list>
#include <memory>

/// === 前方宣言 === ///
class GamePlayScene;
class Enemy;
class Reticle3D;
class LockOn;
class Camera;

enum class PlayerMode {
	Title,
	Play,
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

	/// <summary>
	/// 奥にしか移動できないようにする(演出用)
	/// </summary>
	void MoveToZ();

	// ロックオン

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
	/// スクリーン座標のゲッター
	/// </summary>
	/// <returns></returns>
	Vector2 GetScreenPos() { return screenPos_; }

	float GetMoveSpeedTitle() { return moveSpeedTitle; }

	float GetMoveSpeedPlay() { return moveSpeedPlay; }

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
	/// モードの設定
	/// </summary>
	/// <param name="mode"></param>
	void SetPlayerMode(PlayerMode mode) { this->mode_ = mode; }

	void SetMoveSpeedTitle(float speed) { this->moveSpeedTitle = speed; }

	void SetMoveSpeedPlay(float speed) { this->moveSpeedPlay = speed; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// モデルのポインタ
	std::unique_ptr<Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Object3d> object = nullptr;

	float moveSpeedTitle = 0.5f;

	float moveSpeedPlay = 0.5f;

	Vector2 screenPos_ = { 0.0f, 0.0f };

	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	float fireTimer = 60.0f * 0.2f;

	bool isDead = false;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

	// ロックオンの借りポインタ
	LockOn* lockOn_ = nullptr;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	// プレイヤーモード
	PlayerMode mode_ = PlayerMode::Play;
};