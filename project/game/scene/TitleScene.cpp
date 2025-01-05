#include "TitleScene.h"
#include "2d/TextureManager.h"
#include "2d/SpriteCommon.h"
#include "math/Vector2.h"
#include "math/Vector4.h"
#include <imgui.h>

void TitleScene::Initialize() {

	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	// スプライトの生成・初期化
	sprite = new Sprite();
	sprite->Initialize("resources/monsterBall.png");
}

void TitleScene::Update() {

	ImGui::Begin("Sprite");

	// サイズ変更の確認
	Vector2 size = sprite->GetSize();
	ImGui::DragFloat2("Size", &size.x, 1.0f);
	sprite->SetSize(size);

	// 回転変更の確認
	float rotation = sprite->GetRotation();
	ImGui::DragFloat("Rotation", &rotation, 0.01f);
	sprite->SetRotation(rotation);

	// 座標変更の確認
	Vector2 position = sprite->GetPosition();
	ImGui::DragFloat2("Position", &position.x, 1.0f);
	sprite->SetPosition(position);

	// 色変更の確認
	Vector4 color = sprite->GetColor();
	ImGui::DragFloat4("Color", &color.x, 0.01f);
	sprite->SetColor(color);

	// アンカー変更の確認
	Vector2 anchorPoint = sprite->GetAnchorPoint();
	ImGui::SliderFloat2("Anchor", &anchorPoint.x, -1.0f, 1.0f);
	sprite->SetAnchorPoint(anchorPoint);

	// フリップ変更の確認
	bool isFlipX = sprite->GetIsFlipX();
	bool isFlipY = sprite->GetIsFlipY();
	ImGui::Checkbox("IsFlipX", &isFlipX);
	ImGui::Checkbox("IsFlipY", &isFlipY);
	sprite->SetIsFlipX(isFlipX);
	sprite->SetIsFlipY(isFlipY);

	// テクスチャ左上座標の確認
	Vector2 textureLeftTop = sprite->GetTextureLeftTop();
	ImGui::DragFloat2("TextureLeftTop", &textureLeftTop.x, 1.0f);
	sprite->SetTextureLeftTop(textureLeftTop);

	// テクスチャ切り出しサイズの確認
	Vector2 textureSize = sprite->GetTextureSize();
	ImGui::DragFloat2("TextureSize", &textureSize.x, 1.0f);
	sprite->SetTextureSize(textureSize);

	sprite->Update();

	ImGui::End();
}

void TitleScene::Draw() {

	/// === スプライトの描画準備 === ///
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全てのスプライト個々の描画
	sprite->Draw();
}

void TitleScene::Finalize() {

	delete sprite;
}