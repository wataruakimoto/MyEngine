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

	// 2Dレティクルの座標を設定
	Vector2 reticle2DPos = reticle2D_->GetReticlePosition();

	// カメラからビュープロジェクション行列を受け取り、逆行列に変換
	Matrix4x4 inverseViewProjectionMatrix = Inverse(camera_->GetViewProjectionMatrix());

	// 2Dレティクルのスクリーン座標をワールド座標に変換
	Vector3 reticlePos = ConvertScreenToWorld(reticle2DPos, inverseViewProjectionMatrix, 1.0f, 1.0f);

	// ワールド座標をレティクルの座標に設定
	worldTransform_.SetTranslate(reticlePos);

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
