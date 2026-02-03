#pragma once

#include "CameraControll/ICameraController.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "MathRandom.h"

#include <memory>

/// ===== 前方宣言 ===== ///
class Player;

/// ===== 追従カメラコントローラー ===== ///
class FollowCameraController : public ICameraController {

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
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

	/// <summary>
	/// カメラシェイクを開始
	/// </summary>
	/// <param name="intensity">シェイクの強度</param>
	/// <param name="duration">シェイクの持続時間（秒）</param>
	void StartShake(float intensity, float duration);

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	void Shake();

	void Follow();

	void DutchRoll();

	void DynamicFov();

	void LookAhead();

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { this->player = player; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 追従対象 借りポインタ
	Player* player = nullptr;

	/// ===== シェイク用のパラメータ ===== ///

	bool isShaking_ = false;           // シェイク中かどうか
	float shakeTimer_ = 0.0f;          // シェイクタイマー
	float shakeDuration_ = 0.0f;       // シェイク持続時間
	float shakeIntensity_ = 0.0f;      // シェイク強度
	Vector3 shakeOffset_ = { 0.0f, 0.0f, 0.0f }; // シェイクオフセット
	Vector3Range shakeRange_ = { {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f} }; // シェイク範囲

	/// ===== 追従用 ===== ///

	// 追従対象のオフセット
	Vector3 baseOffset_ = { 0.0f, 4.0f, -12.0f };

	// 追従の補間
	float followLerp_ = 0.1f;

	/// ===== ダッチロール用 ===== ///

	const float kTiltFactor_ = -0.05f; // 傾け具合
	const float kTiltLimit_ = 15.0f;  // 傾きの最大値 (度)
	const float kTiltDelay_ = 0.05f; // 追従の遅延 (0〜1)

	float currentTilt_ = 0.0f; // 現在の傾き角度

	/// ===== ダイナミックFov ===== ///

	const float kFovNormal_ = 75.0f; // 通常移動時の視野角
	const float kFovDash_ = 80.0f; // ダッシュ時の視野角

	/// ===== 先読み用 ===== ///

	Vector3 lookAheadOffset_ = { 0.0f, 0.0f, 0.0f }; // 先読みオフセット

	const float kLookAheadFactor_ = 5.0f; // カメラのズレ具合

	const float kLookAheadDelay_ = 0.1f;  // 追従の遅延 (0〜1)
};

