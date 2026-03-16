#include "SlideTransition.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

SlideTransition::SlideTransition(Vector2 endPosition) {

	endPosition_ = endPosition;

	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("White1x1.png");
	sprite_->SetSize({ 1280.0f, 720.0f }); // 画面全体を覆うサイズに設定
	sprite_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f }); // 黒色で初期化
	sprite_->SetAnchorPoint({ 0.5f, 0.5f }); // 中心をアンカーに設定
}

void SlideTransition::Update(float progress) {

	// 進行度に応じてスプライトの位置を更新
	Vector2 currentPosition = Lerp(centerPosition_, endPosition_, progress);

	// スプライトの位置を更新
	sprite_->SetPosition(currentPosition);

	// スプライトの更新
	sprite_->Update();
}

void SlideTransition::Draw() {

	// スプライトの描画
	sprite_->Draw();
}
