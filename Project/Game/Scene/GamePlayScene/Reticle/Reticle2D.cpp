#include "Reticle2D.h"
#include "2D/TextureManager.h"
#include "input/Input.h"
#include "Reticle3D.h"
#include "Camera/Camera.h"
#include "math/MathVector.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"

#include <imgui.h>

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

	ConvertWorldToScreen();

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

	spriteReticle_->ShowImGui("Reticle");
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

void Reticle2D::ConvertWorldToScreen() {

	// 3Dレティクルの位置を取得
	Vector3 position = reticle3D_->GetWorldTransform().GetTranslate();

	// ビューポート行列
	Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);

	// カメラからビュープロジェクション行列を取得
	Matrix4x4 viewProjectionMatrix_ = camera_->GetViewProjectionMatrix();

	// ビュープロジェクション行列とビューポート行列を掛け合わせる
	Matrix4x4 viewProjectionViewPort = viewProjectionMatrix_ * viewPortMatrix;

	// ワールド座標をスクリーン座標に変換(3Dから2Dに)
	position = Transform(position, viewProjectionViewPort);

	// Y座標を反転
	float screenY = WinApp::kClientHeight - position.y;

	// 座標を設定
	reticlePosition_ = { position.x, screenY };
}
