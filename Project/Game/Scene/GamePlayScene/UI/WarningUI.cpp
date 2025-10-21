#define NOMINMAX

#include "WarningUI.h"

#include <imgui.h>
#include <algorithm>

WarningUI::WarningUI() {

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/Warning.png");
}

void WarningUI::Initialize() {

	// スプライトの生成
	warningSprite_ = std::make_unique<Sprite>();
	// スプライトの初期化
	warningSprite_->Initialize("Resources/Warning.png");
	// スプライトの位置を画面中央に設定
	warningSprite_->SetPosition({ 640.0f,300.0f });
	// スプライトのアンカーポイントを中央に設定
	warningSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	warningSprite_->SetSize({ 0.0f, 0.0f });

	// アニメーション関連の初期化
	isAnimating_ = false;
	isAnimationFinished_ = false;
	animationFrame_ = 0;
}

void WarningUI::Update() {

	// アニメーション中なら
	if (isAnimating_) {

		// アニメーション更新
		AnimationUpdate();
	}

	// スプライトの更新
	warningSprite_->Update();
}

void WarningUI::Draw() {

	// スプライトの描画
	warningSprite_->Draw();
}

void WarningUI::ShowImGui() {

#ifdef _DEBUG

	// スプライトのImGui表示
	warningSprite_->ShowImGui("WarningUI");

	ImGui::Begin("WarningUI");

	// アニメーション情報表示
	ImGui::Text("Animation Frame: %d", animationFrame_);
	ImGui::Text("Is Animating: %s", isAnimating_ ? "true" : "false");
	ImGui::Text("Is Finished: %s", isAnimationFinished_ ? "true" : "false");

	// アニメーション開始ボタン
	if (ImGui::Button("Start Bounce Animation")) {
		StartBounceAnimation();
	}

	// アニメーション設定
	ImGui::SliderInt("Bounce In Duration", &bounceInDuration_, 1, 180);
	ImGui::SliderInt("Wait Duration", &waitDuration_, 1, 180);
	ImGui::SliderInt("Scale Down Duration", &scaleDownDuration_, 1, 180);
	ImGui::SliderFloat2("Start Scale", &startScale_.x, 0.0f, 2.0f);
	ImGui::SliderFloat2("End Scale", &endScale_.x, 0.0f, 3.0f);

	ImGui::End();

#endif // _DEBUG
}

void WarningUI::StartBounceAnimation() {

	// アニメーションフラグを立てる
	isAnimating_ = true;

	// アニメーション終了フラグを下ろす
	isAnimationFinished_ = false;

	// アニメーションフレームをリセット
	animationFrame_ = 0;

	// 状態を初期化
	animationState_ = AnimationStateWarning::BounceIn;

	// 初期サイズ
	Vector2 initialSize = { 0.0f, 0.0f };

	// スプライトのサイズを初期化
	warningSprite_->SetSize(initialSize);
}

void WarningUI::AnimationUpdate() {

	// フレームを進める
	animationFrame_++;

	Vector2 currentScale = { 0.0f, 0.0f };

	switch (animationState_) {
	case AnimationStateWarning::BounceIn:

		// 拡大フェーズ
		if (animationFrame_ <= bounceInDuration_) {
			float t = static_cast<float>(animationFrame_) / static_cast<float>(bounceInDuration_);

			float easedT = EaseOutBounce(t);

			currentScale = {
				startScale_.x + (maxScale_.x - startScale_.x) * easedT,
				startScale_.y + (maxScale_.y - startScale_.y) * easedT
			};
		}
		else {
			// 拡大完了、待機フェーズに移行
			animationState_ = AnimationStateWarning::Wait;
			animationFrame_ = 0;
			currentScale = maxScale_;
		}

		break;

	case AnimationStateWarning::Wait:

		// 待機フェーズ
		if (animationFrame_ <= waitDuration_) {

			currentScale = maxScale_;
		}
		else {

			// 待機完了、縮小フェーズに移行
			animationState_ = AnimationStateWarning::ScaleDown;
			animationFrame_ = 0;
			currentScale = maxScale_;
		}

		break;

	case AnimationStateWarning::ScaleDown:

		// 縮小フェーズ
		if (animationFrame_ <= scaleDownDuration_) {
			float t = static_cast<float>(animationFrame_) / static_cast<float>(scaleDownDuration_);
			float easedT = EaseInOut(t);

			currentScale = {
				maxScale_.x + (endScale_.x - maxScale_.x) * easedT,
				maxScale_.y + (endScale_.y - maxScale_.y) * easedT
			};
		}
		else {
			// 縮小完了、アニメーション終了
			isAnimating_ = false;
			isAnimationFinished_ = true;
			currentScale = endScale_;
		}

		break;
	}

	// サイズを更新
	Vector2 newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
	warningSprite_->SetSize(newSize);
}

float WarningUI::EaseOutBounce(float t) {

	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {

		return n1 * t * t;
	}
	else if (t < 2.0f / d1) {

		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < 2.5f / d1) {

		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {

		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float WarningUI::EaseInOut(float t) {
	
	if (t < 0.5f) {

		return 2.0f * t * t;
	}
	else {
		return -1.0f + (4.0f - 2.0f * t) * t;
	}
}
