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

	// ワールド変換
	WorldTransform worldTransform;

	// カメラ
	std::unique_ptr<Camera> camera = nullptr;

	// 追従対象
	const WorldTransform* target = nullptr;

	// 追従対象のオフセット
	Vector3 baseOffset = { 0.0f, 2.0f, -10.0f };
};

