#pragma once

#include "2D/Sprite.h"

#include <memory>

/// === 前方宣言 === ///
class Reticle3D;
class Camera;

/// === 2Dレティクル === ///
class Reticle2D {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Reticle2D();

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
	/// ImGui表示
	/// </summary>
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
	/// 3Dレティクルのセッター
	/// </summary>
	/// <param name="reticle"></param>
	void SetReticle3D(Reticle3D* reticle) { this->reticle3D_ = reticle; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { this->camera_ = camera; }

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// レティクルの位置のゲッター
	/// </summary>
	/// <returns></returns>
	const Vector2& GetReticlePosition() const { return reticlePosition_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// レティクルのスプライト
	std::unique_ptr<Sprite> spriteReticle_ = nullptr;

	// レティクルの位置
	Vector2 reticlePosition_ = { 0.0f, 0.0f };

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

	// カメラの借りポインタ
	Camera* camera_ = nullptr;
};

