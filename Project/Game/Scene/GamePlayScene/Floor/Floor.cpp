#include "Floor.h"
#include "3D/ModelManager.h"

#include <imgui.h>

void Floor::Initialize() {

	// モデル読み込み
	ModelManager::GetInstance()->LoadModelData("Resources/Floor", "floor.obj");

	// モデルの生成・初期化
	model_ = std::make_unique<Model>();
	model_->Initialize("Resources/Floor", "floor.obj");

	// 3Dオブジェクトの生成・初期化
	for (int i = 0; i < 10; ++i) {
		auto object = std::make_unique<Object3d>();
		object->Initialize();
		object->SetModel(model_.get());
		object->SetScale({ 50.0f, 1.0f, 50.0f });
		object->SetTranslate({ 0.0f, 0.0f, static_cast<float>(i) * object->GetScale().z });
		objects_.push_back(std::move(object));
	}
}

void Floor::Update() {

	// 削除対象のインデックスを記録
	std::vector<size_t> removeIndices;

	// カメラより後ろか判定
	for (size_t i = 0; i < objects_.size(); ++i) {
		float objectZ = objects_[i]->GetTranslate().z + objects_[i]->GetScale().z;
		if (objectZ < cameraTranslate_.z) {
			removeIndices.push_back(i);
		}
	}

	// 最後尾のZ座標
	float lastZ = 0.0f;
	// 最後尾のZ座標を取得するためにループ
	for (const auto& obj : objects_) {

		// 最後尾のZ座標を取得
		lastZ = obj->GetTranslate().z + obj->GetScale().z;
	}

	// 削除と追加
	for (auto ite = removeIndices.rbegin(); ite != removeIndices.rend(); ++ite) {

		// 削除
		objects_.erase(objects_.begin() + *ite);

		// 新規作成
		auto newObject = std::make_unique<Object3d>();
		newObject->Initialize();
		newObject->SetModel(model_.get());
		newObject->SetScale({ 50.0f, 1.0f, 50.0f });
		newObject->SetTranslate({ 0.0f, 0.0f, lastZ });
		objects_.push_back(std::move(newObject));
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

#ifdef _DEBUG
	
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

#endif // _DEBUG
}