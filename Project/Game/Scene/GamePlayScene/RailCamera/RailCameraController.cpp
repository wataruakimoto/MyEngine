#include "RailCameraController.h"
#include "Math/MathMatrix.h"

using namespace MathMatrix;

void RailCameraController::Initialize() {
	
	// ワールド変換の初期化
	worldTransform.Initialize();

	// カメラの初期化
	camera.Initialize();
}

void RailCameraController::Update() {
	
	// ワールド変換の更新
	worldTransform.UpdateMatrix();

	// ワールド行列をワールド変換から取得
	Matrix4x4 worldMatrix = worldTransform.GetWorldMatrix();

	// ワールド行列を逆行列に変換
	worldMatrix = Inverse(worldMatrix);

	// カメラのビュー行列を設定
	camera.SetViewMatrix(worldMatrix);
}