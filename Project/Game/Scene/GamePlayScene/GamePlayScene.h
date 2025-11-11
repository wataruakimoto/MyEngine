#pragma once
#include "BaseScene.h"
#include "Collision/CollisionManager.h"
#include "OffscreenRendering/FilterManager.h"
#include "Particle/ParticleCommon.h"
#include "Particle/ParticleSystem.h"
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
#include "UI/WarningUI.h"
#include "Fade/whiteFade.h"

#include <list>
#include <sstream>
#include <memory>
#include <optional>

class RadialBlurFilter;

enum class PlayFlowState {
	SpeedDown,	// 自機を減速させる
	ShowUI,		// UI表示
	Play,		// プレイ
	WhiteFade,	// 白フェード
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
	/// 描画
	/// </summary>
	void Draw() override;

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

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// ノルマクリアのチェック
	/// </summary>
	/// <returns></returns>
	bool CheckNormaClear();

	void SpeedDownInitialize();

	void SpeedDownUpdate();

	void ShowUIInitialize();

	void ShowUIUpdate();

	void PlayInitialize();

	void PlayUpdate();

	void WhiteFadeInitialize();

	void WhiteFadeUpdate();

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// フィルターマネージャのインスタンス
	FilterManager* filterManager_ = FilterManager::GetInstance();

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

	// クリアのノルマ
	const int kClearNorma_ = 5; // とりあえず5体に設定

	// ゴールラインのZ座標
	const float kGoalLineZ = 1700.0f;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWait_ = true;

	// 待機タイマー
	int32_t standbyTimer_ = 0;

	// プレイの流れの状態
	PlayFlowState playFlowState_ = PlayFlowState::SpeedDown;

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

	// プレイヤーの移動速度
	float playerMoveSpeed_ = 4.0f;

	// プレイ時のプレイヤーの移動速度
	const float kPlayerMoveSpeedPlay = 0.5f;

	// 警告UI
	std::unique_ptr<WarningUI> warningUI_ = nullptr;

	// パーティクルシステムのインスタンス
	ParticleSystem* particleSystem = ParticleSystem::GetInstance();

	// パーティクル共通のインスタンス
	ParticleCommon* particleCommon = ParticleCommon::GetInstance();

	// 白フェードのポインタ
	std::unique_ptr<WhiteFade> whiteFade_ = nullptr;
};