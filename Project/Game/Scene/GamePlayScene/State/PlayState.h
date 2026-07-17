#pragma once

#include "IPlayState.h"
#include "UI/GuideUI.h"
#include "UI/TimerUI.h"

#include <memory>

/// === 前方宣言 === ///

namespace Engine {
	class FilterManager;
	class VignetteFilter;
}

class GameObjectManager;
class Player;
class Goal;
class ICameraController;

/// <summary>
/// プレイ状態
/// </summary>
class PlayState : public IPlayState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="scene">ゲームプレイシーンのポインタ</param>
	void Initialize(GamePlayScene* scene) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// プレイヤーがダメージを受けたときの処理
	/// </summary>
	void OnPlayerDamaged();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// ビネットエフェクトの更新
	/// </summary>
	void UpdateVignetteEffect();

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	/// <summary>
	/// ゴールフラグのセッター
	/// </summary>
	/// <param name="isGoalReached">ゴールしたかどうか</param>
	void SetGoalReached(bool isGoalReached) { isGoalReached_ = isGoalReached; }

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ===== ビネットエフェクト用変数 ===== ///

	// 被弾時の点滅 (1回だけ光らせる)
	bool isDamageVignetteActive_ = false; // ダメージ時のビネット有効フラグ
	float damageVignetteTimer_ = 0.0f; // ダメージビネットの経過時間
	const float kDamageVignetteDuration_ = 0.3f; // ダメージビネットの持続時間 (秒)

	// 残り時間が少ないときの点滅
	const float kLowTimeThreshold_ = 10.0f; // この秒数以下になったら点滅開始
	bool lowTimeBlinkState_ = false; // 点滅のON/OFF状態
	float lowTimeBlinkTimer_ = 0.0f; // 点滅間隔用タイマー
	const float kLowTimeBlinkInterval_ = 0.5f; // 点滅間隔 (秒)

	// ゴールしたかどうか
	bool isGoalReached_ = false;

	/// ===== 借りポインタ・インスタンス ===== ///

	// オブジェクトマネージャのインスタンス
	GameObjectManager* gameObjectManager_ = nullptr;

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// ガイドUI
	std::unique_ptr<GuideUI> guideUI_ = nullptr;

	// タイマーUI
	std::unique_ptr<TimerUI> timerUI_ = nullptr;

	// カメラコントローラーの借りポインタ
	ICameraController* cameraController_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	Engine::VignetteFilter* vignetteFilter_ = nullptr;
};