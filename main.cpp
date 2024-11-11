#include <dxgidebug.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "MathVector.h"
#include "MathMatrix.h"
#include <cmath>
#include <fstream>
#include <sstream>

#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

#pragma comment(lib,"dxcompiler.lib")

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResourceLeakChecker.h"
#include "Logger.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3dCommon.h"
#include "Object3d.h"
#include "TextureManager.h"

using namespace MathMatrix;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	///
	/// 初期化処理開始
	///

	/// ----------ポインタ置き場----------

	// 入力のポインタ
	Input* input = nullptr;

	// WindowsAPIのポインタ
	WinApp* winApp = nullptr;

	// DirectX基盤のポインタ
	DirectXCommon* dxCommon = nullptr;

	// スプライト共通部のポインタ
	SpriteCommon* spriteCommon = nullptr;

	// 3Dオブジェクト共通部のポインタ
	Object3dCommon* object3dCommon = nullptr;

	/// ----------ゲームウィンドウ作成----------

	// WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	// ウィンドウを表示する
	ShowWindow(winApp->GetHwnd(), SW_SHOW);

	/// ----------DirectX初期化----------

	// DirectXの初期化
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	/// ----------汎用機能初期化----------

	// 入力の初期化
	input = new Input();
	input->Initialize(winApp);

	// スプライト共通部初期化
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon);

	// 3Dオブジェクト共通部初期化
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize(dxCommon);

	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	/// ----------シーンの初期化----------

	// スプライトの生成・初期化
	Sprite* sprite = new Sprite();
	sprite->Initialize(spriteCommon, "resources/uvChecker.png");

	// 3Dオブジェクトの生成・初期化
	Object3d* object3d = new Object3d();
	object3d->Initialize(object3dCommon);

	///
	/// 初期化処終了
	///

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {

		///
		/// メインループ開始
		///

		/// ----------Windowsメッセージ処理----------

		// Windowにメッセージが来てたら最優先で処理させる
		if (winApp->ProcessMessage()) {

			// ゲームループを抜ける
			break;

		} else {

			///
			/// 更新処理開始
			///

			/// ----------入力の更新----------
			input->Update();

			//if (input->PushKey(DIK_UP)) {
			//	transform.translate.y += 0.1f;
			//}
			//
			//if (input->TriggerKey(DIK_DOWN)) {
			//	transform.translate.y += -0.1f;
			//}
			//
			//if (input->PushKey(DIK_RIGHT)) {
			//	transform.translate.x += 0.1f;
			//}
			//
			//if (input->TriggerKey(DIK_LEFT)) {
			//	transform.translate.x += -0.1f;
			//}

			/// ----------シーンの更新----------

			/// === ImGui開始 === ///

			// ImGuiにフレーム開始を告げる
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

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

			// ImGuiの内部コマンドを生成する
			ImGui::Render();

			/// === ImGui終了 === ///

			///
			/// 更新処理終了
			///

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

			// 実際のcommandListのImGuiの描画コマンドを積む
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandList().Get());

			/// ----------DirectX描画処理----------
			dxCommon->PostDraw();

			///
			/// 描画処理終了
			///

			///
			/// メインループ終了
			///
		}
	}

	///
	/// 解放処理開始
	///

	/// ----------最初のシーンの解放----------

	//textureResource2.Reset();
	//textureResource.Reset();
	//vertexResource.Reset();
	//directionalLightResource.Reset();
	//materialResource.Reset();
	//wvpResource.Reset();
	//graphicsPipelineState.Reset();
	//pixelShaderBlob.Reset();
	//vertexShaderBlob.Reset();
	//rootSignature.Reset();
	//errorBlob.Reset();
	//signatureBlob.Reset();

	// 3Dオブジェクトの解放
	delete object3d;

	// スプライトの解放
	delete sprite;

	/// ----------汎用機能の解放----------

	// テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();

	// 3Dオブジェクト共通部の解放
	delete object3dCommon;

	// スプライト共通部の解放
	delete spriteCommon;

	// 入力の解放
	delete input;

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

	return 0;
}