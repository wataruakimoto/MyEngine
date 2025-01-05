#pragma once

#include "2d/Sprite.h"
#include "3d/Model.h"
#include "3d/Object3d.h"
#include "3d/Camera.h"

/// ===== ゲームシーン ===== ///
class GameScene {

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