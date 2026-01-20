#pragma once

#include "CameraControll/ICameraController.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "MathRandom.h"

#include <memory>

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
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 追従対象のセッター
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(WorldTransform* target) { this->target = target; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 追従対象
	const WorldTransform* target = nullptr;

	// 追従対象のオフセット
	Vector3 baseOffset = { 0.0f, 3.0f, -25.0f };

	// 追従の補間
	float followLerp_ = 0.1f;

	/// ===== シェイク用のパラメータ ===== ///

	bool isShaking_ = false;           // シェイク中かどうか
	float shakeTimer_ = 0.0f;          // シェイクタイマー
	float shakeDuration_ = 0.0f;       // シェイク持続時間
	float shakeIntensity_ = 0.0f;      // シェイク強度
	Vector3 shakeOffset_ = { 0.0f, 0.0f, 0.0f }; // シェイクオフセット
	Vector3Range shakeRange_ = { {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f} }; // シェイク範囲
};

