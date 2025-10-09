#define NOMINMAX

#include "BlackScreen.h"

#include <algorithm>
#include <imgui.h>

BlackScreen::BlackScreen() {

	// テクスチャの読み込み
	textureManager_->LoadTexture("Resources/BlackScreen.png");
}

void BlackScreen::Initialize() {

	// スプライトの生成
	sprite_ = std::make_unique<Sprite>();

	// スプライトの初期化
	sprite_->Initialize("Resources/BlackScreen.png");
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 最初は黒画面
}

void BlackScreen::Update() {

	// フェードアウト更新
	FadeOutUpdate();

	// スプライトの更新
	sprite_->Update();
}

void BlackScreen::Draw() {

	// スプライトの描画
	sprite_->Draw();
}

void BlackScreen::ShowImGui() {

#ifdef _DEBUG

	// スプライトのImGui表示
	sprite_->ShowImGui("BlackScreen");

	// フェードアウトパラメータ表示
	ImGui::Begin("FadeOut Parameters");
	ImGui::Text("Fade Frame: %d", fadeFrame_);
	ImGui::Text("Fade Duration: %d", fadeDuration_);
	ImGui::Text("Alpha: %.2f", alpha_);
	ImGui::Text("Is Finished Fading Out: %s", isFinishedFadingOut_ ? "True" : "False");
	ImGui::End();

#endif // _DEBUG
}

void BlackScreen::StartFadeOut() {

	// フラグをリセット
	isFinishedFadingOut_ = false;

	// フェードアウトのパラメータをリセット
	fadeFrame_ = 0;
	fadeDuration_ = std::max(1, kFadeDuration * static_cast<int>(60.0f)); // 60フレームでフェードアウト

	// アルファ値を1に設定
	alpha_ = 1.0f;
}

void BlackScreen::FadeOutUpdate() {

	fadeFrame_ = std::min(fadeFrame_ + 1, fadeDuration_);

	float t = static_cast<float>(fadeFrame_) / static_cast<float>(fadeDuration_);

	float k = EaseOutCubic(t);

	alpha_ = 1.0f - k;

	// アルファ値が0~1になるようにクランプ
	alpha_ = std::clamp(alpha_, 0.0f, 1.0f);

	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

	// フェードアウトが完了したら
	if (fadeFrame_ >= fadeDuration_) {

		// フラグを立てる
		isFinishedFadingOut_ = true;
	}
}

float BlackScreen::EaseOutCubic(float t) {
	
	return 1.0f - powf(1.0f - t, 3.0f);
}
