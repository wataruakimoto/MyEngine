#pragma once
#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"

#include <memory>
#include <optional>

enum class AnimationStateWarning {
	BounceIn,  // 拡大
	Wait,      // 待機
	ScaleDown, // 縮小
	SecondBounceIn,  // 2回目の拡大
	SecondWait,      // 2回目の待機
	SecondScaleDown, // 2回目の縮小
};

/// ===== 説明UI ===== ///
class RuleUI {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RuleUI();

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
	/// バウンスアニメーション開始
	/// </summary>
	void StartBounceAnimation();

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

	bool IsAnimationFinished() const { return isAnimationFinished_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	std::unique_ptr<Engine::Sprite> ruleSprite_ = nullptr;
	std::unique_ptr<Engine::Sprite> operationSprite_ = nullptr;

	// アニメーション関連
	Engine::Vector2 startScale_ = { 0.0f, 0.0f }; // 開始時のスケール
	Engine::Vector2 maxScale_ = { 1.0f, 1.0f };   // 最大時のスケール
	Engine::Vector2 endScale_ = { 0.0f, 0.0f };	  // 終了時のスケール

	int animationFrame_ = 0;			  // 現在のアニメーションフレーム

	int bounceInDuration_ = 90;		      // 拡大アニメーション時間(フレーム)
	int waitDuration_ = 90;				  // 待機時間(フレーム)
	int scaleDownDuration_ = 60;		  // 縮小アニメーション時間(フレーム)

	bool isAnimating_ = false;			  // アニメーション中フラグ
	bool isAnimationFinished_ = false;	  // アニメーション終了フラグ

	const Engine::Vector2 kDefaultSize = { 896.0f, 504.0f }; // デフォルトのサイズ

	AnimationStateWarning animationState_ = AnimationStateWarning::BounceIn;
};

