#pragma once

#include "Model/Model.h"
#include "Object/Object3d.h"
#include "Collision/Basecharacter.h"
#include "State/IPlayerState.h"

#include <memory>
#include <optional>
#include <numbers>
#include <unordered_map>
#include <typeindex>

/// === 前方宣言 === ///

class GamePlayScene;

namespace Engine {
	class Camera;
}

/// <summary>
/// プレイヤーの状態
/// </summary>
enum class PlayerState {
	AutoPilot,
	Manual,
};

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public BaseCharacter {

	/// ================================================== ///
	// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	
	/// <summary>
	///	初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Ui描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui表示
	/// </summary>
	void ShowImGui();

	/// <summary>
	/// 衝突時の処理
	/// </summary>
	void OnCollision(Engine::Collider * other) override;

	/// ================================================== ///
	/// テンプレート関数
	/// ================================================== ///
public:

	/// <summary>
	/// 状態変更
	/// </summary>
	template <typename T>
	void ChangeState() {

		// すでに状態を持っていたら
		if (currentState_) {

			// 現在の状態から出るときの処理を呼び出す
			currentState_->Exit();
		}

		// マップから新しい状態を取得
		currentState_ = states_[typeid(T)].get();

		// 新しい状態を持っていたら
		if (currentState_) {

			// 新しい状態に入ったときの処理を呼び出す
			currentState_->Enter();
		}
	}

	/// ================================================== ///
	/// ゲッター
	/// ================================================== ///
public:

	/// <summary>
	/// 状態のゲッター
	/// </summary>
	/// <returns>状態</returns>
	PlayerState GetState() const { return state_; }

	/// <summary>
	/// オート操縦時の移動の速さのゲッター
	/// </summary>
	/// <returns>オート操縦時の移動の速さ</returns>
	float GetMoveSpeedAuto() const { return moveSpeedAuto_; }

	/// <summary>
	/// 速度のゲッター
	/// </summary>
	/// <returns>速度</returns>
	const Engine::Vector3& GetVelocity() const { return velocity_; }

	/// <summary>
	/// スクリーン座標のゲッター
	/// </summary>
	/// <returns>スクリーン座標</returns>
	Engine::Vector2 GetScreenPos() const { return screenPos_; }

	/// ================================================== ///
	/// セッター
	/// ================================================== ///
public:

	/// <summary>
	/// ゲームプレイシーンのセッター
	/// </summary>
	/// <param name="scene"></param>
	void SetGamePlayScene(GamePlayScene* scene) { gamePlayScene_ = scene; }

	/// <summary>
	/// カメラのセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Engine::Camera* camera) { camera_ = camera; }

	/// <summary>
	/// 状態変更リクエストのセッター
	/// </summary>
	/// <param name="state"></param>
	void SetPlayerState(PlayerState state) { stateRequest_ = state; }

	/// <summary>
	/// オート操縦時の移動の速さのセッター
	/// </summary>
	/// <param name="speed">速さ</param>
	void SetMoveSpeedAuto(float speed) { moveSpeedAuto_ = speed; }

	/// ================================================== /// 
	/// メンバ変数
	/// ================================================== ///
private:

	/// ========== ポインタ・インスタンス ========== ///

	// モデルのポインタ
	std::unique_ptr<Engine::Model> model = nullptr;

	// 3Dオブジェクトのポインタ
	std::unique_ptr<Engine::Object3d> object = nullptr;

	// カメラの借りポインタ
	Engine::Camera* camera_ = nullptr;

	// ゲームプレイシーンの借りポインタ
	GamePlayScene* gamePlayScene_ = nullptr;

	/// ========== 状態用 ========== ///

	// すべての状態を保存しておく箱
	std::unordered_map<std::type_index, std::unique_ptr<IPlayerState>> states_;

	// 現在の状態(ポインタ)
	IPlayerState* currentState_ = nullptr;

	// 現在の状態(構造体)
	PlayerState state_ = PlayerState::Manual;

	// 状態変更リクエスト
	std::optional<PlayerState> stateRequest_ = std::nullopt;

	/// ========== 速度パラメータ ========== ///

	// オート操縦時の移動の速さ
	float moveSpeedAuto_ = 0.0f;

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	/// ========== その他 ========== ///

	Engine::Vector3 defaultScale_ = { 2.0f, 2.0f, 2.0f };

	// スクリーン座標
	Engine::Vector2 screenPos_ = { 0.0f, 0.0f };
};