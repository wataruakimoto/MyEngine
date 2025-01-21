#include "GameScene.h"
#include "audio/AudioManager.h"
#include "input/Input.h"
#include "2d/TextureManager.h"
#include "2d/SpriteCommon.h"
#include "3d/Object3dCommon.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include <imgui.h>

void GameScene::Initialize() {

	// カメラの初期化
	camera = new Camera();

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");

	AudioManager::GetInstance()->SoundLoadWave("resources/fanfare.wav");

	/// === シーンの初期化 === ///

	// スプライトの生成・初期化
	sprite = new Sprite();
	sprite->Initialize("resources/uvChecker.png");

	// モデルの生成・初期化
	model = new Model();
	model->Initialize("resources/sphere", "sphere.obj");

	// 3Dオブジェクトの生成・初期化
	object3d = new Object3d();
	object3d->Initialize();
	object3d->SetModel(model);
	object3d->SetCamera(camera);
}

void GameScene::Update() {

	// サウンド更新
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
	
		AudioManager::GetInstance()->SoundPlayWave();
	}

	/// === カメラ更新 === ///
	camera->Update();

	/// === スプライト更新 === ///

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

	/// === 3Dオブジェクト更新 === ///

	object3d->Update();

	ImGui::Begin("Object3d");

	// 大きさ変更の確認
	Vector3 scale = object3d->GetScale();
	ImGui::DragFloat3("Scale", &scale.x, 0.1f);
	object3d->SetScale(scale);

	// 回転変更の確認
	Vector3 rotate = object3d->GetRotate();
	ImGui::DragFloat3("Rotate", &rotate.x, 0.01f);
	object3d->SetRotate(rotate);

	// 座標変更の確認
	Vector3 translate = object3d->GetTranslate();
	ImGui::DragFloat3("Translate", &translate.x, 0.1f);
	object3d->SetTranslate(translate);

	// 色変更の確認
	Vector4 rightColor = object3d->GetColor();
	ImGui::DragFloat4("Color", &rightColor.x, 0.01f);
	object3d->SetColor(rightColor);

	// 向き変更の確認
	Vector3 direction = object3d->GetDirection();
	ImGui::DragFloat3("Direction", &direction.x, 0.01f);
	object3d->SetDirection(direction);

	// 輝度変更の確認
	float intensity = object3d->GetIntensity();
	ImGui::DragFloat("Intensity", &intensity, 0.01f);
	object3d->SetIntensity(intensity);

	// ライティングの仕方の設定
	int lightingMode = model->GetLightingMode();
	ImGui::Combo("LightingMode", &lightingMode, "None\0Lambertian Reflection\0Harf Lambert\0Phong Reflection Model\0Blinn-Phong Reflection Model\0");
	model->SetLightingMode(lightingMode);

	// 光沢度の設定の確認
	float lightShininess = model->GetShininess();
	ImGui::DragFloat("Shininess", &lightShininess, 0.1f);
	model->SetShininess(lightShininess);

	ImGui::End();
}

void GameScene::Draw() {

	/// === スプライトの描画準備 === ///
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全てのスプライト個々の描画
	sprite->Draw();

	/// === 3Dオブジェクトの描画準備 === ///
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画
	object3d->Draw();
}

void GameScene::Finalize() {

	// モデルの解放
	delete model;

	// 3Dオブジェクトの解放
	delete object3d;

	// スプライトの解放
	delete sprite;
}