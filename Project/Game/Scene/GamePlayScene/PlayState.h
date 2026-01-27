#pragma once

#include "Vector2.h"

/// ===== 前方宣言 ===== ///

class GamePlayScene;
class FilterManager;
class RadialBlurFilter;
class Player;
class RuleUI;

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
/// メンバ変数
///-------------------------------------------///
protected:

	// シーンの借りポインタ
	GamePlayScene* scene_ = nullptr;
};

/// ===== プレイシーンのイントロ状態 ===== ///
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

	// ルールUIの借りポインタ
	RuleUI* ruleUI_ = nullptr;
};

/// ===== プレイ状態 ===== ///
class PlayState : public IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize(GamePlayScene* scene) override;

	void Update() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	int killCount_ = 0;

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
};
	
/// ===== リザルト状態 ===== ///
class ResultState : public IPlayState {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	void Initialize(GamePlayScene* scene) override;

	void Update() override;

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

};
