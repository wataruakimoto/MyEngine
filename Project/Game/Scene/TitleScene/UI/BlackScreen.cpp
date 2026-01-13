#include "BlackScreen.h"

BlackScreen::BlackScreen() {
}

void BlackScreen::Initialize() {

	// スプライトの生成
	sprite_ = std::make_unique<Sprite>();

	// スプライトの初期化
	sprite_->Initialize("BlackScreen.png");
}

void BlackScreen::Update() {

	// アルファ値が0未満にならないようにする
	if (alpha_ < 0.0f) {
		alpha_ = 0.0f;
	}

	// 透明度を設定
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

	// スプライトの更新
	sprite_->Update();
}

void BlackScreen::Draw() {

	// スプライトの描画
	sprite_->Draw();
}
