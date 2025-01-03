#include "MyGame.h"
#include "audio/AudioManager.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include <imgui.h>


void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// カメラの初期化
	camera = new Camera();

	// カメラを設定
	object3dCommon->SetDefaultCamera(camera);

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	ModelManager::GetInstance()->LoadModel("axis.obj");

	AudioManager::GetInstance()->SoundLoadWave("resources/fanfare.wav");

	/// === シーンの初期化 === ///

	// スプライトの生成・初期化
	sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");

	// モデルの生成・初期化
	model = new Model();
	model->Initialize(modelCommon, "resources", "plane.obj");

	// 3Dオブジェクトの生成・初期化
	object3d = new Object3d();
	object3d->Initialize(object3dCommon);
	object3d->SetModel(model);

	useModelManager = true;
}

void MyGame::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (winApp->ProcessMessage()) {

		// ゲームループを抜ける
		endRequest_ = true;

	} else {

		// 基底クラス更新
		Framework::Update();

		// サウンド更新
		if (input->TriggerKey(DIK_SPACE)) {

			AudioManager::GetInstance()->SoundPlayWave();
		}

		/// === ImGui開始 === ///
		imGuiManager->Begin();

		/// === カメラ更新 === ///
		ImGui::Begin("Camera");

		// 向き変更
		Vector3 cameraRotate = camera->GetRotate();
		ImGui::DragFloat3("Rotate", &cameraRotate.x, 0.01f);
		camera->SetRotate(cameraRotate);

		// 位置変更
		Vector3 cameraPositon = camera->GetTranslate();
		ImGui::DragFloat3("Translate", &cameraPositon.x, 0.1f);
		camera->SetTranslate(cameraPositon);

		camera->Update();

		ImGui::End();

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

		// モデルを使うかどうかの確認
		ImGui::Checkbox("UseModelManager", &useModelManager);
		if (useModelManager) {

			object3d->SetModel("axis.obj");

		}
		else {

			object3d->SetModel(model);
		}

		ImGui::End();

		/// === ImGui終了 === ///
		imGuiManager->End();
	}
}

void MyGame::Draw() {

	/// ----------DirectX描画開始----------
	dxCommon->PreDraw();

	/// ----------シーンの描画----------

	/// === スプライトの描画準備 === ///
	spriteCommon->SettingCommonDrawing();

	//TODO: 全てのスプライト個々の描画
	sprite->Draw();

	/// === 3Dオブジェクトの描画準備 === ///
	object3dCommon->SettingCommonDrawing();

	//TODO: 全ての3Dオブジェクト個々の描画
	object3d->Draw();

	/// === ImGui描画 === ///
	imGuiManager->Draw();

	/// ----------DirectX描画処理----------
	dxCommon->PostDraw();
}

void MyGame::Finalize() {

	// モデルの解放
	delete model;

	// 3Dオブジェクトの解放
	delete object3d;

	// スプライトの解放
	delete sprite;

	// 基底クラス解放
	Framework::Finalize();
}