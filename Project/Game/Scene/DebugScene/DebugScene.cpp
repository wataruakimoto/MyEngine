#include "DebugScene.h"
#include "3d/Object3dCommon.h"
#include "Base/OffscreenRendering/FilterManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include <numbers>
#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetRotate({ 0.25f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,7.5f,-25.0f });

	FilterManager::GetInstance()->SetCamera(camera.get());

	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	modelTerrain = std::make_unique <Model>();
	modelTerrain->Initialize("Resources/terrain", "terrain.gltf");

	terrain = std::make_unique <Object3d>();
	terrain->Initialize();
	terrain->SetModel(modelTerrain.get());
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// terrainの更新
	terrain->Update();
}

void DebugScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画

	terrain->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef _DEBUG

	//ImGui::Begin("terrain");
	//
	//terrain->ShowImGui();
	//
	//ImGui::End();

	ImGui::Begin("Camera");

	camera->ShowImGuiTree();

	ImGui::End();

#endif // _DEBUG
}
