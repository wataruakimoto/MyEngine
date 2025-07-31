#include "DebugScene.h"
#include "3d/Object3dCommon.h"
#include "3D/Skybox/SkyboxCommon.h"
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
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	FilterManager::GetInstance()->SetCamera(camera.get());

	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	model = std::make_unique <Model>();
	model->Initialize("Resources/Sphere", "Sphere.gltf");

	object = std::make_unique <Object3d>();
	object->Initialize();
	object->SetModel(model.get());

	// Skyboxの初期化
	skybox = std::make_unique <Skybox>();
	skybox->Initialize("Resources", "rostock_laage_airport_4k.dds");
	skybox->SetScale({ 100.0f, 100.0f, 100.0f });
	// Skyboxのカメラをセット
	skybox->SetCamera(camera.get());

	// モデルに環境マップをセット
	model->SetEnvironmentMapFilePath("Resources/rostock_laage_airport_4k.dds");
	model->SetLightingMode(7);
}

void DebugScene::Update() {

	// カメラの更新
	camera->Update();

	// terrainの更新
	object->Update();

	// Skyboxの更新
	skybox->Update();
}

void DebugScene::Draw() {

	/// === Skyboxの描画準備 === ///
	SkyboxCommon::GetInstance()->SettingDrawing();

	// Skyboxの描画
	skybox->Draw();

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画

	object->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("terrain");
	
	object->ShowImGui();

	model->ShowImGui();
	
	ImGui::End();

	skybox->ShowImGui("Skybox");

	ImGui::Begin("Camera");

	camera->ShowImGuiTree();

	ImGui::End();

#endif // _DEBUG
}
