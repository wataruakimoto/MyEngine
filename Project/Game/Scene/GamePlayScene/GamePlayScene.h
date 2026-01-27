#pragma once
#include "BaseScene.h"
#include "Collision/CollisionManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "Particle/ParticleManager.h"
#include "Particle/ParticleCommon.h"
#include "Player/Player.h"
#include "Player/Bullet.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include "Reticle/Reticle3D.h"
#include "Reticle/Reticle2D.h"
#include "LockOn/LockOn.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "SkyBox/SkyBoxGame.h"
#include "UI/RuleUI.h"
#include "UI/NormaUI.h"
#include "UI/ResultUI.h"
#include "UI/GuideUI.h"
#include "Fade/whiteFade.h"
#include "Fade/BlackFade.h"
#include "Goal/Goal.h"

#include <list>
#include <sstream>
#include <memory>
#include <optional>

class RadialBlurFilter;

enum class PlayFlowState {
	Play,		// プレイ
	Result,     // 結果表示
	WhiteFade,	// 白フェード
	BlackFade,  // 黒フェード
};

/// ===== ゲームプレイシーン ===== ///
class GamePlayScene : public BaseScene {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// フィルター適応のある描画
	/// </summary>
	void DrawFiltered() override;

	/// <summary>
	/// フィルター適応のない描画
	/// </summary>
	void DrawUnfiltered() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui() override;

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 自機の弾の追加
	/// </summary>
	/// <param name="bullet"></param>
	void AddPlayerBullet(std::unique_ptr<Bullet> bullet);

	/// <summary>
	/// 敵の弾の追加
	/// </summary>
	/// <param name="bullet"></param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> bullet);

	/// <summary>
	/// プレイヤーがダメージを受けたときの処理
	/// </summary>
	/// <param name="currentHP">現在のHP</param>
	void OnPlayerDamaged(uint16_t currentHP);

	/// <summary>
	/// 敵を倒したときの処理
	/// </summary>
	void OnEnemyDefeated();

	/// <summary>
	/// ビネットエフェクトの更新
	/// </summary>
	void UpdateVignetteEffect();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// オリジンシフトの確認と実行
	/// </summary>
	void CheckOriginShift();

	/// <summary>
	/// オブジェクトを手前にずらす
	/// </summary>
	/// <param name="shiftZ">手前にずらす量</param>
	void ShiftWorld(float shiftZ);

	/// ===== 各状態の処理 ===== ///

	void PlayInitialize();

	void PlayUpdate();

	void ResultInitialize();

	void ResultUpdate();

	void WhiteFadeInitialize();

	void WhiteFadeUpdate();

	void BlackFadeInitialize();

	void BlackFadeUpdate();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	Player* GetPlayer() { return player_.get(); }

	RuleUI* GetRuleUI() { return ruleUI_.get(); }

	Goal* GetGoal() { return goal_.get(); }

	Reticle3D* GetReticle3D() { return reticle3D_.get(); }

	Reticle2D* GetReticle2D() { return reticle2D_.get(); }

	LockOn* GetLockOn() { return lockOn_.get(); }

	NormaUI* GetNormaUI() { return normaUI_.get(); }

	GuideUI* GetGuideUI() { return guideUI_.get(); }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// フィルターマネージャのインスタンス
	FilterManager* filterManager_ = FilterManager::GetInstance();

	// パーティクルマネージャのインスタンス
	ParticleManager* particleManager_ = ParticleManager::GetInstance();

	// カメラ
	std::unique_ptr<Camera> camera_ = nullptr;

	// カメラコントローラ
	std::unique_ptr<ICameraController> cameraController_ = nullptr;

	// 衝突マネージャのポインタ
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// プレイヤーのポインタ
	std::unique_ptr<Player> player_ = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

	// 自機の弾のリスト
	std::list<std::unique_ptr<Bullet>> playerBullets_;

	// 敵の弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// 3Dレティクルのポインタ
	std::unique_ptr<Reticle3D> reticle3D_ = nullptr;

	// 2Dレティクルのポインタ
	std::unique_ptr<Reticle2D> reticle2D_ = nullptr;

	// ロックオンのポインタ
	std::unique_ptr<LockOn> lockOn_ = nullptr;

	// フロアのポインタ
	std::unique_ptr<Floor> floor_ = nullptr;

	// シリンダーのポインタ
	std::unique_ptr<Cylinder> cylinder_ = nullptr;

	// スカイボックスのポインタ
	std::unique_ptr<SkyBoxGame> skyBox_ = nullptr;

	// 敵を倒した数
	int killCount = 0;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWait_ = true;

	// 待機タイマー
	int32_t standbyTimer_ = 0;

	// プレイの流れの状態
	PlayFlowState playFlowState_ = PlayFlowState::Play;

	// 状態リクエスト
	std::optional<PlayFlowState> stateRequest_ = std::nullopt;

	// ラジアルブラー借りポインタ
	RadialBlurFilter* radialBlurFilter_ = nullptr;

	// ブラーの中心座標
	Vector2 blurCenter_ = { 0.5f, 0.5f };

	// ブラーの強さ
	float blurStrength_ = 0.07f;

	// ブラーの最小値
	const float kMinBlurStrength = 0.0f;

	// ビネットフィルター借りポインタ
	VignetteFilter* vignetteFilter_ = nullptr;

	// ビネットエフェクト用変数
	bool isDamageVignetteActive_ = false; // ダメージ時の一時ビネット有効フラグ
	float damageVignetteTimer_ = 0.0f; // ダメージビネットのタイマー
	const float kDamageVignetteDuration_ = 0.5f; // ダメージビネットの持続時間 (秒)
	uint16_t previousHP_ = 0; // 前フレームのHP

	// プレイヤーの移動速度
	float playerMoveSpeed_ = 4.0f;

	// プレイ時のプレイヤーの移動速度
	const float kPlayerMoveSpeedPlay = 0.5f;

	// ループする距離
	const float kLoopDistance = 1000.0f;

	// ルールUI
	std::unique_ptr<RuleUI> ruleUI_ = nullptr;

	// ノルマUI
	std::unique_ptr<NormaUI> normaUI_ = nullptr;

	// リザルトUI
	std::unique_ptr<ResultUI> resultUI_ = nullptr;

	// ガイドUI
	std::unique_ptr<GuideUI> guideUI_ = nullptr;

	// パーティクル共通のインスタンス
	ParticleCommon* particleCommon = ParticleCommon::GetInstance();

	// 白フェードのポインタ
	std::unique_ptr<WhiteFade> whiteFade_ = nullptr;

	// 黒フェードのポインタ
	std::unique_ptr<BlackFade> blackFade_ = nullptr;

	// ゴールのポインタ
	std::unique_ptr<Goal> goal_ = nullptr;
};