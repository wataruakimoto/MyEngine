#include "RailCameraController.h"
#include "MathMatrix.h"

#include <imgui.h>

using namespace Engine;
using namespace MathMatrix;

void RailCameraController::Initialize() {
	
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetTranslate({ 0.0f, 10.0f, 0.0f });
}

void RailCameraController::Update() {

	// Z方向にカメラを移動
	worldTransform_.AddTranslate({ 0.0f, 0.0f, 0.2f });
	
	// ワールド変換の更新
	worldTransform_.Update();

	// ワールド行列をワールド変換から取得
	Matrix4x4 worldMatrix = worldTransform_.GetWorldMatrix();

	// ワールド行列を逆行列に変換
	worldMatrix = Inverse(worldMatrix);

	// カメラのビュー行列を設定
	camera_->SetViewMatrix(worldMatrix);

	// カメラの更新
	camera_->UpdateProjectionMatrix();
	camera_->UpdateViewProjectionMatrix();
}

void RailCameraController::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("RailCameraController");

	// ワールド変換のImGui表示
	worldTransform_.ShowImGui();

	// ImGuiのツリー表示
	camera_->ShowImGuiTree();

	ImGui::End();

#endif // USE_IMGUI
}
