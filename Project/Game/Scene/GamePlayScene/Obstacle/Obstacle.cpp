#include "Obstacle.h"
#include "Collision/CollisionTypeIDDef.h"

#include <imgui.h>

using namespace Engine;

void Obstacle::Initialize() {

	// ワールド変換初期化
	worldTransform_.Initialize();

	// コライダー生成
	collider_ = std::make_unique<Collider>(
		AABB{},
		static_cast<uint32_t>(CollisionTypeIDDef::kObstacle)
	);

	// コライダー初期化
	collider_->Initialize();

	// コライダーに衝突時のコールバック関数を設定
	collider_->SetOnCollision([this](Collider* other) {OnCollision(other); });

	// コライダーにワールド変換を設定
	collider_->SetWorldTransform(worldTransform_);

	// モデルの生成
	model_ = std::make_unique<Model>();
	// モデルの初期化
	model_->Initialize("Obstacle", "Obstacle.obj");

	// 3Dオブジェクトの生成
	object_ = std::make_unique<Object3d>();
	// 3Dオブジェクトの初期化
	object_->Initialize();
	// モデルを設定
	object_->SetModel(model_.get());
}

void Obstacle::Update() {

	worldTransform_.Update();

	collider_->SetWorldTransform(worldTransform_);

	collider_->Update();

	object_->SetRotate(worldTransform_.GetRotate());

	object_->SetTranslate(worldTransform_.GetWorldPosition());

	object_->Update();
}

void Obstacle::Draw() {

	object_->Draw();
}

void Obstacle::OnCollision(Engine::Collider* other) {
}

void Obstacle::ShowImGui() {

	ImGui::Begin("Obstacle");

	worldTransform_.ShowImGui();

	object_->ShowImGui();

	model_->ShowImGui();

	ImGui::End();
}
