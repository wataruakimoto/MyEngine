#pragma once

#include "BaseScene.h"
#include "Camera.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class LineManager;
	class FilterManager;
	class TransitionManager;
	class SceneManager;
	class Input;
}

/// ===== デバッグシーン ===== ///
class DebugScene : public BaseScene {

/// ================================================== ///
/// メンバ関数
/// ================================================== ///
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

	// カメラのポインタ
	std::unique_ptr<Engine::Camera> camera = nullptr;

	// 線描画マネージャのポインタ
	Engine::LineManager* lineManager = nullptr;

	// フィルターマネージャのポインタ
	Engine::FilterManager* filterManager = nullptr;

	// 遷移マネージャのポインタ
	Engine::TransitionManager* transitionManager = nullptr;

	// シーンマネージャのポインタ
	Engine::SceneManager* sceneManager = nullptr;

	// 入力のポインタ
	Engine::Input* input = nullptr;
};