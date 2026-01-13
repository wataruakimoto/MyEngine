#include "StartUI.h"

#include <numbers>

StartUI::StartUI() {
}

void StartUI::Initialize() {

	// スプライトの生成
	sprite_ = std::make_unique<Sprite>();
	// スプライトの初期化
	sprite_->Initialize("start.png");
	sprite_->SetPosition({ 640.0f,600.0f }); // 画面中央に表示
	sprite_->SetAnchorPoint({ 0.5f,0.5f }); // 中心を基準に表示

	// 最初は非表示
	isVisible_ = false;
}

void StartUI::Update() {

	// タイマーを進める
	blinkTimer_ += 1.0f / 60.0f; // 60FPS想定

	// θを計算
	const float theta = (blinkTimer_ / blinkCycle_) * 2.0f * std::numbers::pi_v<float>;

	// サイン波でアルファ値を変化させる
	// 振幅を-0.5~0.5にして範囲が1になり、0.5ずらして0~1に変換 (sin(θ)+1)/2 これと同じ
	// Max-Minで範囲を出してそれを掛けてからMinを足す これで最低がMinで最高がMaxになる
	alpha_ = (sinf(theta) * 0.5f + 0.5f) * (alphaMax_ - alphaMin_) + alphaMin_;

	// 表示フラグが立っていなければ透明にする
	if (!isVisible_) {
		alpha_ = 0.0f;
	}

	// 透明度を設定
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
	// スプライトの更新
	sprite_->Update();
}

void StartUI::Draw() {

	// スプライトの描画
	sprite_->Draw();
}

void StartUI::ShowImGui() {

	// スプライトのImGui表示
	sprite_->ShowImGui("StartUI");
}
