#pragma once
#include "WorldTransform/WorldTransform.h"
#include "Camera/Camera.h"

#include <memory>

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
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() const { return worldTransform; }

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <returns></returns>
    Camera& GetCamera() const { return *camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// ワールド変換
	WorldTransform worldTransform;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;
};

