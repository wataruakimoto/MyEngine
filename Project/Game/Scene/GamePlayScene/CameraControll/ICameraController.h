#pragma once

#include "WorldTransform.h"

/// ===== 前方宣言 ===== ///
class Camera;

/// ===== カメラコントローラーのインターフェース ===== ///
class ICameraController {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ICameraController() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ImGui表示
	/// </summary>
	virtual void ShowImGui() = 0;

///-------------------------------------------///
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform() { return worldTransform; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	// カメラのワールド変換
	WorldTransform worldTransform;

	// カメラ(ゲームシーンからの借りポインタ)
	Camera* camera = nullptr;
};