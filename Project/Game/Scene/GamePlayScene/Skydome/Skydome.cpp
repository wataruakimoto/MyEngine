#include "Skydome.h"
#include "3D/ModelManager.h"
#include <imgui.h>

void Skydome::Initialize() {

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModelData("Resources/Skydome", "skydome.obj");

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("Resources/Skydome", "skydome.obj");

	// 3Dオブジェクトの生成・初期化
	object = new Object3d();
	object->Initialize();
	object->SetModel(model);
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

	// 3Dオブジェクトの解放
	delete object;

	// モデルの解放
	delete model;
}