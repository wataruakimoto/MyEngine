#define NOMINMAX

#include "TimerUI.h"

#include <string>
#include <algorithm>

using namespace Engine;

/// ================================================== ///
/// 初期化
void TimerUI::Initialize() {

	/// ========== スプライトの生成 ========== ///

	// 数字のスプライトを生成 0~9まで
	for (uint32_t i = 0; i < 10; ++i) {
		timeSprites_[i] = std::make_unique<Engine::Sprite>();
		timeSprites_[i]->Initialize("UI/Timer" + std::to_string(i) + ".png");
		timeSprites_[i]->SetAnchorPoint(kAnchorPoint);
	}

	// コロンのスプライトを生成
	colonSprite_ = std::make_unique<Engine::Sprite>();
	colonSprite_->Initialize("UI/Timer/Colon.png");
	colonSprite_->SetAnchorPoint(kAnchorPoint);

	// 位置を初期化 (残り時間が0の状態)
	UpdateDisplaySprites(currentTimeDigit_);
}

/// ================================================== ///
/// 更新
void TimerUI::Update() {

	// 残り時間を分秒の桁に変換
	currentTimeDigit_ = ConvertTimeToDigits(remainingTime_);

	// 表示するスプライトを更新
	UpdateDisplaySprites(currentTimeDigit_);

	// コロンのスプライトを更新
	colonSprite_->Update();
}

/// ================================================== ///
/// 描画
void TimerUI::Draw() {

	// 表示するスプライトを描画
	for (auto& sprite : displaySprites_) {

		if (sprite) {
			sprite->Draw();
		}
	}

	// コロンのスプライトを描画
	colonSprite_->Draw();
}

/// ================================================== ///
/// 残り時間を分秒の桁に変換
TimerUI::TimeDigit TimerUI::ConvertTimeToDigits(float remainingTime) {

	// 残り時間を0~3599秒の範囲に制限
	remainingTime = std::clamp(remainingTime, 0.0f, 3599.0f); // 00:00 ~ 59:59 に制限
	
	// 残り時間を整数秒に変換
	uint32_t totalSeconds = static_cast<uint32_t>(remainingTime);

	// 分に計算
	uint32_t minutes = totalSeconds / 60;
	// 秒に計算
	uint32_t seconds = totalSeconds % 60;

	// 各桁を取得
	TimeDigit result;

	result.minutesTens = minutes / 10; // 分の十の位
	result.minutesOnes = minutes % 10; // 分の一の位
	result.secondsTens = seconds / 10; // 秒の十の位
	result.secondsOnes = seconds % 10; // 秒の一の位

	return result;
}

/// ================================================== ///
/// 表示するスプライトを更新
void TimerUI::UpdateDisplaySprites(TimeDigit timeDigit) {

	// 表示するスプライトを設定
	displaySprites_[0] = timeSprites_[timeDigit.minutesTens].get(); // 分の十の位
	displaySprites_[1] = timeSprites_[timeDigit.minutesOnes].get(); // 分の一の位
	displaySprites_[2] = timeSprites_[timeDigit.secondsTens].get(); // 秒の十の位
	displaySprites_[3] = timeSprites_[timeDigit.secondsOnes].get(); // 秒の一の位

	/// ========== 位置の設定 ========== ///

	// 全体の幅を計算
	float totalWidth = kDigitWidth * 4 + kColonWidth; // [00:00] 数字4桁 + コロン1つ

	// 開始位置を計算
	float startX = kBasePosition.x - totalWidth / 2.0f + kDigitWidth / 2.0f; // 最初の数字の中心位置

	displaySprites_[0]->SetPosition({ startX,								  kBasePosition.y }); // 分の十の位
	displaySprites_[1]->SetPosition({ startX + kDigitWidth,					  kBasePosition.y }); // 分の一の位
	colonSprite_->SetPosition({ startX + kDigitWidth * 2,				  kBasePosition.y }); // コロン
	displaySprites_[2]->SetPosition({ startX + kDigitWidth * 2 + kColonWidth, kBasePosition.y }); // 秒の十の位
	displaySprites_[3]->SetPosition({ startX + kDigitWidth * 3 + kColonWidth, kBasePosition.y }); // 秒の一の位

	/// ========== 表示スプライトの更新 ========== ///
	for (auto& sprite : displaySprites_) {

		if (sprite) {
			sprite->Update();
		}
	}
}
