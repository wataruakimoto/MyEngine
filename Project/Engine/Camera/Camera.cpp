#include "Camera.h"
#include "math/MathMatrix.h"
#include "winApp/WinApp.h"
#include <imgui.h>

using namespace MathMatrix;

void Camera::Initialize() {

	// 水平方向視野を設定
	fovY = 0.45f;

	// アスペクト比を設定
	aspectRatio = static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight);

	// ニアクリップ距離を設定
	nearClip = 0.1f;

	// ファークリップ距離を設定
	farClip = 100.0f;

	// ワールド行列をアフィン変換行列で初期化
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	// ビュー行列をワールド行列の逆行列で初期化
	viewMatrix = Inverse(worldMatrix);

	// プロジェクション行列を透視投影行列で初期化
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// プロジェクション行列の逆行列を初期化
	projectionMatrixInverse = Inverse(projectionMatrix);

	// ビュープロジェクション行列をビュー行列とプロジェクション行列の積で初期化
	viewProjectionMatrix = viewMatrix * projectionMatrix;


}

void Camera::Update() {

	// transformからアフィン変換行列を計算
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	// worldMatrixの逆行列を代入
	viewMatrix = Inverse(worldMatrix);

	// 透視投影行列を書き込む
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// projectionMatrixの逆行列を代入
	projectionMatrixInverse = Inverse(projectionMatrix);

	// viewMatrixとprojectionMatrixを合わせる
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::ShowImGui(const char* name) {

#ifdef _DEBUG

	ImGui::Begin(name);

	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &translate.x, 0.01f);

	ImGui::DragFloat("FovY", &fovY, 0.01f);
	ImGui::DragFloat("NearClip", &nearClip, 0.01f);
	ImGui::DragFloat("FarClip", &farClip, 0.01f);

	ImGui::End();


#endif // _DEBUG
}

const Vector3& Camera::GetWorldPosition() const {

	// ワールド座標
	static Vector3 worldPosition;

	// ワールド行列から座標を取得
	worldPosition.x = worldMatrix.m[3][0];
	worldPosition.y = worldMatrix.m[3][1];
	worldPosition.z = worldMatrix.m[3][2];

	return worldPosition;
}