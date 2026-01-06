#include "MyGame.h"
#include "DirectXUtility.h"
#include "OffscreenRendering/FilterManager.h"
#include "ImGuiManager.h"
#include "Particle/ParticleCommon.h"
#include "Particle/ParticleManager.h"
#include "SceneManager.h"
#include "SceneFactory.h"

void MyGame::Initialize() {

	// 基底クラス初期化
	Framework::Initialize();

	// シーンファクトリーを生成
	sceneFactory_ = std::make_unique <SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());

	// シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

	// パーティクルマネージャの初期化
	ParticleManager::GetInstance()->Initialize();
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

		// シーンのImGui表示
		SceneManager::GetInstance()->ShowImGui();

		// シーンビュー作成
		sceneRenderTexture->CreateSceneView();

		/// === ImGui終了 === ///
		ImGuiManager::GetInstance()->End();
	}
}

void MyGame::Draw() {

	/// ===== ゲームシーンの描画 ===== ///

	// ポストエフェクトの描画前処理
	postEffect->PreDraw(); // RENDER_TARGET

	// シーンマネージャの描画
	SceneManager::GetInstance()->Draw();

	// ポストエフェクトの描画後処理
	postEffect->PostDraw();  // PIXEL_SHADER_RESOURCE

	/// ===== フィルターへの描画 ===== ///

	// シーンレンダーテクスチャの描画前処理
	sceneRenderTexture->PreDraw(); // RENDER_TARGET

	// フィルターマネージャの描画
	FilterManager::GetInstance()->Draw();

	// シーンレンダーテクスチャの描画後処理
	sceneRenderTexture->PostDraw(); // PIXEL_SHADER_RESOURCE

	/// ===== 画面への描画 ===== ///

	// スワップチェイン描画前処理
	swapChain->PreDraw(); // RENDER_TARGET

	// ImGuiの描画
	ImGuiManager::GetInstance()->Draw();

	// スワップチェイン描画後処理
	swapChain->PostDraw(); // PRESENT

	// DirectXユーティリティの描画後処理
	DirectXUtility::GetInstance()->PostDraw();
}

void MyGame::Finalize() {

	// パーティクルマネージャの終了
	ParticleManager::GetInstance()->Finalize();

	// 基底クラス解放
	Framework::Finalize();
}