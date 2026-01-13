#include "SkyBoxGame.h"
#include "Player/Player.h"

#include <imgui.h>

void SkyBoxGame::Initialize() {

	// スカイボックスの生成
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("rostock_laage_airport_4k.dds");

	// 大きくしとく
	skybox->SetScale({ 500.0f, 500.0f, 500.0f });
}

void SkyBoxGame::Update() {

	// プレイヤーの位置を取得
	Vector3 playerPos = player_->GetWorldTransform().GetTranslate();

	// Z軸だけプレイヤーの位置に合わせる
	skybox->SetTranslate({ 0.0f, 0.0f, playerPos.z });

	// スカイボックスの更新
	skybox->Update();
}

void SkyBoxGame::Draw() {

	// スカイボックスの描画
	skybox->Draw();
}

void SkyBoxGame::Finalize() {
}

void SkyBoxGame::ShowImGui() {

#ifdef USE_IMGUI

	ImGui::Begin("SkyBoxGame");
	skybox->ShowImGui("SkyBoxGame");
	ImGui::End();

#endif
}