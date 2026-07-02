#pragma once

#include "WorldTransform.h"
#include "Vector3.h"

/// <summary>
/// プレイヤー移動コンポーネント
/// </summary>
class PlayerMover {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// レティクルに向かって移動
	/// </summary>
	void MoveToReticle();

	/// <summary>
	/// 移動範囲の制限
	/// </summary>
	void ClampPosition();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 移動範囲の制限 半径
	const float kMaxRadius_ = 25.0f;

	// 地面の高さの制限
	const float kMinHeight_ = 0.0f;

	/// ========== プレイヤーから受け取ってくる変数 ========== ///

	// プレイヤーのワールド変換
	Engine::WorldTransform* playerWorldTransform_ = nullptr;

	// レティクルの座標
	Engine::Vector3 reticlePos_ = { 0.0f, 0.0f, 0.0f };

	float moveSpeed_ = 0.0f;
};

