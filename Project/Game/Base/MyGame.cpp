#include "MyGame.h"
#include "Base/DirectXUtility.h"
#include "Base/OffscreenRendering/FilterManager.h"
#include "Debug/ImGuiManager.h"
#include "3D/Particle/ParticleCommon.h"
#include "3D/Particle/ParticleSystem.h"
#include "Scene/SceneManager.h"
#include "Scene/System/SceneFactory.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// シーンファクトリーを生成
	sceneFactory_ = std::make_unique <SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());

	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("DEBUG");

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

		// フィルターマネージャのImGui表示
		FilterManager::GetInstance()->ShowImGui();

		// シーンマネージャの更新
		SceneManager::GetInstance()->Update();

		/// === ImGui終了 === ///
		ImGuiManager::GetInstance()->End();
	}
}

void MyGame::Draw() {

	/// === レンダーテクスチャ描画開始=== ///
	postEffect->PreDraw();

	// シーンマネージャの描画
	SceneManager::GetInstance()->Draw();

	/// === レンダーテクスチャ描画処理 === ///
	postEffect->PostDraw();

	/// === スワップチェイン描画開始 === ///
	swapChain->PreDraw();

	// フィルターマネージャの描画
	FilterManager::GetInstance()->Draw();

	/// === ImGui描画 === ///
	ImGuiManager::GetInstance()->Draw();

	/// === スワップチェイン描画処理 === ///
	swapChain->PostDraw();

	/// === DirectX描画処理 === ///
	DirectXUtility::GetInstance()->PostDraw();
}

void MyGame::Finalize() {

	// パーティクルシステムの終了
	ParticleSystem::GetInstance()->Finalize();

	// 基底クラス解放
	Framework::Finalize();
}