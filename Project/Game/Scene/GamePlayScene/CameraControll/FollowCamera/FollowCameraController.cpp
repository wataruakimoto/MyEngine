#include "FollowCameraController.h"
#include "Object/Object3dCommon.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include "MathRandom.h"

#include <imgui.h>

using namespace MathVector;
using namespace MathMatrix;
using namespace MathRandom;

void FollowCameraController::Initialize() {

	// ワールド変換の初期化
	worldTransform.Initialize();
}

void FollowCameraController::Update() {

	// 追従対象からカメラまでのオフセット
	Vector3 offset = baseOffset;

	// カメラシェイクの更新
	if (isShaking_) {
		shakeTimer_ += 1.0f / 60.0f; // 60FPS想定

		if (shakeTimer_ >= shakeDuration_) {
			// シェイク終了
			isShaking_ = false;
			shakeTimer_ = 0.0f;
			shakeOffset_ = { 0.0f, 0.0f, 0.0f };
		}
		else {

			// 時間経過で減衰
			float decay = 1.0f - (shakeTimer_ / shakeDuration_);
			float currentIntensity = shakeIntensity_ * decay;

			shakeOffset_ = RandomVector3(shakeRange_) * currentIntensity;
		}
	}

	if (target) {
	
		// 回転行列を作成
		Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform.GetRotate());
	
		// オフセットを回転に合わせる
		offset = TransformNormal(offset, rotateMatrix);
	
		// 座標をコピーしてオフセット分ずらす
		Vector3 targetPosition = target->GetTranslate() + offset + shakeOffset_;
	
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

#ifdef USE_IMGUI

	ImGui::Begin("FollowCameraController");

	ImGui::DragFloat3("BaseOffset", &baseOffset.x, 0.1f);

	// ワールド変換のImGui表示
	worldTransform.ShowImGui();

	// ImGuiのツリー表示
	camera->ShowImGuiTree();

	ImGui::End();

#endif // USE_IMGUI
}

void FollowCameraController::StartShake(float intensity, float duration) {
	isShaking_ = true;
	shakeIntensity_ = intensity;
	shakeDuration_ = duration;
	shakeTimer_ = 0.0f;

	// シェイク範囲を強度に応じて設定
	shakeRange_ = { {-intensity, -intensity, -intensity}, {intensity, intensity, intensity} };
}
