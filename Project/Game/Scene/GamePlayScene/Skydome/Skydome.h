#pragma once

#include "3d/Model.h"
#include "3d/Object3d.h"

/// ===== 天球 ===== ///
class Skydome {

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

private:

	// モデルのポインタ
	Model* model = nullptr;

	// 3Dオブジェクトのポインタ
	Object3d* object = nullptr;
};