#include "FadeTransition.h"
#include "Sprite/SpriteRenderer.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

FadeTransition::FadeTransition(Vector4 color, float startAlpha, float endAlpha) {

	// 引数をメンバ変数に保存
	color_ = color;
	startAlpha_ = startAlpha;
	endAlpha_ = endAlpha;

	// スプライトレンダラーのインスタンス取得
	spriteRenderer_ = SpriteRenderer::GetInstance();

	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("White1x1.png");
	sprite_->SetSize({ 1280.0f, 720.0f }); // 画面全体を覆うサイズに設定

	// 初期アルファ
	color_.w = startAlpha_;
	sprite_->SetColor(color_);
}

void FadeTransition::Update(float progress) {

	progress = std::clamp(progress, 0.0f, 1.0f);

	const float a = Lerp(startAlpha_, endAlpha_, progress);
	Vector4 c = color_;
	c.w = a;

	sprite_->SetColor(c);
	sprite_->Update();
}

void FadeTransition::Draw() {

	// スプライトの描画設定
	spriteRenderer_->SettingDrawing();

	// スプライトの描画
	sprite_->Draw();
}

void FadeTransition::ShowImGui() {
}
