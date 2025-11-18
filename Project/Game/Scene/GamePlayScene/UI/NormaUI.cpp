#define NOMINMAX

#include "NormaUI.h"

#include <algorithm>
#include <imgui.h>

NormaUI::NormaUI() {

	// テクスチャ読み込み
	textureManager_->GetInstance()->LoadTexture("Resources/Norma/NormaText.png");
	textureManager_->GetInstance()->LoadTexture("Resources/Norma/Slash.png");
	textureManager_->GetInstance()->LoadTexture("Resources/Numbers.png");
}

void NormaUI::Initialize() {

	// 固定テキストスプライトの生成
	normaTextSprite_ = std::make_unique<Sprite>();
	normaTextSprite_->Initialize("Resources/Norma/NormaText.png");
	normaTextSprite_->SetAnchorPoint({ 0.5f, 0.5f }); // 左中央揃え

	slashSprite_ = std::make_unique<Sprite>();
	slashSprite_->Initialize("Resources/Norma/Slash.png");
	slashSprite_->SetAnchorPoint({ 0.5f, 0.5f }); // 中央揃え

	// 数字用スプライトの事前準備（最大桁数分を確保）
	for (int i = 0; i < kMaxDigits; ++i) {
		// 現在値用スプライト
		auto currentSprite = std::make_unique<Sprite>();
		currentSprite->Initialize("Resources/Numbers.png");
		currentSprite->SetAnchorPoint({ 0.5f, 0.5f });
		currentSprite->SetSize(digitSize_);
		CurrentNumSprites_.push_back(std::move(currentSprite));

		// ノルマ値用スプライト
		auto normaSprite = std::make_unique<Sprite>();
		normaSprite->Initialize("Resources/Numbers.png");
		normaSprite->SetAnchorPoint({ 0.5f, 0.5f });
		normaSprite->SetSize(digitSize_);
		NormaNumSprites_.push_back(std::move(normaSprite));
	}

	// 初期値の設定
	SetCurrentValue(0);
	SetTargetValue(5);

	// スプライトの位置を更新
	UpdateSpritePositions();
}

void NormaUI::Update() {
	
	// 固定テキストスプライトの更新
	normaTextSprite_->Update();
	slashSprite_->Update();

	// 現在値スプライトの更新（使用中のもののみ）
	int currentDigits = GetDigitCount(currentValue_);
	for (int i = 0; i < currentDigits; ++i) {
		CurrentNumSprites_[i]->Update();
	}

	// ノルマ値スプライトの更新（使用中のもののみ）
	int normaDigits = GetDigitCount(targetValue_);
	for (int i = 0; i < normaDigits; ++i) {
		NormaNumSprites_[i]->Update();
	}
}

void NormaUI::Draw() {

	// "ノルマ："の描画
	normaTextSprite_->Draw();

	// 現在値の描画
	int currentDigits = GetDigitCount(currentValue_);
	for (int i = 0; i < currentDigits; ++i) {
		CurrentNumSprites_[i]->Draw();
	}

	// "/"の描画
	slashSprite_->Draw();

	// ノルマ値の描画
	int normaDigits = GetDigitCount(targetValue_);
	for (int i = 0; i < normaDigits; ++i) {
		NormaNumSprites_[i]->Draw();
	}
}

void NormaUI::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("NormaUI");

	int tempCurrent = currentValue_;
	int tempTarget = targetValue_;

	if (ImGui::SliderInt("Current Value", &tempCurrent, 0, 999)) {
		SetCurrentValue(tempCurrent);
	}

	if (ImGui::SliderInt("Target Value", &tempTarget, 1, 999)) {
		SetTargetValue(tempTarget);
	}

	float pos[2] = { basePosition_.x, basePosition_.y };
	if (ImGui::SliderFloat2("Base Position", pos, 0.0f, 1280.0f)) {
		basePosition_ = { pos[0], pos[1] };
		UpdateSpritePositions();
	}

	ImGui::Text("Display: ノルマ： %d / %d", currentValue_, targetValue_);

	ImGui::End();

#endif // USE_IMGUI

}

Vector2 NormaUI::GetNumberUV(int number) {

	// 数字が0-9の範囲外の場合は0として扱う
	number = std::clamp(number, 0, 9);

	// 横一列に並んでいる数字テクスチャから該当する数字のU座標を計算
	float u = static_cast<float>(number * singleNumberWidth_) / numberTextureWidth_;

	return { u, 0.0f };
}

int NormaUI::GetDigitCount(int number) {

	if (number == 0) {
		return 1;
	}

	int count = 0;

	while (number > 0) {

		number /= 10;

		count++;
	}

	return count;
}

int NormaUI::GetDigitAt(int number, int digit) {

	// 指定桁の値を取得（右端が0桁目）
	for (int i = 0; i < digit; i++) {

		number /= 10;
	}

	return number % 10;
}

Vector2 NormaUI::GetNumberPixelPosition(int number) {

	// 数字が0-9の範囲外の場合は0として扱う
	number = std::clamp(number, 0, 9);

	// ピクセル座標での左上位置を計算
	float leftTopX = static_cast<float>(number * singleNumberWidth_);
	return { leftTopX, 0.0f };
}

void NormaUI::UpdateSpritePositions() {

	// 各値の桁数を取得
	int currentDigits = GetDigitCount(currentValue_);
	int normaDigits = GetDigitCount(targetValue_);

	// 各要素の幅を計算
	float normaWidth = textSize_.x;
	float currentWidth = static_cast<float>(currentDigits) * digitSize_.x;
	float slashWidth = symbolSize_.x;
	float targetWidth = static_cast<float>(normaDigits) * digitSize_.x;

	// 全体の表示幅を計算（間隔も含む）
	float totalWidth = normaWidth + spacing_ + currentWidth + spacing_ + slashWidth + spacing_ + targetWidth;

	// 開始X座標を計算（中央揃え）
	float startX = basePosition_.x - (totalWidth * 0.5f);

	// "ノルマ："の位置設定
	normaTextX_ = startX + normaWidth * 0.5f;
	normaTextSprite_->SetPosition({ normaTextX_, basePosition_.y });
	startX += normaWidth + spacing_;

	// 現在値の位置設定
	currentValueX_ = startX;
	for (int i = 0; i < currentDigits; ++i) {
		float digitX = startX + i * digitSize_.x + digitSize_.x * 0.5f;
		CurrentNumSprites_[i]->SetPosition({ digitX, basePosition_.y });
	}
	startX += currentWidth + spacing_;

	// "/"の位置設定
	slashX_ = startX + slashWidth * 0.5f;
	slashSprite_->SetPosition({ slashX_, basePosition_.y });
	startX += slashWidth + spacing_;

	// ノルマ値の位置設定
	targetValueX_ = startX;
	for (int i = 0; i < normaDigits; ++i) {
		float digitX = startX + i * digitSize_.x + digitSize_.x * 0.5f;
		NormaNumSprites_[i]->SetPosition({ digitX, basePosition_.y });
	}
}

void NormaUI::UpdateCurrentValueSprites() {
	int digits = GetDigitCount(currentValue_);

	for (int i = 0; i < digits; ++i) {

		int digit = GetDigitAt(currentValue_, digits - 1 - i); // 左から右の順

		// テクスチャの切り出し設定
		Vector2 leftTopPosition = GetNumberPixelPosition(digit);
		Vector2 textureSize = { singleNumberWidth_, numberTextureHeight_ };

		CurrentNumSprites_[i]->SetTextureLeftTop(leftTopPosition);
		CurrentNumSprites_[i]->SetTextureSize(textureSize);
	}
}

void NormaUI::UpdateTargetValueSprites() {

	int digits = GetDigitCount(targetValue_);

	for (int i = 0; i < digits; ++i) {

		int digit = GetDigitAt(targetValue_, digits - 1 - i); // 左から右の順

		// テクスチャの切り出し設定
		Vector2 leftTopPosition = GetNumberPixelPosition(digit);
		Vector2 textureSize = { singleNumberWidth_, numberTextureHeight_ };

		NormaNumSprites_[i]->SetTextureLeftTop(leftTopPosition);
		NormaNumSprites_[i]->SetTextureSize(textureSize);
	}
}

void NormaUI::SetCurrentValue(int currentValue) {
	currentValue_ = std::clamp(currentValue, 0, 99); // 最大2桁に制限
	UpdateCurrentValueSprites();
	UpdateSpritePositions();
}

void NormaUI::SetTargetValue(int targetValue) {
	targetValue_ = std::clamp(targetValue, 1, 99); // 最大2桁に制限
	UpdateTargetValueSprites();
	UpdateSpritePositions();
}

