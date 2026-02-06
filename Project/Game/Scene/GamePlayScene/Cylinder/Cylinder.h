#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"

#include <vector>

class Cylinder {

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
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="Translate"></param>
	void SetCameraTranslate(Engine::Vector3 Translate) { cameraTranslate_ = Translate; }

///-------------------------------------------///
/// メンバ変数
///-------------------------------------------///
private:

	// モデル
	std::unique_ptr<Engine::Model> model_;

	// 3Dオブジェクトのリスト
	std::vector<std::unique_ptr<Engine::Object3d>> objects_;

	// カメラの借りポインタ
	Engine::Vector3 cameraTranslate_ = {};

	// 50.0fの壁が20枚で1000で予定
	const float kCylinderLength = 50.0f * 20.0f; // 壁の長さ
};

