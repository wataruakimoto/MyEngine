#pragma once

#include "Sprite/Sprite.h"

#include <memory>

// 選択肢
enum class PauseSelect {
	None,
	Resume,
	Restart,
	Quit,
};

/// ===== ポーズ用UI ===== ///
class PauseUI {

private:

	struct Button {
		std::unique_ptr<Sprite> sprite; // スプライト
		Vector2 position;               // 位置
		Vector2 size;                   // サイズ
		PauseSelect select;             // 選択肢
	};

public:

	// コンストラクタ
	PauseUI();

	// デストラクタ
	~PauseUI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// ボタン作成処理
	/// </summary>
	/// <param name="select"></param>
	/// <param name="relativePath"></param>
	/// <param name="position"></param>
	void CreateButton(PauseSelect select, const std::string& relativePath, const Vector2& position);

	/// <summary>
	/// ボタンとマウスカーソルの当たり判定
	/// </summary>
	bool CheckButtonSelect(const Button& button, const Vector2& mousePosition);

public:

	PauseSelect GetCurrentSelect() const { return currentSelect_; }

private:

	std::unique_ptr<Sprite> backFilter_; // 背景フィルター

	std::vector<Button> buttons_; // ボタンの配列

	std::unique_ptr<Sprite> selectFrame_; // 選択枠

	float centerX_ = 640.0f; // 画面中央X座標

	float buttonStartY_ = 300.0f; // ボタン開始Y座標

	float buttonGapY_ = 100.0f;   // ボタン間隔Y座標

	int selectedIndex_ = 0;    // 選択中インデックス

	PauseSelect currentSelect_ = PauseSelect::None; // 現在の選択肢
};

