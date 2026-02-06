#include "FollowCameraController.h"
#include "Object/Object3dCommon.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include "MathRandom.h"
#include "Easing.h"
#include "Player/Player.h"

#include <imgui.h>

using namespace Engine;
using namespace MathVector;
using namespace MathMatrix;
using namespace MathRandom;
using namespace Easing;

void FollowCameraController::Initialize() {

	// ワールド変換の初期化
	worldTransform.Initialize();
}

void FollowCameraController::Update() {

	// カメラシェイクの更新
	Shake();

	// 先読みの更新
	LookAhead();

	// 追従の更新
	Follow();

	// ダッチロールの更新
	DutchRoll();

	// ダイナミックFovの更新
	DynamicFov();

	// ワールド変換の更新
	worldTransform.UpdateMatrix();

	// 位置を取得
	camera->SetTranslate(worldTransform.GetTranslate());

	// カメラの更新
	camera->Update();
}

void FollowCameraController::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("FollowCameraController");

	ImGui::DragFloat3("BaseOffset", &baseOffset_.x, 0.1f);

	// ワールド変換のImGui表示
	worldTransform.ShowImGui();

	// ImGuiのツリー表示
	camera->ShowImGuiTree();

	ImGui::End();

#endif // USE_IMGUI
}

void FollowCameraController::StartShake(float intensity, float duration) {

	// シェイクの初期化
	isShaking_ = true;
	shakeIntensity_ = intensity;
	shakeDuration_ = duration;
	shakeTimer_ = 0.0f;

	// シェイク範囲を強度に応じて設定
	shakeRange_ = { {-intensity, -intensity, -intensity}, {intensity, intensity, intensity} };
}

void FollowCameraController::Shake() {

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
}

void FollowCameraController::Follow() {

	if (player) {

		// 回転行列を作成
		Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform.GetRotate());

		// オフセットを回転に合わせる
		Vector3 offset = TransformNormal(baseOffset_, rotateMatrix);

		// 座標をコピーしてオフセット分ずらす
		Vector3 targetPosition = player->GetWorldTransform().GetWorldPosition() + offset + shakeOffset_ + lookAheadOffset_;

		// 現在地を取得
		Vector3 currentPosition = worldTransform.GetTranslate();

		// 新しい位置を計算
		Vector3 newPosition;

		newPosition.x = Lerp(currentPosition.x, targetPosition.x, followLerp_);
		newPosition.y = Lerp(currentPosition.y, targetPosition.y, followLerp_);
		newPosition.z = targetPosition.z; // Z座標は直接設定

		// ワールド変換の平行移動を設定
		worldTransform.SetTranslate(newPosition);
	}
}

void FollowCameraController::DutchRoll() {

	if (player) {

		// プレイヤーの横方向の速度を取得
		float playerVelocityX = player->GetVelocity().x;

		// 目標の傾き角度を計算
		float targetTilt = playerVelocityX * kTiltFactor_;

		// 傾きの最大値を度からラジアンに変換
		float tiltLimitRad = ConvertDegreesToRadians(kTiltLimit_);

		// 傾きを制限
		targetTilt = std::clamp(targetTilt, -tiltLimitRad, tiltLimitRad);

		// 現在の傾き角度を補間で滑らかに変更
		currentTilt_ = Lerp(currentTilt_, targetTilt, kTiltDelay_);

		// 現在の回転を取得
		Vector3 currentRotation = worldTransform.GetRotate();

		// Z軸に傾きを適用
		currentRotation.z = currentTilt_;

		// ワールド変換の回転を設定
		worldTransform.SetRotate(currentRotation);
	}
}

void FollowCameraController::DynamicFov() {

	float playerSpeedRate = player->GetSpeedRate();

	// 度をラジアンに変換
	float fovNormalRad = ConvertDegreesToRadians(kFovNormal_);
	float fovDashRad = ConvertDegreesToRadians(kFovDash_);

	// 速度に応じてFovを変化させる
	float newFovY = Lerp(fovNormalRad, fovDashRad, playerSpeedRate);

	// 現在のFovを取得
	float currentFovY = camera->GetFovY();

	// Fovを補間で滑らかに変更
	float updatedFovY = Lerp(currentFovY, newFovY, 0.1f);

	// カメラに設定
	camera->SetFovY(updatedFovY);
}

void FollowCameraController::LookAhead() {

	if (player) {

		// プレイヤーの速度を取得
		Vector3 playerVelocity = player->GetVelocity();

		// 目標のズレ幅を計算
		Vector3 targetOffset = playerVelocity * kLookAheadFactor_;

		// 滑らかに補間
		lookAheadOffset_ = Lerp(lookAheadOffset_, targetOffset, kLookAheadDelay_);

		// Z軸は0に固定
		lookAheadOffset_.z = 0.0f;
	}
}
