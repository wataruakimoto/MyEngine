#pragma once
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

#include <memory>

/// ===== 追従カメラコントローラー ===== ///
class FollowCameraController{

///-------------------------------------------/// 
/// メンバ関数
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
	std::unique_ptr<Camera> camera = nullptr;

	// 追従対象
	const WorldTransform* target = nullptr;
};

