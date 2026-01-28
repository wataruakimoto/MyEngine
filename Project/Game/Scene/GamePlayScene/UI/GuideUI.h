#pragma once

#include "Sprite/Sprite.h"

#include <memory>

class Input;

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
	std::unique_ptr<Sprite> mouseSprite = nullptr;

	// ボタンAスプライト
	std::unique_ptr<Sprite> buttonASprite = nullptr;

	// ボタンDスプライト
	std::unique_ptr<Sprite> buttonDSprite = nullptr;

	// ボタンESCスプライト
	std::unique_ptr<Sprite> buttonESCSprite = nullptr;

	bool isShooting = false;

	bool isRollLeft = false;

	bool isRollRight = false;

	bool isPause = false;

	// input
	Input* input = nullptr;
};

