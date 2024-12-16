#include "game/MyGame.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// 生成
	MyGame game;

	// 初期化
	game.Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (true) {

		// 更新
		game.Update();

		// 描画
		game.Draw();
	}

	// ゲーム終了
	game.Finalize();

	return 0;
}