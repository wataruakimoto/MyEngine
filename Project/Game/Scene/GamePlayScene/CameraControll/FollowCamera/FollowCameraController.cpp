#include "FollowCameraController.h"
#include "3D/Object/Object3dCommon.h"
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

	// 回転を取得
	worldTransform.SetRotate(worldTransform.GetRotate());

	// 位置を取得
	camera->SetTranslate(worldTransform.GetTranslate());

	// カメラの更新
	camera->Update();
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