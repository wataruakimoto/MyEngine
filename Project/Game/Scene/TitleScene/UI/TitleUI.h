#pragma once
#include "2D/Texture/TextureManager.h"
#include "2D/Sprite/Sprite.h"

#include <memory>
#include <optional>

enum class AnimationState {
	SlideIn, // スライドイン
	MoveUp,  // 上に移動
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

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

	static float EaseOutCubic(float t);

	static float EaseInCubic(float t);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	bool IsSlideInFinished() { return isSlideFinished_; }

	bool IsMoveUpFinished() { return isMoveUpFinished_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// 終了フラグ
	bool isSlideFinished_ = false;
	bool isMoveUpFinished_ = false;

	// アニメーション状態
	AnimationState animationState_ = AnimationState::SlideIn;

	// アニメーション状態リクエスト
	std::optional<AnimationState> animationStateRequest_ = std::nullopt;

	// アニメーション時間
	float animationTime_ = 0.0f;

	// Tweenパラメータ
	Vector2 tweenStart_ = { 0.0f, 0.0f };
	Vector2 tweenEnd_ = { 0.0f, 0.0f };
	int tweenFrame_ = 0;
	int tweenDuration_ = 0;

	// アニメーションを何秒で行うか
	const int kAnimationDuration = 1;
};

