#pragma once
#include "3D/Model/Model.h"
#include "3D/Object/Object3d.h"

#include <vector>

class Floor {

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
	void SetCameraTranslate(Vector3 Translate) { cameraTranslate_ = Translate; }

///-------------------------------------------///
/// メンバ変数
///-------------------------------------------///
private:

	// モデル
	std::unique_ptr<Model> model_;

	// 3Dオブジェクトのリスト
	std::vector<std::unique_ptr<Object3d>> objects_;

	// カメラの借りポインタ
	Vector3 cameraTranslate_ = {};
};

