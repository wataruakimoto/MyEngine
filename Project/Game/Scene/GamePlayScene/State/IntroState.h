#pragma once

#include "IPlayState.h"
#include "UI/RuleUI.h"

/// === 前方宣言 === ///

namespace Engine {
	class FilterManager;
	class RadialBlurFilter;
}

class GameObjectManager;
class Player;
class Goal;

/// <summary>
/// イントロ状態
/// </summary>
class IntroState : public IPlayState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene"></param>
	void Initialize(GamePlayScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 減速処理
	/// </summary>
	void Deceleration();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ===== 減速用 ===== ///

	// 減速タイマー カウントアップ
	float decelerationTimer_ = 0.0f;

	// 減速にかける時間(秒)
	const float kDecelerationDuration = 2.0f;

	// 減速完了フラグ
	bool isDecelerateFinished_ = false;

	// 減速開始時の速度
	float initialSpeed_ = 0.0f;

	// 減速開始時のブラー強度
	float initialBlurStrength_ = 0.0f;

	// ブラーの中心座標
	Engine::Vector2 blurCenter_ = {};

	/// ===== UI表示用 ===== ///

	// UIアニメーション開始フラグ
	bool isAnimationStarted_ = false;

	// スペースキー長押しで早送りする倍率 (1フレームで進める回数)
	const int kFastForwardSpeed_ = 4;

	/// ===== 借りポインタ・インスタンス ===== ///

	// オブジェクトマネージャのインスタンス
	GameObjectManager* gameObjectManager_ = nullptr;

	// ラジアルブラー借りポインタ
	Engine::RadialBlurFilter* radialBlurFilter_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// ルールUI
	std::unique_ptr<RuleUI> ruleUI_ = nullptr;
};

