#include "Camera.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"
#include <imgui.h>

using namespace MathMatrix;

Camera::Camera() {

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	fovY = 0.45f;

	aspectRatio = static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight);

	nearClip = 0.1f;

	farClip = 100.0f;

	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	viewMatrix = Inverse(worldMatrix);

	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	projectionMatrixInverse = Inverse(projectionMatrix);

	viewProjectionMatrix = viewMatrix * projectionMatrix;

	// ワールド座標をワールド行列から得る
	worldPosition.x = worldMatrix.m[3][0];
	worldPosition.y = worldMatrix.m[3][1];
	worldPosition.z = worldMatrix.m[3][2];
}

void Camera::Update() {

	// transformからアフィン変換行列を計算
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// worldMatrixの逆行列を代入
	viewMatrix = Inverse(worldMatrix);

	// 透視投影行列を書き込む
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// projectionMatrixの逆行列を代入
	projectionMatrixInverse = Inverse(projectionMatrix);

	// viewMatrixとprojectionMatrixを合わせる
	viewProjectionMatrix = viewMatrix * projectionMatrix;

	// ワールド座標をワールド行列から得る
	worldPosition.x = worldMatrix.m[3][0];
	worldPosition.y = worldMatrix.m[3][1];
	worldPosition.z = worldMatrix.m[3][2];
}

void Camera::ShowImGui(const char* name) {

#ifdef _DEBUG
	ImGui::Begin(name);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}