#pragma once

#include "Sprite/Sprite.h"
#include "Vector2.h"

#include <memory>
#include <array>
#include <string>

/// <summary>
/// 制限時間表示UI
/// </summary>
class TimerUI {

	/// ================================================== ///
	/// 構造体
	/// ================================================== ///
public:
	
	/// <summary>
	/// 分秒の桁を表す構造体
	/// </summary>
	struct TimeDigit {
		uint32_t minutesTens; // 分の十の位
		uint32_t minutesOnes; // 分の一の位
		uint32_t secondsTens; // 秒の十の位
		uint32_t secondsOnes; // 秒の一の位
	};

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
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

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 残り時間を分秒の桁に変換
	/// </summary>
	/// <param name="remainingTime">残り時間</param>
	/// <returns>分秒の桁</returns>
	TimeDigit ConvertTimeToDigits(float remainingTime);

	/// <summary>
	/// 表示するスプライトのテクスチャを更新
	/// </summary>
	/// <param name="timeDigit">分秒の桁</param>
	void UpdateDisplaySprites(TimeDigit timeDigit);

	/// <summary>
	/// 数字のテクスチャのフルパスを取得
	/// </summary>
	/// <param name="digit">数字(0~9)</param>
	/// <returns>テクスチャのフルパス</returns>
	std::string GetDigitTexturePath(uint32_t digit);

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	/// <summary>
	/// 残り時間のセッター
	/// </summary>
	/// <param name="remainingTime">残り時間</param>
	void SetRemainingTime(float remainingTime) { remainingTime_ = remainingTime; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ========== スプライト ========== ///

	// 表示スロットのスプライト 分十・分一・秒十・秒一
	std::array<std::unique_ptr<Engine::Sprite>, 4> slotSprites_;

	// コロンのスプライト
	std::unique_ptr<Engine::Sprite> colonSprite_ = nullptr;

	// 画像フォルダのパス
	const std::string kFolderPath_ = "UI/Timer/";

	/// ========== 表示設定 ========== ///

	// タイマーの基準位置
	const Engine::Vector2 kBasePosition = { 640.0f, 36.0f };

	// 数字1つあたりの横幅
	const float kDigitWidth = 48.0f;

	// コロンの横幅
	const float kColonWidth = 24.0f;

	// 残り時間
	float remainingTime_ = 0.0f;

	// 現在の分秒の桁
	TimeDigit currentTimeDigit_ = { 0, 0, 0, 0 };
};

