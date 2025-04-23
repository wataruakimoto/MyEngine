#include "MyGame.h"
#include "debug/ImGuiManager.h"
#include "3d/ParticleCommon.h"
#include "3d/ParticleSystem.h"
#include "scene/SceneManager.h"
#include "scene/SceneFactory.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// シーンファクトリーを生成
	sceneFactory_ = std::make_unique <SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
	
	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

	// パーティクルシステムの初期化
	ParticleSystem::GetInstance()->Initialize();
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
		ImGuiManager::GetInstance()->Begin();

		// シーンマネージャの更新
		SceneManager::GetInstance()->Update();

		// パーティクルシステムの更新
		ParticleSystem::GetInstance()->Update();

		/// === ImGui終了 === ///
		ImGuiManager::GetInstance()->End();
	}
}

void MyGame::Draw() {

	/// === レンダーテクスチャ描画開始=== ///
	renderTexture->PreDraw();

	// シーンマネージャの描画
	SceneManager::GetInstance()->Draw();

	/// === パーティクルの描画準備 === ///
	ParticleCommon::GetInstance()->SettingDrawing();

	// パーティクルシステムの描画
	ParticleSystem::GetInstance()->Draw();

	/// === スワップチェイン描画開始 === ///
	swapChain->PreDraw();

	/// === ImGui描画 === ///
	ImGuiManager::GetInstance()->Draw();

	/// === レンダーテクスチャ描画処理 === ///
	renderTexture->PostDraw();

	/// === スワップチェイン描画処理 === ///
	swapChain->PostDraw();

	/// === DirectX描画処理 === ///
	dxUtility->PostDraw();
}

void MyGame::Finalize() {

	// パーティクルシステムの終了
	ParticleSystem::GetInstance()->Finalize();

	// 基底クラス解放
	Framework::Finalize();
}