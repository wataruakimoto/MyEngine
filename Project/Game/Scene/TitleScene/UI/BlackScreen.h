#pragma once
#include "2D/Texture/TextureManager.h"
#include "2D/Sprite/Sprite.h"

#include <memory>

/// ===== 黒画面 ===== ///
class BlackScreen {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BlackScreen();

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

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// アルファ値のゲッター
	/// </summary>
	/// <returns></returns>
	float GetAlpha() { return alpha_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// アルファ値のセッター
	/// </summary>
	/// <param name="alpha"></param>
	void SetAlpha(float alpha) { alpha_ = alpha; }

	/// <summary>
	/// アルファ値を減少させる
	/// </summary>
	/// <param name="value"></param>
	void SubAlpha(float value) { alpha_ -= value;}

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	float alpha_ = 1.0f;
};

