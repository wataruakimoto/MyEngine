#define NOMINMAX

#include "RuleUI.h"
#include "Easing.h"

#include <imgui.h>
#include <algorithm>

using namespace Engine;
using namespace Easing;

RuleUI::RuleUI() {
}

void RuleUI::Initialize() {

	// スプライトの生成
	ruleSprite_ = std::make_unique<Sprite>();
	operationSprite_ = std::make_unique<Sprite>();
	// スプライトの初期化
	ruleSprite_->Initialize("Rule/Rule.png");
	operationSprite_->Initialize("Rule/Operation.png");
	// スプライトの位置を画面中央に設定
	ruleSprite_->SetPosition({ 640.0f,300.0f });
	operationSprite_->SetPosition({ 640.0f,300.0f });
	// スプライトのアンカーポイントを中央に設定
	ruleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	operationSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	ruleSprite_->SetSize({ 0.0f, 0.0f });
	operationSprite_->SetSize({ 0.0f, 0.0f });

	// アニメーション関連の初期化
	isAnimating_ = false;
	isAnimationFinished_ = false;
	animationFrame_ = 0;
}

void RuleUI::Update() {

	// アニメーション中なら
	if (isAnimating_) {

		// アニメーション更新
		AnimationUpdate();
	}

	// スプライトの更新
	ruleSprite_->Update();
	operationSprite_->Update();
}

void RuleUI::Draw() {

	// スプライトの描画
	switch (animationState_) {

	case AnimationStateWarning::BounceIn:
	case AnimationStateWarning::Wait:
	case AnimationStateWarning::ScaleDown:

		// 1回目のバウンス - ruleSprite_を描画
		ruleSprite_->Draw();

		break;

	case AnimationStateWarning::SecondBounceIn:
	case AnimationStateWarning::SecondWait:
	case AnimationStateWarning::SecondScaleDown:

		// 2回目のバウンス - operationSprite_を描画
		operationSprite_->Draw();

		break;
	}
}

void RuleUI::ShowImGui() {

#ifdef USE_IMGUI

	// スプライトのImGui表示
	ruleSprite_->ShowImGui("RuleUI");

	ImGui::Begin("RuleUI");

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

#endif // USE_IMGUI
}

void RuleUI::StartBounceAnimation() {

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
	ruleSprite_->SetSize(initialSize);
}

void RuleUI::AnimationUpdate() {

	// フレームを進める
	animationFrame_++;

	Vector2 currentScale = { 0.0f, 0.0f };

	Vector2 newSize = { 0.0f, 0.0f };

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

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		ruleSprite_->SetSize(newSize);

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

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		ruleSprite_->SetSize(newSize);

		break;

	case AnimationStateWarning::ScaleDown:

		// 縮小フェーズ
		if (animationFrame_ <= scaleDownDuration_) {

			float t = static_cast<float>(animationFrame_) / static_cast<float>(scaleDownDuration_);
			float easedT = EaseInOutQuad(t);

			currentScale = {
				maxScale_.x + (endScale_.x - maxScale_.x) * easedT,
				maxScale_.y + (endScale_.y - maxScale_.y) * easedT
			};
		}
		else {

			// 縮小完了、2回目のアニメーションに移行
			animationState_ = AnimationStateWarning::SecondBounceIn;
			animationFrame_ = 0;
			currentScale = endScale_;
		}

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		ruleSprite_->SetSize(newSize);

		break;

	case AnimationStateWarning::SecondBounceIn:

		// 2回目の拡大フェーズ
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
			animationState_ = AnimationStateWarning::SecondWait;
			animationFrame_ = 0;
			currentScale = maxScale_;
		}

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		operationSprite_->SetSize(newSize);

		break;

	case AnimationStateWarning::SecondWait:

		// 2回目の待機フェーズ
		if (animationFrame_ <= waitDuration_) {

			currentScale = maxScale_;
		}
		else {

			// 待機完了、縮小フェーズに移行
			animationState_ = AnimationStateWarning::SecondScaleDown;
			animationFrame_ = 0;
			currentScale = maxScale_;
		}

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		operationSprite_->SetSize(newSize);

		break;

	case AnimationStateWarning::SecondScaleDown:

		// 2回目の縮小フェーズ
		if (animationFrame_ <= scaleDownDuration_) {

			float t = static_cast<float>(animationFrame_) / static_cast<float>(scaleDownDuration_);
			float easedT = EaseInOutQuad(t);

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

		// サイズを更新
		newSize = { kDefaultSize.x * currentScale.x, kDefaultSize.y * currentScale.y };
		operationSprite_->SetSize(newSize);

		break;
	}
}
