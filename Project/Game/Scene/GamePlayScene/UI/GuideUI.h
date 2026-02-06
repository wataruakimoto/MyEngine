#pragma once

#include "Sprite/Sprite.h"

#include <memory>

namespace Engine {
	class Input;
}

/// ===== ガイドUI ===== ///
class GuideUI {

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

private:

	// マウススプライト
	std::unique_ptr<Engine::Sprite> mouseSprite = nullptr;

	// ボタンAスプライト
	std::unique_ptr<Engine::Sprite> buttonASprite = nullptr;

	// ボタンDスプライト
	std::unique_ptr<Engine::Sprite> buttonDSprite = nullptr;

	// ボタンESCスプライト
	std::unique_ptr<Engine::Sprite> buttonESCSprite = nullptr;

	bool isShooting = false;

	bool isRollLeft = false;

	bool isRollRight = false;

	bool isPause = false;

	// input
	Engine::Input* input = nullptr;
};

