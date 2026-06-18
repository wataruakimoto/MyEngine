#pragma once

#include "IPlayerState.h"

/// <summary>
/// オートパイロット状態
/// </summary>
class PlayerAutoState : public IPlayerState {

	/// ================================================== ///
	/// メンバ関数
	/// ================================================== ///
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
	/// 状態に入ったときの処理
	/// </summary>
	void Enter() override;

	/// <summary>
	/// 状態から出るときの処理
	/// </summary>
	void Exit() override;

	/// ================================================== ///
	/// クラス内関数
	/// ================================================== ///
private:

	/// <summary>
	/// 加速処理
	/// </summary>
	void Accelerate();

	/// <summary>
	/// 減速処理
	/// </summary>
	void Decelerate();

	/// ================================================== ///
	/// メンバ変数
	/// ================================================== ///
private:

	// 移動の速さ
	float moveSpeed_ = 0.1f;

	// 加速かどうか
	bool isAccelerating_ = false;

	// 減速かどうか
	bool isDecelerating_ = false;

	// 加速の最大値
	const float maxMoveSpeed_ = 0.5f;

	// 減速の最小値
	const float minMoveSpeed_ = 0.0f;

	// 速度
	Engine::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
};

