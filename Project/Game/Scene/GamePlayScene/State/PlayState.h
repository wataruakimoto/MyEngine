#pragma once

#include "IPlayState.h"
#include "UI/GuideUI.h"

#include <memory>

/// === 前方宣言 === ///

namespace Engine {
	class FilterManager;
	class VignetteFilter;
}

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

	/// <summary>
	/// プレイヤーがダメージを受けたときの処理
	/// </summary>
	/// <param name="currentHP">現在のHP</param>
	void OnPlayerDamaged(uint16_t currentHP);

	/// <summary>
	/// 敵を倒したときの処理
	/// </summary>
	void OnEnemyDefeated();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// ビネットエフェクトの更新
	/// </summary>
	void UpdateVignetteEffect();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// ビネットエフェクト用変数
	bool isDamageVignetteActive_ = false; // ダメージ時の一時ビネット有効フラグ
	float damageVignetteTimer_ = 0.0f; // ダメージビネットのタイマー
	const float kDamageVignetteDuration_ = 1.0f; // ダメージビネットの持続時間 (秒)

	/// ===== 借りポインタ・インスタンス ===== ///

	// プレイヤーの借りポインタ
	Player* player_ = nullptr;

	// ゴールの借りポインタ
	Goal* goal_ = nullptr;

	// ガイドUI
	std::unique_ptr<GuideUI> guideUI_ = nullptr;

	// カメラコントローラーの借りポインタ
	ICameraController* cameraController_ = nullptr;

	// フィルターマネージャのインスタンス
	Engine::FilterManager* filterManager_ = nullptr;

	// ビネットフィルター借りポインタ
	Engine::VignetteFilter* vignetteFilter_ = nullptr;
};