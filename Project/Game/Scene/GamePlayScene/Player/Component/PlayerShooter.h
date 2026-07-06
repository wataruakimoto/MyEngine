#pragma once

#include "WorldTransform.h"
#include "Vector3.h"

/// === 前方宣言 === ///

class GameObjectManager;
class Reticle;

namespace Engine {
	class Input;
}

/// <summary>
/// プレイヤー射撃コンポーネント
/// </summary>
class PlayerShooter {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Engine::WorldTransform* playerWorldTransform, Engine::Vector3* defaultScale, Reticle* reticle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 射撃
	/// </summary>
	void Fire();

	/// <summary>
	/// 射撃アニメーション
	/// </summary>
	void FireAnimation();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	/// ========== 射撃用 ========== ///

	// 射撃間隔タイマー
	float fireTimer_ = 0.0f;

	// 射撃間隔
	const float kFireInterval_ = 0.2f;

	/// ========== アニメーション用 ========== ///

	// アニメーション中フラグ
	bool isFireAnimating_ = false;

	// アニメーションの進行度 0→1
	float fireAnimationProgress_ = 0.0f;

	// アニメーションの時間
	const float kFireAnimationDuration_ = 0.4f;

	// 射撃時の大きさ
	const Engine::Vector3 kFireScale_ = { 1.5f, 1.5f, 1.5f };

	/// ========== プレイヤーから受け取ってくる変数 ========== ///

	// プレイヤーのワールド変換
	Engine::WorldTransform* playerWorldTransform_ = nullptr;

	// プレイヤーのデフォルトの大きさ
	Engine::Vector3 defaultScale_ = { 1.0f, 1.0f, 1.0f };

	// レティクル
	Reticle* reticle_ = nullptr;

	/// ========== ポインタ ========== ///

	// ゲームオブジェクトマネージャーのポインタ
	GameObjectManager* objManager_ = nullptr;

	// 入力のポインタ
	Engine::Input* input_ = nullptr;
};

