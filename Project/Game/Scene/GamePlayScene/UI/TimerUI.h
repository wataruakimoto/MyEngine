#pragma once

#include "Sprite/Sprite.h"

#include <memory>
#include <array>

/// <summary>
/// 制限時間表示UI
/// </summary>
class TimerUI {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
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

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 残り時間を分秒に変換する
	/// </summary>
	void ConvertTimeToMinuteSecond();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	std::array<std::unique_ptr<Engine::Sprite>, 10> timeSprites_;
};

