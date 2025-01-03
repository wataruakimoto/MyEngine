#pragma once

#include "Framework.h"

#include "2d/Sprite.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "3d/Camera.h"

/// === ゲーム関連 === ///
class MyGame : public Framework {

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

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// カメラのポインタ
	Camera* camera = nullptr;

	// スプライトのポインタ
	Sprite* sprite = nullptr;

	// モデルのポインタ
	Model* model = nullptr;

	// 3Dオブジェクトのポインタ
	Object3d* object3d = nullptr;

	bool useModelManager;
};