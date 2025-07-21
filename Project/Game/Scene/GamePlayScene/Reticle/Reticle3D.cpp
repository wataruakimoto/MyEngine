#include "Reticle3D.h"
#include "Scene/GamePlayScene/Player/Player.h"
#include "Math/MathVector.h"
#include "Math/MathMatrix.h"
#include "winApp/WinApp.h"
#include "Camera/Camera.h"
#include "Reticle2D.h"

#include <imgui.h>

using namespace MathMatrix;

void Reticle3D::Initialize() {

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("resources/reticle", "reticle.obj");

	// オブジェクトの生成・初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
}

void Reticle3D::Update() {

	// 自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = { 0.0f, 0.0f, 1.0f };
	// 自機のワールド行列を基準にオフセットを変換
	offset = TransformNormal(offset, player_->GetWorldTransform().GetWorldMatrix());
	// ベクトルの長さを整える
	offset = Normalize(offset) * kDistancePlayerToReticle_;
	// プレイヤーの座標を取得
	Vector3 playerPos = player_->GetWorldTransform().GetTranslate();
	// 位置を計算
	Vector3 translate = playerPos + offset;

	// レティクルの位置を設定
	worldTransform_.SetTranslate(translate);

	ConvertScreenToWorld();

	// オブジェクトの座標を設定
	object_->SetTranslate(worldTransform_.GetTranslate());
	// オブジェクトの更新
	object_->Update();
}

void Reticle3D::Draw() {

	// オブジェクトの描画
	object_->Draw();
}

void Reticle3D::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Reticle3D");
	
	worldTransform_.ShowImGui();

	ImGui::End();

#endif // _DEBUG
}

void Reticle3D::ConvertScreenToWorld() {

	// ビューポート行列
	Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 1.0f);

	// カメラからビュープロジェクション行列を取得
	Matrix4x4 viewProjectionMatrix_ = camera_->GetViewProjectionMatrix();

	// ビュープロジェクション行列とビューポート行列を掛け合わせる
	Matrix4x4 viewProjectionViewPort = viewProjectionMatrix_ * viewPortMatrix;

	// 合成した行列の逆行列を取得
	Matrix4x4 inverseViewProjectionViewPort = Inverse(viewProjectionViewPort);

	// レティクルのスクリーン座標を取得
	Vector2 reticlePosition_ = reticle2D_->GetReticlePosition();

	// Y座標を反転
	float screenY = WinApp::kClientHeight - reticlePosition_.y;

	// スクリーン座標
	Vector3 posNear = { reticlePosition_.x, screenY, 0.0f };
	Vector3 posFar = { reticlePosition_.x, screenY, 1.0f };

	// スクリーン座標をワールド座標に変換(2Dから3Dに)
	posNear = Transform(posNear, inverseViewProjectionViewPort);
	posFar = Transform(posFar, inverseViewProjectionViewPort);

	// マウスレイの方向を計算
	Vector3 rmouseDirection = Normalize(posFar - posNear);

	// カメラからレティクルの距離
	const float kDistanceCameraToReticle = 100.0f;
	
	// 位置を計算
	Vector3 translate = posNear + rmouseDirection * kDistanceCameraToReticle;

	// レティクルの位置を計算
	worldTransform_.SetTranslate(translate);
}
