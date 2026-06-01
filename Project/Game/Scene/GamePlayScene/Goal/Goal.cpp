#include "Goal.h"
#include "Player/Player.h"
#include "Math/Easing.h"

#include <imgui.h>

using namespace Engine;
using namespace Easing;

void Goal::Initialize() {

	/// ===== ゴールの初期化 ===== ///

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Goal/Goal.obj");
	model_->SetColor({ 1.0f, 1.0f, 1.0f, 0.9f });

	// 3Dオブジェクトの生成・初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize();
	object_->SetModel(model_.get());
	object_->GetWorldTransform().SetScale({ 50.0f, 50.0f, 1.0f });
}

void Goal::Update() {

	/// ===== ゴールの更新 ===== ///

	// ワールド変換の更新
	worldTransform_.Update();

	object_->GetWorldTransform().SetTranslate(worldTransform_.GetWorldPosition());
	object_->GetWorldTransform().SetRotate(worldTransform_.GetRotate());

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

	ImGui::End();

#endif // USE_IMGUI
}
