#pragma once
#include "WorldTransform/WorldTransform.h"

/// ===== 前方宣言 ===== ///
class Camera;

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
	WorldTransform worldTransform = {};

	// カメラの借りポインタ
	Camera* camera_ = nullptr;
};

