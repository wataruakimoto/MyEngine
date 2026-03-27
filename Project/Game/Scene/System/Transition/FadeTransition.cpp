#include "FadeTransition.h"
#include "Sprite/SpriteRenderer.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

FadeTransition::FadeTransition(Vector3 color, float startAlpha, float endAlpha) {

	// 引数をメンバ変数に保存
	color_ = { color.x, color.y, color.z, startAlpha };
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

	float alpha = Lerp(startAlpha_, endAlpha_, progress);
	
	color_.w = alpha;

	sprite_->SetColor(color_);

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
