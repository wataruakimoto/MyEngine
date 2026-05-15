#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "Light/LightManager.h"
#include "WorldTransform.h"
#include "Object/Object3d.h"
#include "Model/Model.h"

namespace Engine {

	/// ===== 前方宣言 ===== ///

	class SceneManager;
	class Object3dRenderer;
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

/// ================================================== ///
/// メンバ変数
/// ================================================== ///
private:

	// カメラのポインタ
	std::unique_ptr<Engine::Camera> camera = nullptr;

	// シーンマネージャのポインタ
	Engine::SceneManager* sceneManager = nullptr;

	// ライトマネージャのポインタ
	std::unique_ptr<Engine::LightManager> lightManager_ = nullptr;

	// オブジェクトレンダラーのポインタ
	Engine::Object3dRenderer* object3dRenderer = nullptr;

	// ワールド変換
	Engine::WorldTransform worldTransform;

	// オブジェクトのポインタ
	std::unique_ptr<Engine::Object3d> object = nullptr;

	// モデルのポインタ
	std::unique_ptr<Engine::Model> model = nullptr;
};