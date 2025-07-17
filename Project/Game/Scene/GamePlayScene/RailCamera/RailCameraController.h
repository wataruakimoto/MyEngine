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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// ワールド変換
	WorldTransform worldTransform;

	// カメラのポインタ
	Camera camera;
};

