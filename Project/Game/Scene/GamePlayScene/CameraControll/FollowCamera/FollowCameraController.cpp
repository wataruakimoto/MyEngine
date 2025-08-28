#include "FollowCameraController.h"
#include "3D/Object3dCommon.h"
#include "Math/MathVector.h"
#include "Math/MathMatrix.h"

#include <imgui.h>

using namespace MathVector;
using namespace MathMatrix;

void FollowCameraController::Initialize() {

	// ワールド変換の初期化
	worldTransform.Initialize();
}

void FollowCameraController::Update() {

	// 追従対象からカメラまでのオフセット
	Vector3 offset = baseOffset;

	if (target) {
	
		// 回転行列を作成
		Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform.GetRotate());
	
		// オフセットを回転に合わせる
		offset = TransformNormal(offset, rotateMatrix);
	
		// 座標をコピーしてオフセット分ずらす
		Vector3 targetPosition = target->GetTranslate() + offset;
	
		// ワールド変換の平行移動を設定
		worldTransform.SetTranslate(targetPosition);
	}

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

void FollowCameraController::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("FollowCameraController");

	ImGui::DragFloat3("BaseOffset", &baseOffset.x, 0.1f);

	// ワールド変換のImGui表示
	worldTransform.ShowImGui();

	// ImGuiのツリー表示
	camera->ShowImGuiTree();

	ImGui::End();

#endif // _DEBUG
}