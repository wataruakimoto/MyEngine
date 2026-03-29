#pragma once
#include "BaseScene.h"
#include "PlayState.h"
#include "Collision/CollisionManager.h"
#include "Particle/ParticleManager.h"
#include "Player/Player.h"
#include "Player/Bullet.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"
#include "Camera.h"
#include "CameraControll/ICameraController.h"
#include "Floor/Floor.h"
#include "Cylinder/Cylinder.h"
#include "UI/RuleUI.h"
#include "UI/NormaUI.h"
#include "UI/ResultUI.h"
#include "UI/GuideUI.h"
#include "Fade/whiteFade.h"
#include "Fade/BlackFade.h"
#include "Goal/Goal.h"
#include "LevelLoader.h"

#include <list>
#include <memory>

/// ===== 前方宣言 ===== ///

namespace Engine {

	class SpriteRenderer;
	class Object3dRenderer;
	class ParticleRenderer;
	class LineManager;
}

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
	/// 状態を変更
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IPlayState> newState);

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
	/// リストで管理しているオブジェクトの更新
	/// </summary>
	void UpdateListObjects();

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
	/// 倒した数をカウントアップ
	/// </summary>
	void AddKillCount() { killCount_++; }

	/// <summary>
	/// ポーズの切り替え
	/// </summary>
	void TogglePause();

	/// <summary>
	/// リスタート処理
	/// </summary>
	void Restart();

///-------------------------------------------/// 
/// クラス内関数
///-------------------------------------------///
private:

	/// <summary>
	/// レベルデータの読み込みと適用
	/// </summary>
	void LoadLevelAndApply();

	/// <summary>
	/// レベルデータから敵をスポーン
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	void SpawnEnemiesFromLevelData(const GameLevelData& levelData);

	/// <summary>
	/// オリジンシフトの確認と実行
	/// </summary>
	void CheckOriginShift();

	/// <summary>
	/// オブジェクトを手前にずらす
	/// </summary>
	/// <param name="shiftZ">手前にずらす量</param>
	void ShiftWorld(float shiftZ);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	const int& GetKillCount() const { return killCount_; }

	ICameraController* GetCameraController() { return cameraController_.get(); }

	Player* GetPlayer() { return player_.get(); }

	Goal* GetGoal() { return goal_.get(); }

	RuleUI* GetRuleUI() { return ruleUI_.get(); }

	NormaUI* GetNormaUI() { return normaUI_.get(); }

	GuideUI* GetGuideUI() { return guideUI_.get(); }

	ResultUI* GetResultUI() { return resultUI_.get(); }

	WhiteFade* GetWhiteFade() { return whiteFade_.get(); }

	BlackFade* GetBlackFade() { return blackFade_.get(); }

	const std::list<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	std::unique_ptr<IPlayState> state_ = nullptr;
	std::unique_ptr<IPlayState> pauseState_ = nullptr;

	// 敵を倒した数
	int killCount_ = 0;

	// ループする距離
	const float kLoopDistance = 1000.0f;

	/// ========== レベルデータ ========== ///

	LevelLoader levelLoader_;

	const std::string kLevelDataFileName_ = "LevelData.json";

	/// ===== オブジェクト ===== ///

	// カメラコントローラのポインタ
	std::unique_ptr<ICameraController> cameraController_ = nullptr;

	// プレイヤーのポインタ
	std::unique_ptr<Player> player_ = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

	// 自機の弾のリスト
	std::list<std::unique_ptr<Bullet>> playerBullets_;

	// 敵の弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	// フロアのポインタ
	std::unique_ptr<Floor> floor_ = nullptr;

	// シリンダーのポインタ
	std::unique_ptr<Cylinder> cylinder_ = nullptr;

	// ゴールのポインタ
	std::unique_ptr<Goal> goal_ = nullptr;

	/// ===== スプライト ===== ///

	// ルールUI
	std::unique_ptr<RuleUI> ruleUI_ = nullptr;

	// ノルマUI
	std::unique_ptr<NormaUI> normaUI_ = nullptr;

	// ガイドUI
	std::unique_ptr<GuideUI> guideUI_ = nullptr;

	// リザルトUI
	std::unique_ptr<ResultUI> resultUI_ = nullptr;

	// 白フェードのポインタ
	std::unique_ptr<WhiteFade> whiteFade_ = nullptr;

	// 黒フェードのポインタ
	std::unique_ptr<BlackFade> blackFade_ = nullptr;

	/// ===== エンジン ===== ///

	// カメラ
	std::unique_ptr<Engine::Camera> camera_ = nullptr;

	// 衝突マネージャのポインタ
	std::unique_ptr<Engine::CollisionManager> collisionManager_ = nullptr;

	// パーティクルマネージャのインスタンス
	Engine::ParticleManager* particleManager_ = Engine::ParticleManager::GetInstance();

	// スプライトレンダラーのインスタンス
	Engine::SpriteRenderer* spriteRenderer_ = nullptr;

	// 3Dオブジェクトレンダラーのインスタンス
	Engine::Object3dRenderer* object3dRenderer_ = nullptr;

	// パーティクルレンダラーのインスタンス
	Engine::ParticleRenderer* particleRenderer_ = nullptr;

	// ラインマネージャのインスタンス
	Engine::LineManager* lineManager_ = nullptr;
};