#pragma once

#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"

#include <memory>

enum class AnimationState {
	SlideIn, // スライドイン
	MoveUp,  // 上に移動
	Hover,   // 浮遊
};

/// ===== タイトル用UI ===== ///
class TitleUI {

///-------------------------------------------///
/// メンバ関数
///-------------------------------------------///
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleUI();

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

	/// <summary>
	/// 横スライドインアニメーション開始
	/// </summary>
	void StartSlideInAnimation();

	/// <summary>
	/// 上に移動アニメーション開始
	/// </summary>
	void StartMoveUpAnimation();

	/// <summary>
	/// 浮遊アニメーション開始
	/// </summary>
	void StartHoverAnimation();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	bool IsSlideInFinished() { return slideInTween_.isFinished; }

	bool IsMoveUpFinished() { return moveUpTween_.isFinished; }

private:

	struct TweenParameters {
		Engine::Vector2 start;
		Engine::Vector2 end;
		int frame;
		int duration;
		bool isFinished;
	};

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャのインスタンス
	Engine::TextureManager* textureManager_ = Engine::TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Engine::Sprite> sprite_ = nullptr;

	// アニメーション状態
	AnimationState animationState_ = AnimationState::SlideIn;

	// アニメーション時間
	float animationTime_ = 0.0f;

	// 各アニメーションのTweenパラメータ
	TweenParameters slideInTween_;
	TweenParameters moveUpTween_;
	TweenParameters hoverTween_;

	// アニメーションを何秒で行うか
	const int kAnimationDuration = 1;

	// ホバーアニメーションの反転フラグ
	bool isHoverReverse_ = false;
};

