#pragma once

#include "Vector2.h"

/// ===== 前方宣言 ===== ///

class GamePlayScene;
class FilterManager;
class RadialBlurFilter;
class VignetteFilter;
class ICameraController;
class Player;
class Goal;
class Reticle3D;
class Reticle2D;
class LockOn;
class RuleUI;
class NormaUI;
class GuideUI;
class ResultUI;
class WhiteFade;
class BlackFade;

/// ===== プレイシーンの状態 ===== ///
class IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	virtual ~IPlayState() = default;

	virtual void Initialize(GamePlayScene* scene) = 0;

	virtual void Update() = 0;

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	bool IsFinished() const { return isFinished_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
protected:

	bool isFinished_ = false;

	// シーンの借りポインタ
	GamePlayScene* scene_ = nullptr;
};

/// ===== イントロ状態 ===== ///
class IntroState : public IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize(GamePlayScene* scene) override;

	void Update() override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 減速処理
	/// </summary>
	void Deceleration();

	/// <summary>
	/// UI表示処理
	/// </summary>
	void ShowUI();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	/// ===== 減速用 ===== ///

	// 減速タイマー カウントアップ
	float decelerationTimer_ = 0.0f;

	// 減速にかける時間(秒)
	const float kDecelerationDuration = 3.0f;

	// 減速完了フラグ
	bool isDecelerateFinished_ = false;
	
	// 減速開始時の速度
	float initialSpeed_ = 0.0f;

	// 減速開始時のブラー強度
	float initialBlurStrength_ = 0.0f;

	// ブラーの中心座標
	Vector2 blurCenter_ = {};

	/// ===== UI表示用 ===== ///

	// UIアニメーション開始フラグ
	bool isAnimationStarted_ = false;

	/// ===== 借りポインタ・インスタンス ===== ///

	// ラジアルブラー借りポインタ
	RadialBlurFilter* radialBlurFilter_ = nullptr;

	// フィルターマネージャのインスタンス
	FilterManager* filterManager_;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// ルールUIの借りポインタ
	RuleUI* ruleUI_ = nullptr;

	// 白フェードの借りポインタ
	WhiteFade* whiteFade_ = nullptr;
};

/// ===== プレイ状態 ===== ///
class PlayState : public IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize(GamePlayScene* scene) override;

	void Update() override;

	/// <summary>
	/// プレイヤーがダメージを受けたときの処理
	/// </summary>
	/// <param name="currentHP">現在のHP</param>
	void OnPlayerDamaged(uint16_t currentHP);

	/// <summary>
	/// 敵を倒したときの処理
	/// </summary>
	void OnEnemyDefeated();

///-------------------------------------------///
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// ビネットエフェクトの更新
	/// </summary>
	void UpdateVignetteEffect();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// ビネットエフェクト用変数
	bool isDamageVignetteActive_ = false; // ダメージ時の一時ビネット有効フラグ
	float damageVignetteTimer_ = 0.0f; // ダメージビネットのタイマー
	const float kDamageVignetteDuration_ = 0.5f; // ダメージビネットの持続時間 (秒)

	/// ===== 借りポインタ・インスタンス ===== ///

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// 3Dレティクルの借りポインタ
	Reticle3D* reticle3D_ = nullptr;

	// 2Dレティクルの借りポインタ
	Reticle2D* reticle2D_ = nullptr;

	// ロックオンの借りポインタ
	LockOn* lockOn_ = nullptr;

	// ノルマUIの借りポインタ
	NormaUI* normaUI_ = nullptr;

	// ガイドUIの借りポインタ
	GuideUI* guideUI_ = nullptr;

	// カメラコントローラーの借りポインタ
	ICameraController* cameraController_ = nullptr;

	// フィルターマネージャのインスタンス
	FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	VignetteFilter* vignetteFilter_ = nullptr;
};
	
/// ===== エンディング状態 ===== ///
class EndingState : public IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize(GamePlayScene* scene) override;

	void Update() override;

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	void ShowUI();

	void FadeIn();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	bool isClear_ = false;

	bool isPlayerDead_ = false;
	
	bool isFadeStarted_ = false;

	bool isAnimationFinished_ = false;

	/// ===== 借りポインタ・インスタンス ===== ///

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// リザルトUIの借りポインタ
	ResultUI* resultUI_ = nullptr;

	// 白フェードの借りポインタ
	WhiteFade* whiteFade_ = nullptr;

	// 黒フェードの借りポインタ
	BlackFade* blackFade_ = nullptr;

	// フィルターマネージャのインスタンス
	FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	VignetteFilter* vignetteFilter_ = nullptr;
};
