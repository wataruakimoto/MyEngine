#include "Windows.h"
#include "Framework/Framework.h"
#include "Base/MyGame.h"
#include "Debug/D3DResourceLeakChecker.h"

using namespace Engine;

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