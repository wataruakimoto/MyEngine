#pragma once

#include "Texture/TextureManager.h"
#include "Sprite/Sprite.h"

#include <memory>

enum class ResultAnimationState {
	SlideDown, // スライドダウン
	Wait,      // 待機
};

enum class ResultType {
	Clear,    // クリア
	GameOver, // ゲームオーバー
};

/// ===== リザルトUI ===== ///
class ResultUI {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultUI();

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

	void StartAnimation(ResultType resultType);

///-------------------------------------------///
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// アニメーション更新
	/// </summary>
	void AnimationUpdate();

	static float EaseOutQuart(float t);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	bool IsAnimationFinished() const { return isAnimationFinished_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// テクスチャマネージャのインスタンス
	TextureManager* textureManager_ = TextureManager::GetInstance();

	// クリアスプライト
	std::unique_ptr<Sprite> clearSprite_;

	// ゲームオーバースプライト
	std::unique_ptr<Sprite> gameOverSprite_;

	// アニメーション関連
	Vector2 startPosition_ = { 640.0f, -300.0f }; // 開始位置（画面外上部）
	Vector2 targetPosition_ = { 640.0f, 288.0f }; // 目標位置（画面中央）
	Vector2 endPosition_ = { 640.0f, 1000.0f };   // 終了位置（画面外下部）

	int animationFrame_ = 0;			  // 現在のアニメーションフレーム

	int slideDownDuration_ = 60;		  // スライドダウン時間(フレーム)
	int waitDuration_ = 90;				  // 待機時間(フレーム)
	int slideUpDuration_ = 45;			  // スライドアップ時間(フレーム)

	bool isAnimating_ = false;			  // アニメーション中フラグ
	bool isAnimationFinished_ = false;	  // アニメーション終了フラグ

	ResultAnimationState animationState_ = ResultAnimationState::SlideDown;

	// 結果タイプ
	ResultType resultType_ = ResultType::Clear;
};

