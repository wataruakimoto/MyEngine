#pragma once

#include "BaseTransition.h"
#include "Sprite/Sprite.h"
#include "Vector3.h"
#include "Vector4.h"

#include <memory>

namespace Engine {

	/// ========== 前方宣言 ========== ///

	class SpriteRenderer;
}

/// <summary>
/// フェード遷移クラス
/// </summary>
class FadeTransition : public Engine::BaseTransition {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="color">色</param>
	FadeTransition(Engine::Vector3 color, float startAlpha, float endAlpha);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FadeTransition() override = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="progress"></param>
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

	// フェードの色
	Engine::Vector4 color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	float startAlpha_ = 0.0f;
	float endAlpha_ = 1.0f;

	// スプライト
	std::unique_ptr<Engine::Sprite> sprite_ = nullptr;

	// スプライトレンダラーのポインタ
	Engine::SpriteRenderer* spriteRenderer_ = nullptr;
};

