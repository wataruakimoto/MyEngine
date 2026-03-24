#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"

#include <memory>

/// ===== 前方宣言 ===== ///

namespace Engine {

	class Object3dRenderer;
	class FilterManager;
	class FogFilter;
	class Input;
	class SceneManager;
}

/// ===== ゲームオーバーシーン ===== ///
class GameOverScene : public BaseScene {

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
	/// フィルター適応のある描画
	/// </summary>
	void DrawFiltered() override;

	/// <summary>
	/// フィルター適応のない描画
	/// </summary>
	void DrawUnfiltered() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラ
	std::unique_ptr<Engine::Camera> camera_ = nullptr;

	// カメラコントローラ
	std::unique_ptr<ICameraController> cameraController_ = nullptr;

	// 床
	std::unique_ptr<Floor> floor_ = nullptr;

	// シリンダー
	std::unique_ptr<Cylinder> cylinder_ = nullptr;

	// オブジェクトレンダラーのインスタンス
	Engine::Object3dRenderer* object3dRenderer_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// インプットのインスタンス
	Engine::Input* input_ = nullptr;

	// シーンマネージャのインスタンス
	Engine::SceneManager* sceneManager_ = nullptr;

	// フォグの借りポインタ
	Engine::FogFilter* fogFilter_ = nullptr;
};