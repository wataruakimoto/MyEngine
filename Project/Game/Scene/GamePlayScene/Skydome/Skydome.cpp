#include "Skydome.h"
#include "3D/ModelManager.h"
#include <imgui.h>

void Skydome::Initialize() {

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModelData("Resources/Sphere", "sphere.obj");

	// モデルの生成・初期化
	model = std::make_unique<Model>();
	model->Initialize("Resources/Sphere", "sphere.obj");

	// 3Dオブジェクトの生成・初期化
	object = std::make_unique<Object3d>();
	object->Initialize();
	object->SetModel(model.get());
	object->SetScale({ 500.0f, 500.0f, 500.0f });
}

void Skydome::Update() {

#ifdef _DEBUG

	object->ShowImGui("Skydome");

#endif // _DEBUG

	// 3Dオブジェクトの更新
	object->Update();
}

void Skydome::Draw() {

	// 3Dオブジェクトの描画
	object->Draw();
}

void Skydome::Finalize() {
}