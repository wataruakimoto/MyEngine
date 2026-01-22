#include "Floor.h"
#include "Model/ModelManager.h"

#include <imgui.h>

void Floor::Initialize() {

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Floor", "floor.obj");

	// 3Dオブジェクトの生成・初期化
	for (int i = 0; i < 20; ++i) {
		auto object = std::make_unique<Object3d>();
		object->Initialize();
		object->SetModel(model_.get());
		object->SetScale({ 50.0f, 1.0f, 50.0f });
		object->SetTranslate({ 0.0f, 0.0f, static_cast<float>(i) * object->GetScale().z });
		objects_.push_back(std::move(object));
	}
}

void Floor::Update() {

	// リストの最後のオブジェクトのZスケールを取得
	float objectScaleZ = objects_.back()->GetScale().z;

	// リストの最後のオブジェクトのZ座標を取得
	float lastObjectZ = objects_.back()->GetTranslate().z;

	// 足し合わせて床の末端Z座標を計算
	float endFloorZ = lastObjectZ + objectScaleZ;

	// カメラと末端Z座標の距離を計算
	float distance = endFloorZ - cameraTranslate_.z;

	// 距離が一定以下なら
	if (distance <= kFloorLength) {

		// 床を1枚作成
		std::unique_ptr<Object3d> object = std::make_unique<Object3d>();
		object->Initialize();							 // 初期化
		object->SetModel(model_.get());					 // モデルセット
		object->SetScale({ 50.0f, 1.0f, 50.0f });		 // スケールセット
		object->SetTranslate({ 0.0f, 0.0f, endFloorZ }); // 位置セット
		objects_.push_back(std::move(object));			 // リストに追加
	}

	// オブジェクトの更新
	for (auto& object : objects_) {

		object->Update();
	}
}

void Floor::Draw() {

	// オブジェクトの描画
	for (const auto& object : objects_) {

		object->Draw();
	}
}

void Floor::Finalize() {
}

void Floor::ShowImGui() {

#ifdef USE_IMGUI
	
	ImGui::Begin("Floor");

	for (size_t i = 0; i < objects_.size(); ++i) {

		// オブジェクトの位置を表示
		ImGui::Text("Position: (%.2f, %.2f, %.2f)", objects_[i]->GetTranslate().x, objects_[i]->GetTranslate().y, objects_[i]->GetTranslate().z);

		// "Object #" + 番号のラベルを生成
		std::string label = "Object " + std::to_string(i);

		// ツリーノードを作成
		if (ImGui::TreeNode(label.c_str())) {

			objects_[i]->ShowImGui();

			ImGui::TreePop();
		}
	}

	model_->ShowImGui();

	ImGui::Text("Camera Translate: (%.2f, %.2f, %.2f)", cameraTranslate_.x, cameraTranslate_.y, cameraTranslate_.z);

	ImGui::End();

#endif // USE_IMGUI
}