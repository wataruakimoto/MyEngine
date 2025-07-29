#pragma once

#include "Scene/System/BaseScene.h"
#include "Camera/Camera.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "Level/Loader.h"

/// ===== デバッグシーン ===== ///
class DebugScene : public BaseScene {

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
	/// 描画
	/// </summary>
	void Draw() override;

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
	std::unique_ptr <Camera> camera = nullptr;

	// モデルのポインタ
	std::unique_ptr <Model> modelTerrain = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr <Object3d> terrain = nullptr;

	// レベルローダーのポインタ
	std::unique_ptr<Loader> levelLoader = nullptr;
};