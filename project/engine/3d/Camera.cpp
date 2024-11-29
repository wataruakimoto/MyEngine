#include "Camera.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"

using namespace MathMatrix;

Camera::Camera() {

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };

	fovY = 0.45f;

	aspectRatio = static_cast<float>(WinApp::kClientWidth / WinApp::kClientHeight);

	nearClip = 0.1f;

	farClip = 100.0f;

	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	viewMatrix = Inverse(worldMatrix);

	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::Update() {

	// transformからアフィン変換行列を計算
	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// worldMatrixの逆行列を代入
	viewMatrix = Inverse(worldMatrix);

	// 透視投影行列を書き込む
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// viewMatrixとprojectionMatrixを合わせる
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}