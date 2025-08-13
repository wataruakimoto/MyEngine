#include "FollowCameraController.h"
#include "Math/MathMatrix.h"

#include <imgui.h>

using namespace MathMatrix;

void FollowCameraController::Initialize() {

	// ワールド変換の初期化
	worldTransform.Initialize();
	worldTransform.SetTranslate({ 0.0f, 10.0f, -10.0f });

	// カメラの初期化
	camera = std::make_unique<Camera>();
	camera->Initialize();
}

void FollowCameraController::Update() {

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