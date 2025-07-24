#include "RailCameraController.h"
#include "Math/MathMatrix.h"

#include <imgui.h>

using namespace MathMatrix;

void RailCameraController::Initialize() {
	
	// ワールド変換の初期化
	worldTransform.Initialize();
	worldTransform.SetTranslate({ 0.0f, 0.0f, -50.0f });

	// カメラの初期化
	camera = new Camera();
	camera->Initialize();
}

void RailCameraController::Update() {
	
	// ワールド変換の更新
	worldTransform.UpdateMatrix();

	// ワールド行列をワールド変換から取得
	Matrix4x4 worldMatrix = worldTransform.GetWorldMatrix();

	// ワールド行列を逆行列に変換
	worldMatrix = Inverse(worldMatrix);

	// カメラのビュー行列を設定
	camera->SetViewMatrix(worldMatrix);

	// カメラの更新
	camera->UpdateProjectionMatrix();
	camera->UpdateViewProjectionMatrix();
}

void RailCameraController::ShowImGui() {

	ImGui::Begin("RailCameraController");

	// ワールド変換のImGui表示
	worldTransform.ShowImGui();

	// ImGuiのツリー表示
	camera->ShowImGuiTree();

	ImGui::End();
}
