#include "Reticle2D.h"
#include "Texture/TextureManager.h"
#include "Input.h"
#include "Reticle3D.h"
#include "Camera.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include "WinApp.h"

#include <imgui.h>

using namespace MathVector;
using namespace MathMatrix;

Reticle2D::Reticle2D() {
}

void Reticle2D::Initialize() {

	// 位置の設定
	reticlePosition_ = { 640.0f, 360.0f };

	// スプライトの生成・初期化
	spriteReticle_ = std::make_unique<Sprite>();
	spriteReticle_->Initialize("2DReticle.png");
	spriteReticle_->SetAnchorPoint({ 0.5f, 0.5f });
	spriteReticle_->SetPosition(reticlePosition_);
}

void Reticle2D::Update() {

	// マウスの位置をレティクルの位置に設定
	reticlePosition_ = Input::GetInstance()->GetMousePosition();

	// スプライトの座標変更を反映
	spriteReticle_->SetPosition(reticlePosition_);

	// スプライトの更新
	spriteReticle_->Update();
}

void Reticle2D::Draw() {

	// スプライトの描画
	spriteReticle_->Draw();
}

void Reticle2D::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Reticle2D");
	ImGui::DragFloat2("Position", &reticlePosition_.x, 0.1f);
	ImGui::End();

#endif // USE_IMGUI
}
