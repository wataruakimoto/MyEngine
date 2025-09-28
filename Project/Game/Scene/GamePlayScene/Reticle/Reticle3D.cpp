#include "Reticle3D.h"
#include "Scene/GamePlayScene/Player/Player.h"
#include "Math/MathVector.h"
#include "Math/MathMatrix.h"
#include "winApp/WinApp.h"
#include "Camera/Camera.h"
#include "Reticle2D.h"

#include <imgui.h>

using namespace MathVector;
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

	// 2Dレティクルのスクリーン座標をワールド座標に変換
	ConvertScreenToWorld();

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

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

	// NDC座標に変換
	Vector2 ndc = {
		(reticle2D_->GetReticlePosition().x / float(WinApp::kClientWidth)) * 2.0f - 1.0f,
		1.0f - (reticle2D_->GetReticlePosition().y / float(WinApp::kClientHeight)) * 2.0f // y軸は上下反転
	};

	// 近平面と遠平面のクリップ座標
	Vector3 nearClip = { ndc.x, ndc.y, 0.0f }; // 近平面
	Vector3 farClip = { ndc.x, ndc.y, 1.0f };  // 遠平面

	//ビュープロジェクション逆行列を取得
	Matrix4x4 inverseViewProjection = Inverse(camera_->GetViewProjectionMatrix());

	// クリップ座標をワールド座標に変換
	Vector3 nearWorld = MathVector::Transform(nearClip, inverseViewProjection);
	Vector3 farWorld = MathVector::Transform(farClip, inverseViewProjection);

	// 座標の計算
	Vector3 translate = nearWorld + Normalize(farWorld - nearWorld) * kDistancePlayerToReticle_;

	// レティクルの位置を計算
	worldTransform_.SetTranslate(translate);
}
