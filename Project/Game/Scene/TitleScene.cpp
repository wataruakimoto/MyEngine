#include "TitleScene.h"
#include "SceneManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "2d/SpriteCommon.h"
#include "math/Vector2.h"
#include "math/Vector4.h"
#include <imgui.h>

void TitleScene::Initialize() {

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");

	// スプライトの生成・初期化
	sprite = std::make_unique <Sprite>();
	sprite->Initialize("Resources/monsterBall.png");
}

void TitleScene::Update() {

	sprite->ShowImGui("sprite");
	sprite->Update();

	ImGui::Begin("Scene");
	ImGui::Text("Push Return : Next Scene");
	ImGui::End();

	// エンターキーが押されたら
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {

		// シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw() {

	/// === スプライトの描画準備 === ///
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全てのスプライト個々の描画
	sprite->Draw();
}

void TitleScene::Finalize() {
}