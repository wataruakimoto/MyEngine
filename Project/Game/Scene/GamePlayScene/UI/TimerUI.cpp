#define NOMINMAX

#include "TimerUI.h"

#include <string>
#include <algorithm>

using namespace Engine;

/// ================================================== ///
/// 初期化
void TimerUI::Initialize() {

	/// ========== スプライトの生成 ========== ///

	// 表示スロットのスプライトを生成 (分十・分一・秒十・秒一)
	for (auto& sprite : slotSprites_) {
		sprite = std::make_unique<Engine::Sprite>();
		sprite->Initialize(kFolderPath_ + "0.png");
	}

	// コロンのスプライトを生成
	colonSprite_ = std::make_unique<Engine::Sprite>();
	colonSprite_->Initialize(kFolderPath_ + "Colon.png");

	/// ========== 位置の設定 (各スロットの位置は固定なので初期化時に1度だけ設定) ========== ///

	// 全体の幅を計算
	float totalWidth = kDigitWidth * 4 + kColonWidth; // [00:00] 数字4桁 + コロン1つ

	// 開始位置を計算
	float startX = kBasePosition.x - totalWidth / 2.0f; // 最初の数字の中心位置

	slotSprites_[0]->SetPosition({ startX,								   kBasePosition.y }); // 分の十の位
	slotSprites_[1]->SetPosition({ startX + kDigitWidth,				   kBasePosition.y }); // 分の一の位
	colonSprite_   ->SetPosition({ startX + kDigitWidth * 2,			   kBasePosition.y }); // コロン
	slotSprites_[2]->SetPosition({ startX + kDigitWidth * 2 + kColonWidth, kBasePosition.y }); // 秒の十の位
	slotSprites_[3]->SetPosition({ startX + kDigitWidth * 3 + kColonWidth, kBasePosition.y }); // 秒の一の位

	// 表示するテクスチャを初期状態 (残り時間が0の状態) に更新
	UpdateDisplaySprites(currentTimeDigit_);
}

/// ================================================== ///
/// 更新
void TimerUI::Update() {

	// 残り時間を分秒の桁に変換
	currentTimeDigit_ = ConvertTimeToDigits(remainingTime_);

	// 表示するスプライトのテクスチャを更新
	UpdateDisplaySprites(currentTimeDigit_);

	// スロットのスプライトを更新
	for (auto& sprite : slotSprites_) {
		sprite->Update();
	}

	// コロンのスプライトを更新
	colonSprite_->Update();
}

/// ================================================== ///
/// 描画
void TimerUI::Draw() {

	// 表示するスプライトを描画
	for (auto& sprite : slotSprites_) {
		sprite->Draw();
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
/// 表示するスプライトのテクスチャを更新
void TimerUI::UpdateDisplaySprites(TimeDigit timeDigit) {

	// 各スロットのテクスチャを表示する数字に差し替える
	slotSprites_[0]->SetTexture(GetDigitTexturePath(timeDigit.minutesTens)); // 分の十の位
	slotSprites_[1]->SetTexture(GetDigitTexturePath(timeDigit.minutesOnes)); // 分の一の位
	slotSprites_[2]->SetTexture(GetDigitTexturePath(timeDigit.secondsTens)); // 秒の十の位
	slotSprites_[3]->SetTexture(GetDigitTexturePath(timeDigit.secondsOnes)); // 秒の一の位
}

/// ================================================== ///
/// 数字のテクスチャのフルパスを取得
std::string TimerUI::GetDigitTexturePath(uint32_t digit) {

	return kFolderPath_ + std::to_string(digit) + ".png";
}
