#pragma once

#include "Camera.h"
#include "CameraControll/ICameraController.h"

#include <memory>
#include <unordered_map>

/// <summary>
/// カメラ管理クラス
/// </summary>
class CameraManager {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
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
	/// カメラコントローラーの追加
	/// </summary>
	/// <param name="name">コントローラーの名前</param>
	/// <param name="controller">コントローラー</param>
	void AddCameraController(const std::string& name, std::unique_ptr<ICameraController> controller);

	/// <summary>
	/// カメラコントローラーの切り替え
	/// </summary>
	/// <param name="name">切り替えるコントローラーの名前</param>
	void SwitchCameraController(const std::string& name);

	/// <summary>
	/// ワールド全体をずらす
	/// </summary>
	/// <param name="shiftZ">Z方向のシフト量</param>
	void ShiftWorld(float shiftZ);

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// カメラのゲッター
	/// </summary>
	/// <returns>カメラ</returns>
	Engine::Camera* GetCamera() const { return camera_.get(); }

	/// <summary>
	/// 現在のカメラコントローラーのゲッター
	/// </summary>
	/// <returns>現在のカメラコントローラー</returns>
	ICameraController* GetCurrentController() const { return currentController_; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// カメラ
	std::unique_ptr<Engine::Camera> camera_ = nullptr;

	// カメラコントローラーのマップ Key:コントローラーの名前
	std::unordered_map<std::string, std::unique_ptr<ICameraController>> cameraControllers_;

	// 現在のカメラコントローラー
	ICameraController* currentController_ = nullptr;
};

