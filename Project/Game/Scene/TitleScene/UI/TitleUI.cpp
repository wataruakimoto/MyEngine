#define NOMINMAX

#include "TitleUI.h"
#include "MathVector.h"
#include "Easing.h"

#include <algorithm>
#include <imgui.h>

using namespace Engine;
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

#endif // USE_IMGUI
}

void TitleUI::StartSlideInAnimation() {

	const float width = sprite_->GetSize().x;

	slideInTween_.start = { -width, 180.0f };

	slideInTween_.end = { 640.0f, 180.0f };

	sprite_->SetPosition(slideInTween_.start);

	slideInTween_.frame = 0;
	slideInTween_.duration = std::max(1, kAnimationDuration * static_cast<int>(60.0f)); // 60フレームでアニメーション

	animationState_ = AnimationState::SlideIn;

	slideInTween_.isFinished = false;
}

void TitleUI::StartMoveUpAnimation() {

	const float height = sprite_->GetSize().y;

	moveUpTween_.start = { 640.0f, 180.0f };
	moveUpTween_.end = { 640.0f, -height };

	sprite_->SetPosition(moveUpTween_.start);

	moveUpTween_.frame = 0;
	moveUpTween_.duration = std::max(1, kAnimationDuration * static_cast<int>(60.0f)); // 60フレームでアニメーション

	animationState_ = AnimationState::MoveUp;

	moveUpTween_.isFinished = false;
}

void TitleUI::StartHoverAnimation() {

	hoverTween_.start = { 640.0f, 180.0f };
	hoverTween_.end = { 640.0f, 200.0f };
	hoverTween_.frame = 0;
	hoverTween_.duration = static_cast<int>(60.0f); // 60フレームでアニメーション
	hoverTween_.isFinished = false;

	isHoverReverse_ = false;

	animationState_ = AnimationState::Hover;
}

void TitleUI::AnimationUpdate() {

	switch (animationState_) {

	case AnimationState::SlideIn: {

		slideInTween_.frame = std::min(slideInTween_.frame + 1, slideInTween_.duration);

		float t = static_cast<float>(slideInTween_.frame) / static_cast<float>(slideInTween_.duration);
		float k = EaseOutCubic(t);

		sprite_->SetPosition(k * (slideInTween_.end - slideInTween_.start) + slideInTween_.start);

		if (slideInTween_.frame >= slideInTween_.duration) {
			slideInTween_.isFinished = true;
		}

		break;
	}

	case AnimationState::MoveUp: {

		moveUpTween_.frame = std::min(moveUpTween_.frame + 1, moveUpTween_.duration);

		float t = static_cast<float>(moveUpTween_.frame) / static_cast<float>(moveUpTween_.duration);
		float k = EaseInCubic(t);

		sprite_->SetPosition(k * (moveUpTween_.end - moveUpTween_.start) + moveUpTween_.start);

		if (moveUpTween_.frame >= moveUpTween_.duration) {
			moveUpTween_.isFinished = true;
		}

		break;
	}

	case AnimationState::Hover: {

		hoverTween_.frame = std::min(hoverTween_.frame + 1, hoverTween_.duration);

		float t = static_cast<float>(hoverTween_.frame) / static_cast<float>(hoverTween_.duration);
		float k = EaseInOutQuad(t);

		sprite_->SetPosition(k * (hoverTween_.end - hoverTween_.start) + hoverTween_.start);

		// 1方向が終わったら反転
		if (hoverTween_.frame >= hoverTween_.duration) {

			std::swap(hoverTween_.start, hoverTween_.end);
			hoverTween_.frame = 0;
			isHoverReverse_ = !isHoverReverse_;
		}

		break;
	}

	default:
		break;
	}
}
