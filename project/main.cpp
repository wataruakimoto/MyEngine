#include "game/MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ゲームの生成
	MyGame* game = new MyGame();

	// ゲームの初期化
	game->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {

		// ゲームの更新
		game->Update();

		// ゲームの描画
		game->Draw();
	}

	// ゲーム終了
	game->Finalize();

	// ゲーム削除
	delete game;

	return 0;
}