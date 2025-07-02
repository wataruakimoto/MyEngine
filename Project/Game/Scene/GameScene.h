#pragma once

#include "BaseScene.h"
#include "camera/Camera.h"
#include "Level/Loader.h"
#include "3D/Model.h"
#include "3D/Object3d.h"

#include <memory>

/// ===== ゲームシーン ===== ///
class GameScene : public BaseScene {

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
	std::unique_ptr<Camera> camera = nullptr;

	// レベルローダーのポインタ
	std::unique_ptr<Loader> levelLoader = nullptr;

	// モデル
	std::unique_ptr<Model> model = nullptr;

	// オブジェクト
	std::unique_ptr<Object3d> object = nullptr;
};