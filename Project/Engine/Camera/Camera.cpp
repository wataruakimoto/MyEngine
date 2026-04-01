#include "Camera.h"
#include "MathMatrix.h"
#include "WinApp.h"

#include <imgui.h>

using namespace Engine;
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

	// ワールド変換を初期化
	worldTransform_.Initialize();

	// ビュー行列をワールド行列の逆行列で初期化
	viewMatrix = Inverse(worldTransform_.GetWorldMatrix());

	// プロジェクション行列を透視投影行列で初期化
	projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRatio, nearClip, farClip);

	// プロジェクション行列の逆行列を初期化
	projectionMatrixInverse = Inverse(projectionMatrix);

	// ビュープロジェクション行列をビュー行列とプロジェクション行列の積で初期化
	viewProjectionMatrix = viewMatrix * projectionMatrix;
}

void Camera::UpdateViewMatrix() {

	// ワールド変換を更新
	worldTransform_.Update();

	// worldMatrixの逆行列を代入
	viewMatrix = Inverse(worldTransform_.GetWorldMatrix());
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

#ifdef USE_IMGUI

	ImGui::Begin(name);

	ImGui::DragFloat("FovY", &fovY, 0.01f);
	ImGui::DragFloat("NearClip", &nearClip, 0.01f);
	ImGui::DragFloat("FarClip", &farClip, 0.01f);

	worldTransform_.ShowImGui();

	// ビュー行列を表示
	if (ImGui::TreeNodeEx("ビュー行列")) {

		for (int i = 0; i < 4; ++i) {

			ImGui::Text("%f, %f, %f, %f", viewMatrix.m[i][0], viewMatrix.m[i][1], viewMatrix.m[i][2], viewMatrix.m[i][3]);
		}

		ImGui::TreePop();
	}

	ImGui::End();

#endif // USE_IMGUI
}

void Camera::ShowImGuiTree() {

#ifdef USE_IMGUI

	if (ImGui::TreeNode("カメラ")) {

		ImGui::DragFloat("FovY", &fovY, 0.01f);
		ImGui::DragFloat("NearClip", &nearClip, 0.01f);
		ImGui::DragFloat("FarClip", &farClip, 0.01f);

		worldTransform_.ShowImGui();

		// ビュー行列を表示
		if (ImGui::TreeNodeEx("ビュー行列")) {

			for (int i = 0; i < 4; ++i) {

				ImGui::Text("%f, %f, %f, %f", viewMatrix.m[i][0], viewMatrix.m[i][1], viewMatrix.m[i][2], viewMatrix.m[i][3]);
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

#endif // USE_IMGUI
}
