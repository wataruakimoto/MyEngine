#include "MyGame.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	titleScene = new TitleScene();
	titleScene->Initialize();

	//gameScene = new GameScene();
	//gameScene->Initialize();
}

void MyGame::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (winApp->ProcessMessage()) {

		// ゲームループを抜ける
		endRequest_ = true;

	} else {

		// 基底クラス更新
		Framework::Update();

		/// === ImGui開始 === ///
		imGuiManager->Begin();

		titleScene->Update();

		//gameScene->Update();

		/// === ImGui終了 === ///
		imGuiManager->End();
	}
}

void MyGame::Draw() {

	/// === DirectX描画開始 === ///
	dxCommon->PreDraw();

	titleScene->Draw();

	//gameScene->Draw();

	/// === ImGui描画 === ///
	imGuiManager->Draw();

	/// === DirectX描画処理 === ///
	dxCommon->PostDraw();
}

void MyGame::Finalize() {

	titleScene->Finalize();
	delete titleScene;

	//gameScene->Finalize();
	//delete gameScene;

	// 基底クラス解放
	Framework::Finalize();
}