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

	// 画像の読み込み
	TextureManager::GetInstance()->LoadTexture("resources/2DReticle.png");
}

void Reticle2D::Initialize() {

	// 位置の設定
	reticlePosition_ = { 640.0f, 360.0f };

	// スプライトの生成・初期化
	spriteReticle_ = std::make_unique<Sprite>();
	spriteReticle_->Initialize("resources/2DReticle.png");
	spriteReticle_->SetAnchorPoint({ 0.5f, 0.5f });
	spriteReticle_->SetPosition(reticlePosition_);
}

void Reticle2D::Update() {

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState = {};

	// ゲームパッドが接続されているか確認
	if (XInputGetState(0, &joyState) == ERROR_SUCCESS) {

		// ジョイスティック状態取得
		if (Input::GetInstance()->IsRightThumbStickOutDeadZone()) {
			reticlePosition_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 2.0f;
			reticlePosition_.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 2.0f;
		}
	}
	// ゲームパッドが接続されていない場合
	else {

		// マウスの位置をレティクルの位置に設定
		reticlePosition_ = Input::GetInstance()->GetMousePosition();
	}

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

#ifdef _DEBUG
	ImGui::Begin("Reticle2D");
	ImGui::DragFloat2("Position", &reticlePosition_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}
