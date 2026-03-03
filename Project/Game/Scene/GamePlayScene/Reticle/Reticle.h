#pragma once

#include "Sprite/Sprite.h"
#include "Object/Object3d.h"
#include "Model/Model.h"
#include "WorldTransform.h"

#include <memory>

/// === 前方宣言 === ///
class Camera;

/// ===== レティクル ===== ///
class Reticle {

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
	/// 2D描画
	/// </summary>
	void Draw2D();

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// ワールド変換のゲッター
	/// </summary>
	/// <returns></returns>
	const Engine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// スクリーン座標のゲッター
	/// </summary>
	/// <returns></returns>
	const Engine::Vector2& GetScreenPosition() const { return screenPosition_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Engine::Camera* camera) { this->camera_ = camera; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// レティクルのスプライト
	std::unique_ptr<Engine::Sprite> sprite_ = nullptr;

	// オブジェクト
	std::unique_ptr<Engine::Object3d> object_;

	// モデル
	std::unique_ptr<Engine::Model> model_;

	// 変換データ
	Engine::WorldTransform worldTransform_;

	// スクリーン座標の位置
	Engine::Vector2 screenPosition_ = {};

	// プレイヤーとレティクルの距離
	const float kDistancePlayerToReticle_ = 70.0f;

	// カメラの借りポインタ
	Engine::Camera* camera_ = nullptr;
};

