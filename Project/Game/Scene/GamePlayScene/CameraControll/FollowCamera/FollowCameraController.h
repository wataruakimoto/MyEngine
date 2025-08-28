#pragma once
#include "Scene/GamePlayScene/CameraControll/ICameraController.h"
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

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
	Vector3 baseOffset = { 0.0f, 1.5f, -25.0f };
};

