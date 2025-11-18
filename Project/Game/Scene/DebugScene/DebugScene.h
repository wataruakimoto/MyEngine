#pragma once

#include "BaseScene.h"
#include "Camera.h"
#include "Object/Object3dCommon.h"
#include "Scene/GamePlayScene/Goal/Goal.h"

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

	// オブジェクト3D共通部のインスタンス
	Object3dCommon* object3dCommon = Object3dCommon::GetInstance();

	// ゴールのポインタ
	std::unique_ptr<Goal> goal = nullptr;
};