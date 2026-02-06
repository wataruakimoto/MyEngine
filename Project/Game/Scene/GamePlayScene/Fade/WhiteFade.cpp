#define NOMINMAX

#include "WhiteFade.h"

#include <imgui.h>
#include <algorithm>

using namespace Engine;

WhiteFade::WhiteFade() {
}

void WhiteFade::Initialize() {

	// スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	// スプライトの初期化
	sprite_->Initialize("White1280x720.png");
	// 画面左上に配置
	sprite_->SetPosition({ 0.0f, 0.0f });
	// 初期は完全透明
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });

	// フェード状態をリセット
	ResetFade();
}

void WhiteFade::Update() {

	// フェード中の場合
	if (isFading_) {

		// タイマーを進める
		fadeTimer_ += kDeltaTime;

		// 進行度を計算（0.0f ～ 1.0f）
		float t = std::clamp(fadeTimer_ / fadeDuration_, 0.0f, 1.0f);

		// タイプによってアルファ値の計算を変える
		if (fadeType_ == FadeType::In) {

			alpha_ = t;          // 0 -> 1
		}
		else {

			alpha_ = 1.0f - t;   // 1 -> 0
		}

		// スプライトの色を更新
		sprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

		// フェードが完了したら
		if (fadeTimer_ >= fadeDuration_) {

			isFading_ = false;
			isFadeFinished_ = true;
		}
	}

	// スプライトの更新
	sprite_->Update();
}

void WhiteFade::Draw() {

	// アルファ値が0より大きい場合のみ描画
	if (alpha_ > 0.0f) {
		sprite_->Draw();
	}
}

void WhiteFade::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("WhiteFade");

	// 現在の状態表示
	ImGui::Text("Alpha: %.3f", alpha_);
	ImGui::Text("Timer: %.3f", fadeTimer_);
	ImGui::Text("Duration: %.3f", fadeDuration_);
	ImGui::Text("Is Fading: %s", isFading_ ? "true" : "false");
	ImGui::Text("Is Finished: %s", isFadeFinished_ ? "true" : "false");

	// フェード時間の調整
	ImGui::SliderFloat("Fade Duration", &fadeDuration_, 0.5f, 10.0f);

	if (ImGui::Button("Start Fade To White")) {
		StartFadeAnimation(FadeType::In);
	}
	if (ImGui::Button("Start Fade From White")) {
		StartFadeAnimation(FadeType::Out);
	}

	// フェードリセットボタン
	if (ImGui::Button("Reset Fade")) {
		ResetFade();
	}

	ImGui::End();

	// スプライトのImGui表示
	sprite_->ShowImGui("WhiteFade Sprite");

#endif // USE_IMGUI
}

void WhiteFade::StartFadeAnimation(FadeType type) {

	// フェード状態をリセット
	fadeTimer_ = 0.0f;
	isFading_ = true;
	isFadeFinished_ = false;
	fadeType_ = type;

	if(fadeType_ == FadeType::In) {
		// スプライトの色を透明に設定
		sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
		alpha_ = 0.0f;
	}
	else {
		// スプライトの色を不透明に設定
		sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		alpha_ = 1.0f;
	}
}

void WhiteFade::ResetFade() {

	// フェード状態をリセット
	fadeTimer_ = 0.0f;
	alpha_ = 0.0f;
	isFading_ = false;
	isFadeFinished_ = false;

	// スプライトの色を透明に設定
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
}