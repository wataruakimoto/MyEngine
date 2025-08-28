#pragma once
#include "Scene/GamePlayScene/CameraControll/ICameraController.h"
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

#include <memory>

/// ===== レールカメラコントローラー ===== ///
class RailCameraController : public ICameraController {

///-------------------------------------------/// 
/// メンバ変数
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
};

