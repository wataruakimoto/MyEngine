#include "DebugScene.h"
#include "3d/Object3dCommon.h"
#include "Base/OffscreenRendering/FilterManager.h"

#include <imgui.h>

void DebugScene::Initialize() {

	// カメラの初期化
	camera = std::make_unique <Camera>();
	camera->Initialize();
	camera->SetRotate({ 0.25f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,5.0f,-20.0f });

	FilterManager::GetInstance()->SetCamera(camera.get());

	Object3dCommon::GetInstance()->SetDefaultCamera(camera.get());

	// レベルローダーの初期化
	levelLoader = std::make_unique<Loader>();
	// レベルデータの読み込み
	levelLoader->LoadLevel("level.json");
	// オブジェクトの配置
	levelLoader->PlaceObject();
	// 自機の配置
	levelLoader->PlacePlayer();
	// 敵の配置
	levelLoader->PlaceEnemy();
}

void DebugScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_1)) {

		// シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("PLAY");
	}

	// カメラの更新
	camera->Update();

	// レベルローダーの更新
	levelLoader->Update();
}

void DebugScene::Draw() {

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画

	// レベルローダーの描画
	levelLoader->Draw();
}

void DebugScene::Finalize() {
}

void DebugScene::ShowImGui() {

#ifdef _DEBUG

	ImGui::Begin("Camera");

	camera->ShowImGuiTree();

	ImGui::End();

	// レベルローダーのImGui表示
	levelLoader->ShowImGui();

#endif // _DEBUG
}
