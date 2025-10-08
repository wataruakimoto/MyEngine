#pragma once
#include "2D/Texture/TextureManager.h"
#include "2D/Sprite/Sprite.h"

#include <memory>

/// ===== スタートUI ===== ///
class StartUI {

///-------------------------------------------///
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StartUI();

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
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 表示フラグのセッター
	/// </summary>
	/// <param name="isVisible"></param>
	void SetVisible(bool isVisible) { isVisible_ = isVisible; }

	/// <summary>
	/// タイマーリセット
	/// </summary>
	void ResetBlinkTimer() { blinkTimer_ = 0.0f; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャ
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// アルファ値
	float alpha_ = 1.0f;

	// 表示フラグ
	bool isVisible_ = false;

	// 点滅タイマー
	float blinkTimer_ = 0.0f;

	// 点滅周期（秒）
	const float blinkCycle_ = 2.0f;

	// アルファ値の下限
	const float alphaMin_ = 0.2f;

	// アルファ値の上限
	const float alphaMax_ = 1.0f;
};

