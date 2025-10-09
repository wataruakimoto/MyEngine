#pragma once

#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"

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

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// フェードアウト開始
	/// </summary>
	void StartFadeOut();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// フェードアウト更新
	/// </summary>
	void FadeOutUpdate();

	static float EaseOutCubic(float t);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// フェードアウト完了フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsFinishedFadingOut() const { return isFinishedFadingOut_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:


///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// アルファ値
	float alpha_ = 1.0f;

	// フェードアウト速度
	float fadeOutSpeed_ = 0.005f;

	// フェードアウト完了フラグ
	bool isFinishedFadingOut_ = false;

	// フェード用パラメータ
	int fadeFrame_ = 0;
	int fadeDuration_ = 0;

	// フェードアウトを何秒で行うか
	const int kFadeDuration = 1;
};

