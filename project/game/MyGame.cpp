#include "MyGame.h"

#include "debug/D3DResourceLeakChecker.h"
#include "debug/Logger.h"
#include "2d/TextureManager.h"
#include "3d/ModelManager.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "audio/AudioManager.h"
#include <imgui.h>


void MyGame::Initialize() {

	/// === ゲームウィンドウ作成 === ///

	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	/// === DirectX初期化 === ///

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	/// === 汎用機能初期化 === ///

	// ImGuiの初期化
	imGuiManager = new ImGuiManager();
	imGuiManager->Initialize(winApp, dxCommon);

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	// カメラの初期化
	camera = new Camera();

	// スプライト共通部初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);
	object3dCommon->SetDefaultCamera(camera);

	// モデル基盤初期化
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	// モデルマネージャ初期化
	ModelManager::GetInstance()->Initialize(dxCommon);

	ModelManager::GetInstance()->LoadModel("axis.obj");

	// オーディオマネージャ初期化
	AudioManager::GetInstance()->Initialize();

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

	bool useModelManager = true;
}

void MyGame::Update() {

	/// === Windowsメッセージ処理=== ///

		// Windowにメッセージが来てたら最優先で処理させる
	if (winApp->ProcessMessage()) {

		// ゲームループを抜ける
		break;

	}
	else {

		///
		/// 更新処理開始
		///

		/// ----------入力の更新----------
		input->Update();

		/// ----------シーンの更新----------

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

		///
		/// 更新処理終了
		///
	}
}

void MyGame::Draw() {

	///
	/// 描画処理開始
	///

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

	///
	/// 描画処理終了
	///
}

void MyGame::Finalize() {

	///
	/// 解放処理開始
	///

	/// ----------最初のシーンの解放----------

	// モデルの解放
	delete model;

	// 3Dオブジェクトの解放
	delete object3d;

	// スプライトの解放
	delete sprite;

	/// ----------汎用機能の解放----------

	// モデルマネージャの終了
	ModelManager::GetInstance()->Finalize();

	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();

	// モデル基盤の解放
	delete modelCommon;

	// 3Dオブジェクト共通部の解放
	delete object3dCommon;

	// スプライト共通部の解放
	delete spriteCommon;

	// オーディオマネージャ終了
	AudioManager::GetInstance()->Finalize();

	// 入力の解放
	delete input;

	// ImGui終了&解放
	imGuiManager->Finalize();
	delete imGuiManager;

	/// ----------DirectXの解放----------
	delete dxCommon;

	/// ----------ゲームウィンドウ解放----------

	// WindowsAPIの終了処理
	winApp->Finalize();
	// WindowsAPIの解放
	delete winApp;

	D3DResourceLeakChecker leakCheck;

	///
	/// 解放処理終了
	///
}