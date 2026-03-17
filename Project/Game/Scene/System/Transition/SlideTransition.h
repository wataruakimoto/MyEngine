#pragma once

#include "BaseTransition.h"
#include "Sprite/Sprite.h"
#include "Vector2.h"

#include <memory>

namespace Engine {
	
	/// ========== 前方宣言 ========== ///

	class SpriteRenderer;
}

/// <summary>
/// スライド遷移クラス
/// </summary>
class SlideTransition : public Engine::BaseTransition {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="endPosition">終了座標</param>
	SlideTransition(Engine::Vector2 endPosition);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SlideTransition() override = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="progress">進行度</param>
	void Update(float progress) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// 画面の中心座標
	Engine::Vector2 startPosition_  = { 0.0f, 0.0f };

	// 終了座標
	Engine::Vector2 endPosition_ = { 0.0f, 0.0f };

	// スプライト
	std::unique_ptr<Engine::Sprite> sprite_ = nullptr;

	// スプライトレンダラーのポインタ
	Engine::SpriteRenderer* spriteRenderer_ = nullptr;
};

