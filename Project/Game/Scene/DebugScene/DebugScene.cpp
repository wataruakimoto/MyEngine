#include "DebugScene.h"

#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });

	// オブジェクト3D共通部のカメラ設定
	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	// ゴールの生成&初期化
	goal = std::make_unique<Goal>();
	goal->Initialize();
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// ゴールの更新
	goal->Update();
}

void DebugScene::Draw() {

	/// === 3Dオブジェクト描画準備 === ///
	object3dCommon->SettingDrawing();

	// ゴールの描画
	goal->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

	// カメラのImGui表示
	camera->ShowImGui("Camera");

	// ゴールのImGui表示
	goal->ShowImGui();
}
