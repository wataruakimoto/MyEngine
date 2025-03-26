#include "game/Framework.h"
#include "game/MyGame.h"
#include "debug/D3DResourceLeakChecker.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ゲームの生成
	Framework* game = new MyGame();

	// ゲーム実行
	game->Run();

	// ゲームの解放
	delete game;

	// リークチェック
	D3DResourceLeakChecker leakCheck;

	return 0;
}