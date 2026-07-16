#pragma once
#include "BaseScene.h"
#include "Collision/CollisionManager.h"
#include "Particle/ParticleManager.h"
#include "Light/LightManager.h"

#include "CameraManager.h"
#include "LevelLoader.h"
#include "State/IPlayState.h"
#include "GameRule.h"

#include <list>
#include <memory>

/// ===== 前方宣言 ===== ///

namespace Engine {

	class SpriteRenderer;
	class Object3dRenderer;
	class ParticleRenderer;
	class LineManager;
}

class GameObjectManager;

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
	/// 敵を倒したときの処理
	/// </summary>
	void OnEnemyDefeated();

	/// <summary>
	/// ゴールに到達したときの処理
	/// </summary>
	void OnGoalReached();

	/// <summary>
	/// プレイヤーがダメージを受けたときの処理
	/// </summary>
	void OnPlayerDamaged();

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
	/// オリジンシフトの確認と実行
	/// </summary>
	void CheckOriginShift();

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	CameraManager* GetCameraManager() { return cameraManager_.get(); }

	GameRule* GetGameRule() { return gameRule_.get(); }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	// オブジェクトマネージャーのインスタンス
	GameObjectManager* gameObjectManager_ = nullptr;

	// カメラマネージャー
	std::unique_ptr<CameraManager> cameraManager_ = nullptr;

	// レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;
	// レベルデータのファイル名
	const std::string kLevelDataFileName_ = "LevelData.json";
	
	std::unique_ptr<IPlayState> state_ = nullptr;
	std::unique_ptr<IPlayState> pauseState_ = nullptr;

	// ゲームルール
	std::unique_ptr<GameRule> gameRule_ = nullptr;

	// ループする距離
	const float kLoopDistance = 1000.0f;

	/// ===== エンジン ===== ///

	// 衝突マネージャのポインタ
	std::unique_ptr<Engine::CollisionManager> collisionManager_ = nullptr;

	// ライトマネージャのポインタ
	std::unique_ptr<Engine::LightManager> lightManager_ = nullptr;

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