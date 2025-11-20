#include "ResultUI.h"

#include <imgui.h>

ResultUI::ResultUI() {

	// テクスチャの読み込み
	textureManager_->LoadTexture("Resources/Result/Clear.png");
	textureManager_->LoadTexture("Resources/Result/GameOver.png");
}

void ResultUI::Initialize() {

	// スプライトの生成
	clearSprite_ = std::make_unique<Sprite>();
	gameOverSprite_ = std::make_unique<Sprite>();

	// スプライトの初期化
	clearSprite_->Initialize("Resources/Result/Clear.png");
	gameOverSprite_->Initialize("Resources/Result/GameOver.png");

	// スプライトの位置を画面中央に設定
	clearSprite_->SetPosition({ 640.0f,360.0f });
	gameOverSprite_->SetPosition({ 640.0f,360.0f });

	// スプライトのアンカーポイントを中央に設定
	clearSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	gameOverSprite_->SetAnchorPoint({ 0.5f, 0.5f });

	// アニメーション関連の初期化
	isAnimating_ = false;
	isAnimationFinished_ = false;
	animationFrame_ = 0;
}

void ResultUI::Update() {

	// アニメーション中なら
	if (isAnimating_) {

		// アニメーション更新
		AnimationUpdate();
	}

	// スプライトの更新
	clearSprite_->Update();
	gameOverSprite_->Update();
}

void ResultUI::Draw() {

	// 結果に応じてスプライトを描画
	if (resultType_ == ResultType::Clear) {
		clearSprite_->Draw();
	}
	else {
		gameOverSprite_->Draw();
	}
}

void ResultUI::ShowImGui() {

#ifdef USE_IMGUI

	// スプライトのImGui表示
	if (resultType_ == ResultType::Clear) {
		clearSprite_->ShowImGui("ClearSprite");
	}
	else {
		gameOverSprite_->ShowImGui("GameOverSprite");
	}

	ImGui::Begin("ResultUI");

	// アニメーション情報表示
	ImGui::Text("Animation Frame: %d", animationFrame_);
	ImGui::Text("Is Animating: %s", isAnimating_ ? "true" : "false");
	ImGui::Text("Is Finished: %s", isAnimationFinished_ ? "true" : "false");

	// アニメーション開始ボタン
	if (ImGui::Button("Start Clear Animation")) {
		StartAnimation(ResultType::Clear);
	}
	if (ImGui::Button("Start GameOver Animation")) {
		StartAnimation(ResultType::GameOver);
	}

	// アニメーション設定
	ImGui::SliderInt("Slide Down Duration", &slideDownDuration_, 1, 180);
	ImGui::SliderInt("Wait Duration", &waitDuration_, 1, 180);
	ImGui::SliderFloat2("Start Position", &startPosition_.x, -500.0f, 1500.0f);
	ImGui::SliderFloat2("Target Position", &targetPosition_.x, 0.0f, 1280.0f);

	ImGui::End();

#endif // USE_IMGUI
}

void ResultUI::StartAnimation(ResultType resultType) {

	// 結果フラグを設定
	resultType_ = resultType;

	// アニメーションフラグを立てる
	isAnimating_ = true;

	// アニメーション終了フラグを下ろす
	isAnimationFinished_ = false;

	// アニメーションフレームをリセット
	animationFrame_ = 0;

	// 状態を初期化
	animationState_ = ResultAnimationState::SlideDown;

	// スプライトの位置を開始位置に設定
	clearSprite_->SetPosition(startPosition_);
	gameOverSprite_->SetPosition(startPosition_);
}

void ResultUI::AnimationUpdate() {

	// フレームを進める
	animationFrame_++;

	Vector2 currentPosition = { 0.0f, 0.0f };

	// 現在のスプライトを取得
	Sprite* currentSprite = (resultType_ == ResultType::Clear) ? clearSprite_.get() : gameOverSprite_.get();

	switch (animationState_) {

	case ResultAnimationState::SlideDown:

		// スライドダウンフェーズ
		if (animationFrame_ <= slideDownDuration_) {

			float t = static_cast<float>(animationFrame_) / static_cast<float>(slideDownDuration_);
			float easedT = EaseOutQuart(t);

			currentPosition = {
				startPosition_.x + (targetPosition_.x - startPosition_.x) * easedT,
				startPosition_.y + (targetPosition_.y - startPosition_.y) * easedT
			};
		}
		else {

			// スライド完了、待機フェーズに移行
			animationState_ = ResultAnimationState::Wait;
			animationFrame_ = 0;
			currentPosition = targetPosition_;
		}

		// 位置を更新
		currentSprite->SetPosition(currentPosition);

		break;

	case ResultAnimationState::Wait:

		// 待機フェーズ
		if (animationFrame_ <= waitDuration_) {

			currentPosition = targetPosition_;
		}
		else {

			// 待機完了、アニメーション終了（位置はそのまま）
			isAnimating_ = false;
			isAnimationFinished_ = true;
			currentPosition = targetPosition_;
		}

		// 位置を更新
		currentSprite->SetPosition(currentPosition);

		break;
	}
}

float ResultUI::EaseOutQuart(float t) {
	
	return 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t) * (1.0f - t);
}
