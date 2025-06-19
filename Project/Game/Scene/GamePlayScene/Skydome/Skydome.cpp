#include "Skydome.h"
#include <imgui.h>

void Skydome::Initialize() {

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("resources/sphere", "sphere.obj");

	// 3Dオブジェクトの生成・初期化
	object = new Object3d();
	object->Initialize();
	object->SetModel(model);
	object->SetScale({ 500.0f, 500.0f, 500.0f });
}

void Skydome::Update() {

#ifdef DEBUG_

	ImGui::Begin("Skydome");

	// 大きさ変更の確認
	Vector3 scale = object->GetScale();
	ImGui::DragFloat3("Scale", &scale.x, 0.1f);
	object->SetScale(scale);

	// 回転変更の確認
	Vector3 rotate = object->GetRotate();
	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	object->SetRotate(rotate);

	// 座標変更の確認
	Vector3 translate = object->GetTranslate();
	ImGui::DragFloat3("Translate", &translate.x, 0.1f);
	object->SetTranslate(translate);

	ImGui::End();

#endif // DEBUG_

	// 3Dオブジェクトの更新
	object->Update();
}

void Skydome::Draw() {

	// 3Dオブジェクトの描画
	object->Draw();
}

void Skydome::Finalize() {

	// 3Dオブジェクトの解放
	delete object;

	// モデルの解放
	delete model;
}