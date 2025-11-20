#include "Goal.h"

#include <imgui.h>

void Goal::Initialize() {

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.SetTranslate({ 0.0f, 0.0f, 1700.0f });

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Resources/Goal", "Goal.obj");
	model_->SetColor({ 1.0f, 1.0f, 1.0f, 0.9f });

	// 3Dオブジェクトの生成・初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->SetScale({ 50.0f, 50.0f, 1.0f });
}

void Goal::Update() {

	// ワールド変換の更新
	worldTransform_.UpdateMatrix();

	object_->SetTranslate(worldTransform_.GetWorldPosition());
	object_->SetRotate(worldTransform_.GetRotate());

	// 3Dオブジェクトの更新
	object_->Update();
}

void Goal::Draw() {
	// 3Dオブジェクトの描画
	object_->Draw();
}

void Goal::Finalize() {
}

void Goal::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("Goal");
	worldTransform_.ShowImGui();
	object_->ShowImGui();
	model_->ShowImGui();
	ImGui::End();

#endif // USE_IMGUI
}