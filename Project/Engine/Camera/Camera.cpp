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

void Camera::UpdateViewMatrix() {

	// transformからアフィン変換行列を計算
	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

	// worldMatrixの逆行列を代入
	viewMatrix = Inverse(worldMatrix);
}

void Camera::UpdateProjectionMatrix() {

	// 透視投影行列を書き込む
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// projectionMatrixの逆行列を代入
	projectionMatrixInverse = Inverse(projectionMatrix);
}

void Camera::UpdateViewProjectionMatrix() {

	// viewMatrixとprojectionMatrixを合わせる
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::Update() {

	// ビュー行列を更新
	UpdateViewMatrix();

	// プロジェクション行列を更新
	UpdateProjectionMatrix();

	// ビュープロジェクション行列を更新
	UpdateViewProjectionMatrix();
}

void Camera::ShowImGui(const char* name) {

#ifdef _DEBUG

	ImGui::Begin(name);

	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &translate.x, 0.01f);

	ImGui::DragFloat("FovY", &fovY, 0.01f);
	ImGui::DragFloat("NearClip", &nearClip, 0.01f);
	ImGui::DragFloat("FarClip", &farClip, 0.01f);

	// ワールド行列を表示
	if (ImGui::TreeNode("WorldMatrix")) {

		for (int i = 0; i < 4; ++i) {

			ImGui::Text("%f, %f, %f, %f", worldMatrix.m[i][0], worldMatrix.m[i][1], worldMatrix.m[i][2], worldMatrix.m[i][3]);
		}

		ImGui::TreePop();
	}

	ImGui::End();


#endif // _DEBUG
}

void Camera::ShowImGuiTree() {

	if (ImGui::TreeNode("Camera")) {

		ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &translate.x, 0.01f);

		ImGui::DragFloat("FovY", &fovY, 0.01f);
		ImGui::DragFloat("NearClip", &nearClip, 0.01f);
		ImGui::DragFloat("FarClip", &farClip, 0.01f);

		// ワールド行列を表示
		if (ImGui::TreeNode("WorldMatrix")) {

			for (int i = 0; i < 4; ++i) {

				ImGui::Text("%f, %f, %f, %f", worldMatrix.m[i][0], worldMatrix.m[i][1], worldMatrix.m[i][2], worldMatrix.m[i][3]);
			}

			ImGui::TreePop();
		}

		// ビュー行列を表示
		if (ImGui::TreeNode("ViewMatrix")) {

			for (int i = 0; i < 4; ++i) {

				ImGui::Text("%f, %f, %f, %f", viewMatrix.m[i][0], viewMatrix.m[i][1], viewMatrix.m[i][2], viewMatrix.m[i][3]);
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
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