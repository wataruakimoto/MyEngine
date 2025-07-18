#pragma once
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

/// ===== レールカメラコントローラー ===== ///
class RailCameraController {

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// ワールド変換
	WorldTransform worldTransform;

	// カメラ
	Camera camera;
};

