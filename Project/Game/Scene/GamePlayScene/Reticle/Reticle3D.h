#pragma once

#include "Object/Object3d.h"
#include "Model/Model.h"
#include "WorldTransform.h"

#include <memory>

/// === 前方宣言 === ///
class Player;
class Camera;
class Reticle2D;

/// === 3Dレティクル === ///
class Reticle3D {

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

	void ShowImGui();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera_ = camera; }

	/// <summary>
	/// 2Dレティクルのセッター
	/// </summary>
	/// <param name="reticle2D"></param>
	void SetReticle2D(Reticle2D* reticle2D) { this->reticle2D_ = reticle2D; }

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// オブジェクト
	std::unique_ptr<Object3d> object_;

	// モデル
	std::unique_ptr<Model> model_;

	// 変換データ
	WorldTransform worldTransform_;

	// プレイヤーとレティクルの距離
	const float kDistancePlayerToReticle_ = 50.0f;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;

	// 2Dレティクルの借りポインタ
	Reticle2D* reticle2D_ = nullptr;
};

