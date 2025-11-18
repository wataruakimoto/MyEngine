#pragma once

#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"

#include <memory>

/// ===== ノルマUI ===== ///
class NormaUI {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NormaUI();

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

	/// <summary>
	/// 数字のUV座標を取得
	/// </summary>
	/// <param name="number">数字（0-9）</param>
	/// <returns>UV座標</returns>
	Vector2 GetNumberUV(int number);

	/// <summary>
	/// 数値の桁数を取得
	/// </summary>
	/// <param name="number">数値</param>
	/// <returns>桁数</returns>
	int GetDigitCount(int number);

	/// <summary>
	/// 指定した桁の数字を取得
	/// </summary>
	/// <param name="number">数値</param>
	/// <param name="digit">桁位置（0が最下位桁）</param>
	/// <returns>指定桁の数字</returns>
	int GetDigitAt(int number, int digit);

	/// <summary>
	/// 数字のピクセル座標を取得
	/// </summary>
	/// <param name="number">数字（0-9）</param>
	/// <returns>ピクセル座標での左上位置</returns>
	Vector2 GetNumberPixelPosition(int number);

	/// <summary>
	/// スプライトの位置を更新
	/// </summary>
	void UpdateSpritePositions();

	/// <summary>
	/// 現在値用スプライトを確保・設定
	/// </summary>
	void UpdateCurrentValueSprites();

	/// <summary>
	/// ノルマ値用スプライトを確保・設定
	/// </summary>
	void UpdateTargetValueSprites();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	int GetCurrentValue() const { return currentValue_; }

	int GetTargetValue() const { return targetValue_; }

///-------------------------------------------/// 
/// セッター
///-------------------------------------------///
public:

	/// <summary>
	/// 現在の値を設定
	/// </summary>
	/// <param name="currentValue">現在の値</param>
	void SetCurrentValue(int currentValue);

	/// <summary>
	/// 目標値を設定
	/// </summary>
	/// <param name="targetValue">目標値</param>
	void SetTargetValue(int targetValue);

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 固定テキストスプライト
	std::unique_ptr<Sprite> normaTextSprite_;     // "ノルマ:"
	std::unique_ptr<Sprite> slashSprite_;         // "/"

	// 数字用スプライト
	std::vector<std::unique_ptr<Sprite>> NormaNumSprites_;   // ノルマ数（目標値）用
	std::vector<std::unique_ptr<Sprite>> CurrentNumSprites_; // 現在値用

	int currentValue_ = 0;  // 現在の値
	int targetValue_ = 5;   // 目標値

	Vector2 basePosition_ = { 240.0f, 600.0f };  // 基準位置
	const float spacing_ = 10.0f;                 // 要素間の間隔
	const Vector2 digitSize_ = { 40.0f, 56.0f };  // 各数字のサイズ
	const Vector2 textSize_ = { 176.0f, 56.0f };  // "ノルマ："テキストのサイズ
	const Vector2 symbolSize_ = { 32.0f, 56.0f }; // 記号のサイズ

	// 数字テクスチャのUV設定
	const float numberTextureWidth_ = 400.0f;     // 数字テクスチャ全体の幅
	const float numberTextureHeight_ = 56.0f;     // 数字テクスチャの高さ
	const float singleNumberWidth_ = 40.0f;       // 1つの数字の幅

	// 描画時の位置情報
	float normaTextX_ = 0.0f;     // "ノルマ:"のX座標
	float currentValueX_ = 0.0f;  // 現在値のX座標
	float slashX_ = 0.0f;         // "/"のX座標
	float targetValueX_ = 0.0f;   // 目標値のX座標

	// 最大桁数
	static const int kMaxDigits = 2;  // 最大2桁（0-99）

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();
};

