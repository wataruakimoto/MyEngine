#include "Windows.h"
#include "game/Framework.h"
#include "game/MyGame.h"
#include "debug/D3DResourceLeakChecker.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// リークチェック
	D3DResourceLeakChecker leakCheck;

	// ゲームの生成
	std::unique_ptr <Framework> game = std::make_unique<MyGame>();

	// ゲーム実行
	game->Run();

	return 0;
}