#pragma once

#include "Sprite/Sprite.h"
#include "Vector2.h"

#include <memory>
#include <array>

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
	/// 表示するスプライトを更新
	/// </summary>
	/// <param name="timeDigit">分秒の桁</param>
	void UpdateDisplaySprites(TimeDigit timeDigit);

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

	// 数字のスプライト 0~9
	std::array<std::unique_ptr<Engine::Sprite>, 10> timeSprites_;

	// 表示するスプライト 分秒
	std::array<Engine::Sprite*, 4> displaySprites_;

	// コロンのスプライト
	std::unique_ptr<Engine::Sprite> colonSprite_ = nullptr;

	/// ========== 表示設定 ========== ///

	// タイマーの基準位置
	const Engine::Vector2 kBasePosition = { 20.0f, 20.0f };

	// 数字1つあたりの横幅
	const float kDigitWidth = 20.0f;

	// コロンの横幅
	const float kColonWidth = 10.0f;

	// スプライトのアンカーポイント
	const Engine::Vector2 kAnchorPoint = { 0.5f, 0.5f };

	// 残り時間
	float remainingTime_ = 0.0f;

	// 現在の分秒の桁
	TimeDigit currentTimeDigit_ = { 0, 0, 0, 0 };
};

