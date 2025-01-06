#include "MyGame.h"
#include "scene/SceneManager.h"
#include "scene/TitleScene.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// 最初のシーンを生成
	BaseScene* scene = new TitleScene();
	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->SetNextScene(scene);
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

		// シーンマネージャの更新
		SceneManager::GetInstance()->Update();

		/// === ImGui終了 === ///
		imGuiManager->End();
	}
}

void MyGame::Draw() {

	/// === DirectX描画開始 === ///
	dxCommon->PreDraw();

	// シーンマネージャの描画
	SceneManager::GetInstance()->Draw();

	/// === ImGui描画 === ///
	imGuiManager->Draw();

	/// === DirectX描画処理 === ///
	dxCommon->PostDraw();
}

void MyGame::Finalize() {

	// 基底クラス解放
	Framework::Finalize();
}