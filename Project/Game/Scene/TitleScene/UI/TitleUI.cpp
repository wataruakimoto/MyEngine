#define NOMINMAX

#include "TitleUI.h"
#include "MathVector.h"
#include "Easing.h"

#include <algorithm>
#include <imgui.h>

using namespace MathVector;
using namespace Easing;

TitleUI::TitleUI() {
}

void TitleUI::Initialize() {

	// スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	// スプライトの初期化
	sprite_->Initialize("title.png");
	sprite_->SetPosition({ 640.0f,180.0f }); // 画面中央に表示
	sprite_->SetAnchorPoint({ 0.5f,0.5f }); // 中心を基準に表示
}

void TitleUI::Update() {

	// アニメーション更新
	AnimationUpdate();
	
	// スプライトの更新
	sprite_->Update();
}

void TitleUI::Draw() {

	// スプライトの描画
	sprite_->Draw();
}

void TitleUI::ShowImGui() {

#ifdef USE_IMGUI
	
	// スプライトのImGui表示
	sprite_->ShowImGui("TitleUI");

	// Tweenパラメータ表示
	ImGui::Begin("Tween Parameters");
	ImGui::Text("Tween Start: (%.1f, %.1f)", tweenStart_.x, tweenStart_.y);
	ImGui::Text("Tween End: (%.1f, %.1f)", tweenEnd_.x, tweenEnd_.y);
	ImGui::Text("Tween Frame: %d", tweenFrame_);
	ImGui::Text("Tween Duration: %d", tweenDuration_);
	ImGui::End();

#endif // USE_IMGUI
}

void TitleUI::StartSlideInAnimation() {

	const float width = sprite_->GetSize().x;

	tweenStart_ = { -width, 180.0f };

	tweenEnd_ = { 640.0f, 180.0f };

	sprite_->SetPosition(tweenStart_);

	tweenFrame_ = 0;
	tweenDuration_ = std::max(1, kAnimationDuration * static_cast<int>(60.0f)); // 60フレームでアニメーション

	animationState_ = AnimationState::SlideIn;

	isSlideFinished_ = false;
}

void TitleUI::StartMoveUpAnimation() {

	const float height = sprite_->GetSize().y;

	tweenStart_ = { 640.0f, 180.0f };
	tweenEnd_ = { 640.0f, -height };

	sprite_->SetPosition(tweenStart_);

	tweenFrame_ = 0;
	tweenDuration_ = std::max(1, kAnimationDuration * static_cast<int>(60.0f)); // 60フレームでアニメーション

	animationState_ = AnimationState::MoveUp;

	isMoveUpFinished_ = false;
}

void TitleUI::AnimationUpdate() {

	tweenFrame_ = std::min(tweenFrame_ + 1, tweenDuration_);

	float t = static_cast<float>(tweenFrame_) / static_cast<float>(tweenDuration_);

	float k = 0.0f;

	switch (animationState_) {

	case AnimationState::SlideIn:

		k = EaseOutCubic(t);

		break;

	case AnimationState::MoveUp:

		k = EaseInCubic(t);

		break;

	default:
		break;
	}

	const Vector2 newPos = k * (tweenEnd_ - tweenStart_) + tweenStart_;

	sprite_->SetPosition(newPos);

	if (tweenFrame_ >= tweenDuration_) {

		switch (animationState_) {

		case AnimationState::SlideIn:

			isSlideFinished_ = true;

			break;

		case AnimationState::MoveUp:

			isMoveUpFinished_ = true;

			break;

		default:
			break;
		}
		
		animationStateRequest_ = std::nullopt;
	}
}
