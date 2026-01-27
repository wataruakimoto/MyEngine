#pragma once

#include "Sprite/Sprite.h"
#include "Texture/TextureManager.h"

#include <memory>

/// ===== 白フェード ===== ///
class WhiteFade {

///-------------------------------------------/// 
/// 構造体
///-------------------------------------------///
public:

	// フェードタイプ
	enum class FadeType {
		In,
		Out,
	};

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WhiteFade();

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
	/// 白フェードアニメーション開始
	/// </summary>
	void StartFadeAnimation(FadeType type);

	/// <summary>
	/// フェードアニメーションリセット
	/// </summary>
	void ResetFade();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	/// <summary>
	/// フェードが完了しているか
	/// </summary>
	/// <returns></returns>
	bool IsFadeFinished() const { return isFadeFinished_; }

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsFading() const { return isFading_; }

	/// <summary>
	/// 現在のアルファ値
	/// </summary>
	/// <returns></returns>
	float GetAlpha() const { return alpha_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// フェード時間の設定
	/// </summary>
	/// <param name="duration">フェード時間（秒）</param>
	void SetFadeDuration(float duration) { fadeDuration_ = duration; }

	void SetAlpha(float alpha) { alpha_ = alpha; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:
	
	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// アルファ値
	float alpha_ = 0.0f;

	// フェードタイマー
	float fadeTimer_ = 0.0f;

	// フェード時間（秒）
	float fadeDuration_ = 3.0f;

	// フェード中フラグ
	bool isFading_ = false;

	// フェード完了フラグ
	bool isFadeFinished_ = false;

	// デルタタイム
	const float kDeltaTime = 1.0f / 60.0f;

	// フェードタイプ
	FadeType fadeType_ = FadeType::In;
};

