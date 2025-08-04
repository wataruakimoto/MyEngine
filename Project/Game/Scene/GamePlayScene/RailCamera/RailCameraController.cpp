#include "RailCameraController.h"
#include "Math/MathMatrix.h"
#include "3d/Object3dCommon.h"

#include <imgui.h>

using namespace MathMatrix;

void RailCameraController::Initialize() {
	
	// ワールド変換の初期化
	worldTransform.Initialize();
	worldTransform.SetTranslate({ 0.0f, 10.0f, 0.0f });

	// カメラの初期化
	camera = std::make_unique<Camera>();
	camera->Initialize();

	// カメラの設定
	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());
}

void RailCameraController::Update() {

	// Z方向にカメラを移動
	//worldTransform.AddTranslate({ 0.0f, 0.0f, 0.2f });
	
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
